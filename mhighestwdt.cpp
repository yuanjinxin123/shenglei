#include "mhighestwdt.h"

#include <qlineedit.h>

#include "log.h"
#include "mportmanager.h"
#include "order.h"
#include "ui_mhighestwdt.h"
mHighestWdt::mHighestWdt(QWidget *parent)
  : mWidget(parent), ui(new Ui::mHighestWdt) {
  ui->setupUi(this);
  init();
  setProperty("icon", ":/img/gj.png");
  setAttribute(Qt::WA_TranslucentBackground, true);
}

mHighestWdt::~mHighestWdt() { delete ui; }

QTabWidget *mHighestWdt::getTableWidget() const {
  return ui->mHighTabWdt;
}

bool mHighestWdt::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress ||
      event->type() == QEvent::Wheel || event->type() == QEvent::KeyPress) {
    auto r = dynamic_cast<QWidget *>(obj);
    if (nullptr == dynamic_cast<QWidget *>(obj)) {
      return false;
    }

    if (obj->objectName() == "mFreqMax") {
      mMaxFreqChange = true;
      if (event->type() == QEvent::KeyPress) {
        mMaxFrepPress = true;
      }
    }
    if (obj->objectName() == "mFreqMin") {
      mMinFreqChange = true;
      if (event->type() == QEvent::KeyPress) {
        mMinFreqPress = true;
      }
    }
  }
  return false;
}

void mHighestWdt::on_mBurstBtn_clicked() {}

void mHighestWdt::on_mFreqBtn_clicked() {}

void mHighestWdt::on_mFreqValueMax(int v) {
  if (mMaxFreqChange == false || mMinFreqPress == true) return;
  QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
  QDataStream packet(&s, QIODevice::ReadWrite);

  packet.setByteOrder(QDataStream::BigEndian);
  packet << (uint16_t)v;
  if (mportMg->send(F_max_set_out, s)) {
    return;
  }
}

void mHighestWdt::on_mFreqValueMin(int v) {
  if (mMinFreqChange == false || mMinFreqPress == true) return;
  QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
  QDataStream packet(&s, QIODevice::ReadWrite);

  packet.setByteOrder(QDataStream::BigEndian);
  packet << (uint16_t)v;
  if (mportMg->send(F_min_set_out, s)) {
    return;
  }
}

void mHighestWdt::on_comboBox_currentTextChanged(const QString &text) {}

void mHighestWdt::on_mBurstMax_activated(int index) {
  if (index < ui->mBurstMin->currentIndex()) {
    ui->mBurstMax->setCurrentIndex(ui->mBurstMin->currentIndex());
  }
  uint16_t val;
  val = ui->mBurstMax->currentIndex() + 1;

  QByteArray s;
  QDataStream packet(&s, QIODevice::ReadWrite);

  packet.setByteOrder(QDataStream::BigEndian);
  packet << val;
  if (mportMg->send(BURST_max_set_out, s)) {
    // ui->mBurstMax->setCurrentIndex(ui->mBurstMax->currentData().toUInt());
  }
}

void mHighestWdt::on_mBurstMin_activated(int index) {
  if (index > ui->mBurstMax->currentIndex()) {
    ui->mBurstMin->setCurrentIndex(ui->mBurstMax->currentIndex());
  }
  uint16_t val;
  val = ui->mBurstMin->currentIndex() + 1;
  QByteArray s;
  QDataStream packet(&s, QIODevice::ReadWrite);

  packet.setByteOrder(QDataStream::BigEndian);
  packet << val;
  if (mportMg->send(BURST_min_set_out, s)) {
    // ui->mBurstMin->setCurrentIndex(ui->mBurstMin->currentData().toUInt());
  }
}

void mHighestWdt::receiveQuery(QString name, queryInfo info, int a) {
  updataparam(name, info, a);
}
void mHighestWdt::init() {
  ui->mFreqVer->setItemData(0, 0x001e, Qt::UserRole);
  ui->mFreqVer->setItemData(1, 0x001f, Qt::UserRole);

  ui->mFreqMax->installEventFilter(this);
  ui->mFreqMin->installEventFilter(this);
  QObjectList o_list = ui->mFreqMax->children();
  for (int i = 0; i < o_list.length(); i++) {
    // QLineEdit *cast = qobject_cast<QLineEdit *>(o_list[i]);
    if (o_list[i]) o_list[i]->installEventFilter(this);
  }
  o_list = ui->mFreqMin->children();
  for (int i = 0; i < o_list.length(); i++) {
    // QLineEdit *cast = qobject_cast<QLineEdit *>(o_list[i]);
    if (o_list[i]) o_list[i]->installEventFilter(this);
  }

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  QObject::connect(this, SIGNAL(changerVer(uint8_t)), mportManager::instance(),
                   SIGNAL(changePsoOrPod(uint8_t)));

  mFreqMin = ui->mFreqMin->value();
  mFreqMax = ui->mFreqMax->value();
  // connect(ui->mFreqMax, &QSpinBox::valueChanged, this, [](auto i) {});
  QObject::connect(ui->mFreqMax, SIGNAL(valueChanged(int)), this,
                   SLOT(on_mFreqValueMax(int)));
  // QObject::connect(ui->mFreqMax, &QSpinBox::editingFinished,
  //                 [this]() { mMaxFreqChange = false; });
  QObject::connect(ui->mFreqMin, SIGNAL(valueChanged(int)), this,
                   SLOT(on_mFreqValueMin(int)));
  // QObject::connect(ui->mFreqMin, &QSpinBox::editingFinished,
  //                 [this]() { mMinFreqChange = false; });
  auto id = ui->mFreqVer->currentIndex();

  mportMg->saveFreqVer(ui->mFreqVer->currentIndex());
  emit changerVer(id);
}
void mHighestWdt::updataparam(QString name, queryInfo info, int a) {
  if (a != 0) return;

  if (info.Burst_max_set_out - 1 != ui->mBurstMax->currentData().toUInt()) {
    ui->mBurstMax->setCurrentIndex(info.Burst_max_set_out - 1);
    ui->mBurstMax->setItemData(info.Burst_max_set_out - 1,
                               info.Burst_max_set_out - 1);
  }
  if (info.Burst_min_set_out - 1 != ui->mBurstMin->currentData().toUInt()) {
    ui->mBurstMin->setCurrentIndex(info.Burst_min_set_out - 1);
    ui->mBurstMin->setItemData(info.Burst_min_set_out - 1,
                               info.Burst_min_set_out - 1);
  }

  mFreqMax = info.F_max_set_out;
  if (ui->mFreqMax->value() != mFreqMax && mMaxFreqChange == false) {
    ui->mFreqMax->setValue(mFreqMax);
  }
  mFreqMin = info.F_min_set_out;
  if (ui->mFreqMin->value() != mFreqMin && mMinFreqChange == false)
    ui->mFreqMin->setValue(mFreqMin);
  auto mFreqVer = info.PSO_POD_version;
  if (mFreqVer == 30 || mFreqVer == 31) {
    if (mFreqVer != ui->mFreqVer->currentData(Qt::UserRole)) {
      auto curIndex = ui->mFreqVer->currentIndex();
      curIndex++;
      curIndex %= 2;
      ui->mFreqVer->setCurrentIndex(curIndex);
    }
  }
  // 电流
  if (info.DL_JX.size() >= 5) {
    double f = 0.01;
    ui->mDl1Val->setValue(info.DL_JX[0] * f);
    if (mInit) ui->mDl1Change->setValue(info.DL_JX[0] * f);

    ui->mDl2Val->setValue(info.DL_JX[1] * f);
    if (mInit) ui->mDl2Change->setValue(info.DL_JX[1] * f);

    ui->mDl3Val->setValue(info.DL_JX[2] * f);
    if (mInit) ui->mDl3Change->setValue(info.DL_JX[2] * f);

    ui->mDlVal4->setValue(info.DL_JX[3] * f);
    if (mInit) ui->mDl4Change->setValue(info.DL_JX[3] * f);

    ui->mDl5Val->setValue(info.DL_JX[4] * f);
    if (mInit) ui->mDl5Change->setValue(info.DL_JX[4] * f);
  }
  mInit = false;
}

void mHighestWdt::on_mFreqResetBtn_clicked() {
  mMinFreqChange = false;
  mMaxFreqChange = false;
  mportMg->refresh();
}

void mHighestWdt::on_mFreqSetBtn_clicked() {
  uint16_t v = ui->mFreqMax->value();
  if (v != mFreqMax) {
    QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
    QDataStream packet(&s, QIODevice::ReadWrite);

    packet.setByteOrder(QDataStream::BigEndian);
    packet << v;
    if (mportMg->send(F_max_set_out, s)) {
      return;
    }
  }
  {
    v = ui->mFreqMin->value();
    if (v != mFreqMin) {
      QByteArray s;  // = QByteArray::fromRawData((char *)&v, 2);
      QDataStream packet(&s, QIODevice::ReadWrite);

      packet.setByteOrder(QDataStream::BigEndian);
      packet << v;
      if (mportMg->send(F_min_set_out, s)) {
      }
    }
  }
}

void mHighestWdt::on_mFreqVer_activated(int index) {
  QByteArray s;
  QDataStream packet(&s, QIODevice::ReadWrite);
  uint16_t v = ui->mFreqVer->currentData(Qt::UserRole).toUInt();

  packet << v;
  mportMg->send(POD_OR_PSO, s);
  on_mDlResetBtn_clicked();
}

void mHighestWdt::on_mFreqVer_currentIndexChanged(int index) {
  mportMg->saveFreqVer(index);

  emit changerVer(index);
}

void mHighestWdt::on_mDlSetBtn_clicked() {
  sender()->blockSignals(true);
  if (ui->mDl1Change->value() != ui->mDl1Val->value()) {
    QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
    QDataStream packet(&s, QIODevice::ReadWrite);

    packet.setByteOrder(QDataStream::BigEndian);
    double d = ui->mDl1Change->value();
    d = d * 100;

    uint16_t v = qRound(d);

    packet << v;
    mportMg->send(DL_JX1, s);
  }
  if (ui->mDl2Change->value() != ui->mDl2Val->value()) {
    QByteArray s;
    QDataStream packet(&s, QIODevice::ReadWrite);

    packet.setByteOrder(QDataStream::BigEndian);
    double d = ui->mDl2Change->value();
    d = d * 100;

    uint16_t v = qRound(d);

    packet << v;
    mportMg->send(DL_JX2, s);
  }

  if (ui->mDl3Change->value() != ui->mDl3Val->value()) {
    QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
    QDataStream packet(&s, QIODevice::ReadWrite);

    packet.setByteOrder(QDataStream::BigEndian);
    double d = ui->mDl3Change->value();
    d = d * 100;

    uint16_t v = qRound(d);
    packet << v;
    mportMg->send(DL_JX3, s);
  }

  if (ui->mDl4Change->value() != ui->mDlVal4->value()) {
    QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
    QDataStream packet(&s, QIODevice::ReadWrite);

    packet.setByteOrder(QDataStream::BigEndian);
    double d = ui->mDl4Change->value();
    d = d * 100;

    uint16_t v = qRound(d);
    packet << v;

    mportMg->send(DL_JX4, s);
  }

  if (ui->mDl5Change->value() != ui->mDl5Val->value()) {
    QByteArray s;  //= QByteArray::fromRawData((char *)&v, 2);
    QDataStream packet(&s, QIODevice::ReadWrite);

    packet.setByteOrder(QDataStream::BigEndian);
    double d = ui->mDl5Change->value();
    d = d * 100;

    uint16_t v = qRound(d);
    packet << v;
    mportMg->send(DL_JX5, s);
  }
  sender()->blockSignals(false);
}

void mHighestWdt::on_mDlResetBtn_clicked() {
  mportMg->refresh();

  mInit = true;
}

void mHighestWdt::on_mFreqMin_textChanged(const QString &arg1) {
  QString v = arg1;
}

void mHighestWdt::on_mFreqMin_valueChanged(int v) {}

void mHighestWdt::on_mFreqMax_valueChanged(int v) {}

void mHighestWdt::on_mFreqMin_editingFinished() {
  if (mMinFreqChange) {
    uint16_t v = ui->mFreqMin->value();
    QByteArray s;
    QDataStream packet(&s, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << v;
    if (mportMg->send(F_min_set_out, s)) {
      mMinFreqPress = false;
      mMinFreqChange = false;
      return;
    }
  }
  mMinFreqPress = false;
  mMinFreqChange = false;
}

void mHighestWdt::on_mFreqMax_editingFinished() {
  if (mMaxFreqChange) {
    uint16_t v = ui->mFreqMax->value();
    QByteArray s;
    QDataStream packet(&s, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << v;
    if (mportMg->send(F_max_set_out, s)) {
      mMaxFreqChange = false;
      mMaxFrepPress = false;
      return;
    }
  }
  mMaxFreqChange = false;
  mMaxFrepPress = false;
}
