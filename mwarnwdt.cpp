//#pragma execution_character_set("utf-8")
#include "mwarnwdt.h"

#include <qmath.h>

#include <QPainter>
#include <QStyleFactory>
#include <QStyleOption>
#include <QTime>
#include <bitset>

#include "defer.h"
#include "log.h"
#include "mlogindlg.h"
#include "ui_mwarnwdt.h"
mwarnwdt::mwarnwdt(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::mwarnwdt),
    mBitsChange(true),
    mBitsChange2(true),
    mIsRoleChange(false) {
  ui->setupUi(this);
  init();
  setProperty("icon", ":/img/jg.png");
}

mwarnwdt::~mwarnwdt() { delete ui; }

void mwarnwdt::init() {
  QStringList strList;
  mBits.reset();

  tableInit(ui->mTree1);
  tableInit(ui->mTree2);

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  QObject::connect(loginDlg, &mLoginDlg::logRoleChanged, this,
                   &mwarnwdt::logRoleChanged);

  // enableTree(false);
}
void mwarnwdt::treeItemChanged(QTreeWidgetItem *item, int column) {}

void mwarnwdt::makeBits(const queryInfo &info) {
  uint8_t b1, b2, b3;
  // mBits.reset();
  std::bitset<24> bits;

  b1 = info.Warning1_switch;
  b2 = info.Warning2_switch;
  b3 = info.Warning3_switch;

  mSwitch1 = b1;
  mSwitch2 = b2;
  mSwitch3 = b3;

  // semp
  if (b1 & 0x04) bits.set(0, 1);
  if (b1 & 0x02) bits.set(1, 1);
  if (b1 & 0x01) bits.set(2, 1);

  if (b3 & 0x04) bits.set(3, 1);
  if (b3 & 0x02) bits.set(4, 1);
  // 晶体
  if (b1 & 0x80) bits.set(5, 1);
  if (b1 & 0x40) bits.set(6, 1);

  if (b3 & 0x80) bits.set(7, 1);
  if (b3 & 0x40) bits.set(8, 1);
  if (b3 & 0x20) bits.set(9, 1);

  // 腔体
  // 湿度
  if (b1 & 0x20) bits.set(10, 1);
  if (b1 & 0x08) bits.set(11, 1);

  /// 温度

  if (b2 & 0x01) bits.set(12, 1);
  if (b2 & 0x02) bits.set(13, 1);
  // 水流
  if (b1 & 0x10) bits.set(14, 1);
  if (b3 & 0x10) bits.set(15, 1);

  if (mBits != bits) {
    mBitsChange = true;
    mBits = bits;
  }
  QLOG_DEBUG() << "bits_2" << QString::fromStdString(mBits.to_string());
}

void mwarnwdt::slotClicked(QTreeWidgetItem *item, int column) {}

void mwarnwdt::iChanged(QTreeWidgetItem *item) {}

void mwarnwdt::logRoleChanged(uint8_t id) {
  mIsRoleChange = true;
  if (id != 0)
    enableTree(true);
  else
    enableTree(false);

  mRoleId = id;
  mIsRoleChange = false;
}

void mwarnwdt::sendCmd(int index, bool &ischeck) {
  QByteArray val;
  uint8_t v = 0;
  mIsSendOver[0] = true;
  mIsSendOver[1] = true;
  switch (index) {
    case 0:
      v = 0x04;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);
      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;
    case 1:
      // val.push_back(0x02);
      v = 0x02;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);
      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;
    case 2:
      // val.push_back(0x01);
      v = 0x01;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;

    case 3:
      // val.push_back(0x04);
      v = 0x04;
      if (ischeck)
        v |= mSwitch3;
      else
        v = ~v & mSwitch3;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH3, val, false, false);
      mSwitch3 = v;
      break;
    case 4:
      // val.push_back(0x02);

      v = 0x02;
      if (ischeck)
        v |= mSwitch3;
      else
        v = ~v & mSwitch3;

      val.push_back(v);
      mportMg->send(WARMING_SWITCH3, val, false, false);
      mSwitch3 = v;
      break;
    //
    case 5:
      // val.push_back(0x80);

      v = 0x80;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;
    case 6:
      // val.push_back(0x40);

      v = 0x40;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);
      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;
    case 7:
      // val.push_back(0x80);

      v = 0x80;
      if (ischeck)
        v |= mSwitch3;
      else
        v = ~v & mSwitch3;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH3, val, false, false);
      mSwitch3 = v;
      break;

    case 8:
      // val.push_back(0x40);

      v = 0x40;
      if (ischeck)
        v |= mSwitch3;
      else
        v = ~v & mSwitch3;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH3, val, false, false);
      mSwitch3 = v;
      break;
    case 9:
      // val.push_back(0x20);

      v = 0x20;
      if (ischeck)
        v |= mSwitch3;
      else
        v = ~v & mSwitch3;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH3, val, false, false);
      mSwitch3 = v;
      break;
    //
    case 10:
      // val.push_back(0x20);

      v = 0x20;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;
    case 11:
      // val.push_back(0x8);

      v = 0x8;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;

    case 12:
      // val.push_back(0x40);

      v = 0x01;
      if (ischeck)
        v |= mSwitch2;
      else
        v = ~v & mSwitch2;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH2, val, false, false);
      mSwitch2 = v;
      break;
    case 13:
      // val.push_back(0x20);

      v = 0x02;
      if (ischeck)
        v |= mSwitch2;
      else
        v = ~v & mSwitch2;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH2, val, false, false);
      mSwitch2 = v;
      break;

    case 14:
      // val.push_back(0x10);

      v = 0x10;
      if (ischeck)
        v |= mSwitch1;
      else
        v = ~v & mSwitch1;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH1, val, false, false);
      mSwitch1 = v;
      break;
    case 15:

      v = 0x10;
      if (ischeck)
        v |= mSwitch3;
      else
        v = ~v & mSwitch3;

      val.push_back(v);

      mportMg->send(WARMING_SWITCH3, val, false, false);
      mSwitch3 = v;
      break;

    default:
      break;
  }
}

bool mwarnwdt::updataGUI(QStringView cmd, const queryInfo &info, int a) {
  // sender()->blockSignals(true);
  if (a == 0) {
    makeBits(info);
    queryData1(info);
  }
  if (a == 1) {
    queryData2(info);
  }
  // sender()->blockSignals(false);
#if 0
  if (a == 0) {
    makeBits(info);
    if (mBitsChange || mIsSendOver[0]) queryData1(info);
    mBitsChange = false;
    mIsSendOver[0] = false;

  } else if (a == 1) {
    if (mBitsChange2 || mIsSendOver[1]) queryData2(info);
    mBitsChange2 = false;
    mIsSendOver[1] = false;
  }
  // this->blockSignals(true);
  // ui->mTree1->blockSignals(false);
  // ui->mTree2->blockSignals(false);

  ui->mTree1->setItemsExpandable(true);
  ui->mTree2->setItemsExpandable(true);
  ui->mTree1->expandAll();
  ui->mTree2->expandAll();
#endif

  if (mRoleId == 2) {
    for (int i = 0; i < ui->mTree1->rowCount(); i++) {
      QTableWidgetItem *item = ui->mTree1->item(i, 0);
      QString itemName = item->text();
      if (itemName == "温度6") item->setText("晶体温度1");
      else if (itemName == "温度7") item->setText("晶体温度2");
      else if (itemName == "温度8") item->setText("晶体温度3");
      else if (itemName == "温度9") item->setText("晶体温度4");
      else if (itemName == "温度10") item->setText("晶体温度5");
    }
  } else {
    for (int i = 0; i < ui->mTree1->rowCount(); i++) {
      QTableWidgetItem *item = ui->mTree1->item(i, 0);
      QString itemName = item->text();
      if (itemName == "晶体温度1") item->setText("温度6");
      else if (itemName == "晶体温度2") item->setText("温度7");
      else if (itemName == "晶体温度3") item->setText("温度8");
      else if (itemName == "晶体温度4") item->setText("温度9");
      else if (itemName == "晶体温度5") item->setText("温度10");
    }
  }

  return true;
}

void mwarnwdt::receiveData(QString, queryInfo) {}

void mwarnwdt::receiveQuery(QString c, queryInfo i, int a) {
  if (a < 0 || a > 1) return;
  if (isCh == false) return;
  updataGUI(c, i, a);
}

void mwarnwdt::clickCheck(bool checked) {
  isCh = false;
  QCheckBox *item = (QCheckBox *)sender();

  enableTree(false);
  item->blockSignals(true);
  int b = item->property("bits").toInt();
  item->setChecked(checked);
  sendCmd(b, checked);
  item->blockSignals(false);
  // std::this_thread::sleep_for(std::chrono::seconds(1));
  //  QTime dieTime = QTime::currentTime().addMSecs(1000);
  //  while (QTime::currentTime() < dieTime)
  //    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

  enableTree(true);
  isCh = true;
}

void mwarnwdt::updateItem(QTableWidget *pTable, QStringView title,
                          const uint32_t &start, const QVector<uint16_t> &val,
                          QStringView unit, QHash<uint8_t, _warn_item> &bak,
                          int treeIndex) {
  if (isInt1 == false) {
    QFont font;
    font.setPointSize(20);
    auto i = 0;
    for (auto it : val) {
      auto item = new QTableWidgetItem;
      item->setFlags(item->flags() & ~Qt::ItemIsEditable);
      item->setFont(font);
      item->setText(QString(tr("%1%2").arg(title).arg(i + 1)));
      pTable->setItem(i + start, 0, item);
      item->setTextAlignment(Qt::AlignCenter);
      item->setForeground(QBrush(QColor(98, 98, 98)));
      auto ischeck = new QCheckBox();
      ischeck->setFont(font);

      QObject::connect(ischeck, SIGNAL(clicked(bool)), this,
                       SLOT(clickCheck(bool)), Qt::DirectConnection);
      if (mBits[treeIndex + i])
        ischeck->setChecked(true);
      else
        ischeck->setChecked(false);

      ischeck->setProperty("bits", treeIndex + i);
      QWidget *widget = new QWidget;
      QHBoxLayout *layout = new QHBoxLayout;
      layout->setSpacing(0);
      layout->setMargin(0);
      layout->addWidget(ischeck);
      ischeck->setStyle(QStyleFactory::create("windowsxp"));
      layout->setAlignment(Qt::AlignCenter);
      widget->setLayout(layout);
      widget->setEnabled(false);
      pTable->setCellWidget(i + start, 1, widget);

      auto item1 = new QDoubleSpinBox;
      item1->setFont(font);
      double val = it * mInt;
      item1->setDisabled(true);
      item1->setSuffix(unit.toString());
      item1->setValue(val);
      item1->setButtonSymbols(QDoubleSpinBox::NoButtons);
      if (ischeck->isChecked()) {
        item1->setStyleSheet("color:black;"
                             "border-image:url(\":/img/edit2.png\");"
                             "qproperty-alignment: AlignCenter;");
      } else {
        item1->setStyleSheet("color:rgb(98, 98, 98);"
                             "border-image:url(\":/img/edit2.png\");"
                             "qproperty-alignment: AlignCenter;");
      }

      bak[i] = {item1, ischeck};
      pTable->setCellWidget(i + start, 2, item1);
      i++;
    }
    return;
  }

  int i = 0;
  //  int j = 0;
  for (auto it : val) {
    auto old = bak[i];
    if (mBits[treeIndex + i]) {
      if (!old.CItem->isChecked()) old.CItem->setChecked(true);
    } else {
      if (old.CItem->isChecked()) old.CItem->setChecked(false);
    }
    if (old.CItem->isChecked()) {
      old.VItem->setStyleSheet("color:black;"
                               "border-image:url(\":/img/edit2.png\");"
                               "qproperty-alignment: AlignCenter;");
    } else {
      old.VItem->setStyleSheet("color:rgb(98, 98, 98);"
                               "border-image:url(\":/img/edit2.png\");"
                               "qproperty-alignment: AlignCenter;");
    }
    double val = it * mInt;
    if (old.VItem->value() != val) {
      old.VItem->setValue(val);
      old.VItem->setProperty("userdata", val);
    }
    // j++;
    i++;
  }
}
double getPrecision(double value, int pre) {
  double offset = 0.5;
  if (pre < 1) return value;
  double prod = qPow(10.0, pre);
  if (value < 0) offset = -offset;
  int tmp = (int)(value * prod + offset);
  double ret = tmp / prod;
  return ret;
}
void mwarnwdt::updateItem(QTableWidget *pTable, QStringView title,
                          const uint32_t &start, const uint16_t &val,
                          QStringView unit, QHash<uint8_t, _warn_item> &bak,
                          int itemIndex, int hashIndex, bool &isInit) {
  if (isInit == false) {
    QFont font;
    font.setPointSize(20);
    auto i = itemIndex;
    auto item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setFont(font);
    item->setText(QString(tr("%1").arg(title)));
    pTable->setItem(i, 0, item);
    auto ischeck = new QCheckBox();
    ischeck->setFont(font);
    item->setTextAlignment(Qt::AlignCenter);
    item->setForeground(QBrush(QColor(98, 98, 98)));
    if (mBits[start])
      ischeck->setChecked(true);
    else
      ischeck->setChecked(false);
    QObject::connect(ischeck, SIGNAL(clicked(bool)), this,
                     SLOT(clickCheck(bool)));
    ischeck->setProperty("bits", start);
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(ischeck);
    ischeck->setStyle(QStyleFactory::create("windowsxp"));
    layout->setAlignment(Qt::AlignCenter);
    widget->setLayout(layout);
    widget->setEnabled(false);
    pTable->setCellWidget(i, 1, widget);

    auto item1 = new QDoubleSpinBox;
    item1->setFont(font);
    double v = val * mInt;
    if (ischeck->isChecked()) {
      item1->setStyleSheet("color:black;"
                           "border-image:url(\":/img/edit2.png\");"
                           "qproperty-alignment: AlignCenter;");
    } else {
      item1->setStyleSheet("color:rgb(98, 98, 98);"
                           "border-image:url(\":/img/edit2.png\");"
                           "qproperty-alignment: AlignCenter;");
    }
    item1->setDisabled(true);
    item1->setProperty("userdata", v);
    item1->setSuffix(unit.toString());
    item1->setButtonSymbols(QDoubleSpinBox::NoButtons);
    item1->setValue(v);
    item1->setProperty("bits", start);
    item1->setMaximum(10000);
    bak[hashIndex] = {item1, ischeck};
    pTable->setCellWidget(i, 2, item1);
    return;
  }
  int i = hashIndex;
  //需要判断
  auto old = bak[i];
  if (mBits[start]) {
    if (!old.CItem->isChecked()) old.CItem->setChecked(true);
  } else {
    if (old.CItem->isChecked()) old.CItem->setChecked(false);
  }

  double v = val * mInt;
  if (old.CItem->isChecked()) {
    old.VItem->setStyleSheet("color:black;"
                             "border-image:url(\":/img/edit2.png\");"
                             "qproperty-alignment: AlignCenter;");
  } else {
    old.VItem->setStyleSheet("color:rgb(98, 98, 98);"
                             "border-image:url(\":/img/edit2.png\");"
                             "qproperty-alignment: AlignCenter;");
  }
  if (old.VItem->value() != v) {
    old.VItem->setValue(v);
    old.VItem->setProperty("userdata", v);
  }
}

void mwarnwdt::queryData1(const queryInfo &info) {
  // 需要建立
  mInt = 0.1;
  uint8_t sempBits;
  if (isInt1 == false) {
    // int i = 0;
    mSemp.reserve(info.Amp_work.size());
    mJtSemp.reserve(info.JTWD_work.size());
    mInt = 0.1;
    ui->mTree1->setRowCount(info.Amp_work.size() + info.JTWD_work.size());

    ui->mTree2->setRowCount(6);
  }
  mInt = 0.1;
  updateItem(ui->mTree1, tr("semp"), 0, info.Amp_work, tr("℃"), mSemp, 0);
  updateItem(ui->mTree1, tr("JTsemp"), 5, info.JTWD_work, tr("℃"), mJtSemp, 5);
  mInt = 1;
  updateItem(ui->mTree2, tr("qt sd1"), 10, info.QTSD1, tr("%RH"), mQt, 0, 0,
             isInt1);
  updateItem(ui->mTree2, tr("qt sd2"), 11, info.QTSD2, tr("%RH"), mQt, 1, 1,
             isInt1);
  mInt = 1;
  updateItem(ui->mTree2, tr("qt wd1"), 12, info.QT_WD1, tr("℃"), mQt, 2, 2,
             isInt1);
  updateItem(ui->mTree2, tr("qt wd2"), 13, info.QT_WD2, tr("℃"), mQt, 3, 3,
             isInt1);
  mInt = 1;
  updateItem(ui->mTree2, tr("sl1"), 14, info.SLL, tr("L/min"), mSl, 4, 0,
             isInt1);
  if (isInt1 == false) {
//    ui->mTree1->setEnabled(false);
//    ui->mTree2->setEnabled(false);
    enableTree(false);
    isInt1 = true;
    return;
  }
}

void mwarnwdt::queryData2(const queryInfo &info) {
  mInt = 1;
  updateItem(ui->mTree2, tr("sl2"), 15, info.SL2, tr("L/min"), mSl, 5, 1,
             isInt2);

  if (isInt2 == false) {
    // updateItem1(ui->mTree2, tr("sl2"), 15, info.SLL, tr("L/min"), mSl, 5);
    isInt2 = true;
    return;
  }
  // updateItem1(ui->mTree2, tr("sl2"), 15, info.SLL, tr("L/min"), mSl, 5);
}

void mwarnwdt::enableTree(bool isEnable) {
//  ui->mTree1->setEnabled(isEnable);
//  ui->mTree2->setEnabled(isEnable);

  for (int i = 0; i < ui->mTree1->rowCount(); i++) {
    QWidget *widget = ui->mTree1->cellWidget(i, 1);
    if (widget != nullptr) {
      widget->setEnabled(isEnable);
    }
  }

  for (int i = 0; i < ui->mTree2->rowCount(); i++) {
    QWidget *widget = ui->mTree2->cellWidget(i, 1);
    if (widget != nullptr) {
      widget->setEnabled(isEnable);
    }
  }
}

void mwarnwdt::tableInit(QTableWidget *pTable) {
  QStringList strList;

  strList << tr("name") << tr("status") << tr("value");
  pTable->setColumnCount(strList.size());
  pTable->setHorizontalHeaderLabels(strList);

  pTable->horizontalHeader()->setSectionResizeMode(
    QHeaderView::Stretch);  //平均分配列宽
  // pTable->horizontalHeader()->setDefaultSectionSize(40);
  pTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

  pTable->verticalHeader()->setVisible(false);
  pTable->verticalHeader()->setDefaultSectionSize(45);

  // pTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}
void mwarnwdt::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
//递归删除节点
void mwarnwdt::removeItem(QTreeWidgetItem *item) {
  int count = item->childCount();
  if (count == 0) { //没有子节点，直接删除
    // delete item;
    return;
  }

  for (int i = 0; i < count; i++) {
    QTreeWidgetItem *childItem = item->child(0);  //删除子节点
    removeItem(childItem);
    mItems.remove(childItem);
    delete childItem;  //删除子节点
  }
  // delete item;  //最后将自己删除
}
