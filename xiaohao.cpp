#include "xiaohao.h"

#include <QDoubleSpinBox>

#include "log.h"
#include "ui_xiaohao.h"

xiaohao::xiaohao(QWidget *parent) : QWidget(parent), ui(new Ui::xiaohao) {
  ui->setupUi(this);
  mXhSpin1 = new QVector<QSpinBox *>;
  mXhSpin2 = new QVector<QSpinBox *>;
  mXhItem1 = new QVector<QTableWidgetItem *>;
  mXhItem2 = new QVector<QTableWidgetItem *>;
  init();
}
void xiaohao::receiveQuery(QString name, queryInfo info, int a) {
  // sender()->blockSignals(true);
  updataHome(name, info, a);
  // sender()->blockSignals(false);
}
void xiaohao::editFinish() {
  ui->mTable->setEnabled(false);
  ui->mXiaohaoTab->setEnabled(false);
  QSpinBox *p = (QSpinBox *)sender();

  QByteArray val;
  uint16_t v = p->value();
  uint32_t cmd = p->property("id").toInt();
  uint16_t pVal = p->property("val").toInt();
  uint16_t index = p->property("index").toInt();
  if (cmd == XH1_DELAY && index >= 1) {
    index -= 1;
    cmd = XH2_DELAY;
  };

  if (cmd == XH2_DELAY && index >= 5) {
    index -= 5;
    cmd = XH7_DELAY;
  }
  cmd += index;
  if (pVal == v) {
    ui->mXiaohaoTab->setEnabled(true);
    ui->mTable->setEnabled(true);
    return;
  }
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << v;

  mportMg->send(cmd, val);
  ui->mXiaohaoTab->setEnabled(true);
  ui->mTable->setEnabled(true);
  // emit sendcmd(cmd, val);
}
bool xiaohao::updataHome(QStringView name, const queryInfo &info, int a) {
  if (a == 0 || a == 1) {
    updataTable(ui->mTable, info.XHMK, XH1_MK);
    updataTable(ui->mXiaohaoTab, info.XH_YS, XH1_DELAY);
  }
  return true;
}
xiaohao::~xiaohao() {
  mXhItem1->clear();
  delete mXhItem1;
  mXhItem2->clear();
  delete mXhItem2;
  mXhSpin1->clear();
  mXhSpin2->clear();
  delete mXhSpin1;
  delete mXhSpin2;
  delete ui;
}
bool xiaohao::init() {
  ui->mTable->setProperty("Init", false);
  ui->mTable->setProperty("id", 1);

  ui->mXiaohaoTab->setProperty("Init", false);
  ui->mXiaohaoTab->setProperty("id", 2);

  ui->mTable->setColumnCount(3);

  ui->mTable->horizontalHeader()->setSectionResizeMode(
    QHeaderView::Stretch);  //平均分配列宽
  // ui->mTable->verticalHeader()->setSectionResizeMode(
  // QHeaderView::Stretch);  //平均分配行宽

  ui->mTable->verticalHeader()->setVisible(false);
  QStringList headerText;
  headerText << tr("mai kuan") << tr("value") << tr("change");
  ui->mTable->setHorizontalHeaderLabels(headerText);
  ui->mTable->verticalHeader()->setDefaultSectionSize(45);
  ui->mXiaohaoTab->setColumnCount(3);
  // ui->mTable->setRowCount(10);
  ui->mXiaohaoTab->horizontalHeader()->setSectionResizeMode(
    QHeaderView::Stretch);  //平均分配列宽
  // ui->mXiaohaoTab->verticalHeader()->setSectionResizeMode(
  //  QHeaderView::Stretch);  //平均分配行宽

  ui->mXiaohaoTab->verticalHeader()->setVisible(false);
  ui->mXiaohaoTab->verticalHeader()->setDefaultSectionSize(45);
  // QStringList headerText;
  headerText.clear();
  headerText << tr("xiao hao name") << tr("value") << tr("change");
  ui->mXiaohaoTab->setHorizontalHeaderLabels(headerText);
  ui->mTable->setShowGrid(true);
  ui->mXiaohaoTab->setShowGrid(true);

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  connect(this, &xiaohao::sendcmd, this, &xiaohao::cmdToPortMg);
  return true;
}

void xiaohao::updataTable(QTableWidget *t, const QVector<uint16_t> &v,
                          const uint32_t &cmd) {
  if (t == nullptr) return;
  if (t->property("Init").isValid() == false) return;
  bool isInit = t->property("Init").toBool();

  auto id = t->property("id").toInt();
  QVector<QSpinBox *> *p = nullptr;
  QVector<QTableWidgetItem *> *pItem = nullptr;
  if (id == 1) {
    p = mXhSpin1;
    pItem = mXhItem1;
  } else if (id == 2) {
    p = mXhSpin2;
    pItem = mXhItem2;
  }
  if (v.isEmpty()) return;
  if (isInit == false) {
    while (t->rowCount() > 0) {
      t->removeRow(0);
    }
    t->clearContents();

    int i = 1;

    for (auto iter : v) {
      int iRow = t->rowCount();
      t->setRowCount(iRow + 1);
      t->setItem(iRow, 0, new QTableWidgetItem(QString::number(i)));
      auto item = t->item(iRow, 0);
      item->setTextAlignment(Qt::AlignCenter);
      item->setFlags(item->flags() & (~Qt::ItemIsEditable));
      auto dataItem = new QTableWidgetItem(QString::number(iter));
      t->setItem(iRow, 1, dataItem);

      pItem->push_back(dataItem);
      item = t->item(iRow, 1);
      item->setFlags(item->flags() & (~Qt::ItemIsEditable));
      item->setTextAlignment(Qt::AlignCenter);
      auto c1 = new QSpinBox(t);
      if (c1 == nullptr) {
        p->clear();

        return;
      }
      p->push_back(c1);

      c1->setProperty("index", i - 1);
      c1->setProperty("id", cmd);
      c1->setProperty("val", iter);
      c1->setMaximum(744);
      c1->setMinimum(0);
      c1->setValue(iter);
      QObject::connect(c1, &QSpinBox::editingFinished, this,
                       &xiaohao::editFinish);

      t->setCellWidget(iRow, 2, c1);

      i++;
    }
    t->setProperty("Init", true);
    return;
  }
  int i = 0;
  for (auto iter : v) {
    auto val = pItem->at(i)->text();

    if (val.toUShort() != iter) {
      pItem->at(i)->setText(QString::number(iter));
    }
    //    if (p->at(i)->value() != iter) {
    //      p->at(i)->blockSignals(true);
    //      p->at(i)->setValue(iter);
    //      p->at(i)->blockSignals(false);
    //      pItem->at(i)->setText(QString::number(iter));
    //    }

    i++;
  }
}

void xiaohao::cmdToPortMg(const uint32_t &cmd, const QByteArray &data) {
  mportMg->send(cmd, data);
}
