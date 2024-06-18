#include "maichongwdt.h"

#include <QDoubleSpinBox>

#include "log.h"
#include "makepacket.h"
#include "ui_maichongwdt.h"
maichongwdt::maichongwdt(QWidget *parent)
    : QWidget(parent), ui(new Ui::maichongwdt) {
  ui->setupUi(this);
  mTbSpin1 = new QVector<QSpinBox *>;
  mTbSpin2 = new QVector<QSpinBox *>;
  mTbItem1 = new QVector<QTableWidgetItem *>;
  mTbItem2 = new QVector<QTableWidgetItem *>;
  init();
  setFocusPolicy(Qt::ClickFocus);
}

maichongwdt::~maichongwdt() {
  mTbItem1->clear();
  delete mTbItem1;
  mTbItem2->clear();
  delete mTbItem2;
  mTbSpin1->clear();
  mTbSpin2->clear();
  delete mTbSpin1;
  delete mTbSpin2;
  delete ui;
}
void maichongwdt::receiveQuery(QString name, queryInfo info, int a) {
  // sender()->blockSignals(true);
  updataHome(name, info, a);
  // sender()->blockSignals(false);
}

void maichongwdt::editFinish() {
  QSpinBox *p = (QSpinBox *)sender();
  QByteArray val;
  uint16_t v = p->value();
  uint32_t cmd = p->property("id").toInt();
  uint16_t pVal = p->property("val").toInt();

  if (pVal == v) return;

  // makePacket<uint16_t>(v, sizeof(v), val);
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << v;
  emit sendcmd(cmd, val);
  // mportMg->send(cmd, val);
}
void maichongwdt::cmdToPortMg(const uint32_t &cmd, const QByteArray &data) {
  mportMg->send(cmd, data);
}
void maichongwdt::updataTable(QTableWidget *t, const QVector<uint16_t> &v,
                              const uint32_t &cmd) {
  if (t == nullptr) return;
  if (t->property("Init").isValid() == false) return;
  bool isInit = t->property("Init").toBool();

  auto id = t->property("id").toInt();
  QVector<QSpinBox *> *p = nullptr;
  QVector<QTableWidgetItem *> *pItem = nullptr;
  if (id == 1) {
    p = mTbSpin1;
    pItem = mTbItem1;
  } else if (id == 2) {
    p = mTbSpin2;
    pItem = mTbItem2;
  }
  if (v.isEmpty()) return;
  if (isInit == false) {
    while (t->rowCount() > 0) {
      t->removeRow(0);
    }
    t->clearContents();

    int i = 1;

    QFont font;
    font.setPointSize(20);

    for (auto iter : v) {
      int iRow = t->rowCount();
      t->setRowCount(iRow + 1);
      auto number = new QTableWidgetItem(QString::number(i));
      number->setFont(font);
      t->setItem(iRow, 0, number);
      auto item = t->item(iRow, 0);
      item->setTextAlignment(Qt::AlignCenter);
      item->setFlags(item->flags() & (~Qt::ItemIsEditable));

      auto dataItem = new QTableWidgetItem(QString::number(iter));
      dataItem->setFont(font);
      t->setItem(iRow, 1, dataItem);

      pItem->push_back(dataItem);

      item = t->item(iRow, 1);
      item->setFlags(item->flags() & (~Qt::ItemIsEditable));
      item->setTextAlignment(Qt::AlignCenter);
      auto c1 = new QSpinBox(t);
      c1->setFont(font);
      if (c1 == nullptr) {
        p->clear();

        return;
      }
      p->push_back(c1);
      if (cmd == SX1_DELAY) {
        switch (i) {
          case 1:
            c1->setProperty("id", SX1_DELAY);
            break;
          case 2:
            c1->setProperty("id", SX2_DELAY);
            break;
          case 3:
            c1->setProperty("id", SX3_DELAY);
            break;
          case 4:
            c1->setProperty("id", SX4_DELAY);
            break;
          case 5:
            c1->setProperty("id", SX5_DELAY);
            break;
          case 6:
            c1->setProperty("id", SX6_DELAY);
            break;
          default:
            c1->setProperty("id", SX1_DELAY);
            break;
        }
      } else
        c1->setProperty("id", i - 1 + cmd);
      c1->setProperty("val", iter);
      c1->setMaximum(744);
      c1->setMinimum(0);
      c1->setValue(iter);
      QObject::connect(c1, &QSpinBox::editingFinished, this,
                       &maichongwdt::editFinish);

      t->setCellWidget(iRow, 2, c1);

      i++;
    }
    t->setProperty("Init", true);
    return;
  }
  int i = 0;
  if (p == nullptr) return;
  for (auto iter : v) {
    auto val = pItem->at(i)->text();

    if (val.toUShort() != iter) {
      pItem->at(i)->setText(QString::number(iter));
    }
    // if (p->at(i)->value() != iter) {
    //  p->at(i)->blockSignals(true);
    //  p->at(i)->setValue(iter);

    //  p->at(i)->blockSignals(false);
    //  pItem->at(i)->setText(QString::number(iter));
    //}

    i++;
  }
}
bool maichongwdt::updataHome(QStringView name, const queryInfo &info, int a) {
  if (a != 1 && a != 0) return false;

  updataTable(ui->mTable, info.SXMK, SX1_MK);
  updataTable(ui->mXiaohaoTab, info.SX_YS, SX1_DELAY);
  // updataTable(ui->mXiaohaoTab, info.XHMK, XH1_MK);

  return true;
}
bool maichongwdt::init() {
  ui->mTable->setProperty("Init", false);
  ui->mTable->setProperty("id", 1);

  ui->mXiaohaoTab->setProperty("Init", false);
  ui->mXiaohaoTab->setProperty("id", 2);

  ui->mTable->setColumnCount(3);
  // ui->mTable->setRowCount(10);
  ui->mTable->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);  //平均分配列宽
  // ui->mTable->verticalHeader()->setSectionResizeMode(
  //   QHeaderView::Stretch);  //平均分配行宽

  ui->mTable->verticalHeader()->setVisible(false);
  QStringList headerText;
  headerText << tr("mai kuan name") << tr("value") << tr("change");
  ui->mTable->setHorizontalHeaderLabels(headerText);
  ui->mTable->verticalHeader()->setDefaultSectionSize(45);

  ui->mXiaohaoTab->setColumnCount(3);
  // ui->mTable->setRowCount(10);
  ui->mXiaohaoTab->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);  //平均分配列宽
  // ui->mXiaohaoTab->verticalHeader()->setSectionResizeMode(
  //  QHeaderView::Stretch);  //平均分配行宽
  // ui->mXiaohaoTab->verticalHeader()->setDefaultSectionSize(15);

  ui->mXiaohaoTab->verticalHeader()->setVisible(false);
  ui->mXiaohaoTab->verticalHeader()->setDefaultSectionSize(45);
  // QStringList headerText;
  headerText.clear();
  headerText << tr("shi xu name") << tr("value") << tr("change");
  ui->mXiaohaoTab->setHorizontalHeaderLabels(headerText);
  ui->mTable->setShowGrid(false);
  ui->mXiaohaoTab->setShowGrid(false);

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  connect(this, &maichongwdt::sendcmd, this, &maichongwdt::cmdToPortMg,
          Qt::QueuedConnection);

  return true;
}
