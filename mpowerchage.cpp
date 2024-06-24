#include "mpowerchage.h"

#include <QDoubleSpinBox>

#include "log.h"
#include "ui_mpowerchage.h"
mPowerChage::mPowerChage(QWidget *parent)
  : QWidget(parent), ui(new Ui::mPowerChage) {
  ui->setupUi(this);
  init();
}

mPowerChage::~mPowerChage() { delete ui; }

// bool mPowerChage::eventFilter(QObject *obj, QEvent *event) {
//  if (event->type() == QEvent::MouseButtonPress ||
//      event->type() == QEvent::Wheel||
//      event->type() == QEvent::KeyPress) {
//    auto r = dynamic_cast<QWidget *>(obj);
//    if (nullptr == dynamic_cast<QWidget *>(obj)) {
//      return false;
//    }
//    if(obj->objectName() == "mPowerVal"){
//        mIsChange = true;
//    }
//    auto p = dynamic_cast<QWidget *>(obj)->parent();
//    if (dynamic_cast<QDoubleSpinBox *>(
//            dynamic_cast<QWidget *>(obj)->parent())) {
//      mIsChange = true;
//    }
//  }
//  return false;
//}

void mPowerChage::receiveQuery(QString name, queryInfo info, int a) {
  updataHome(name, info, a);
}
bool mPowerChage::updataHome(QStringView name, const queryInfo &info, int a) {
  double v;
  if (a == 0) {
    v = (double)info.Freq_DA / 1000;

    if (ui->mPowerVal->getHasFouce() == false) ui->mPowerVal->setValue(v);

    auto fVal = ui->mFreqVal->value();
    if (fVal != info.freq_zb && ui->mCurePower->getHasFouce() == false)
      ui->mFreqVal->setValue(info.freq_zb);
    bool s = info.Freq_DA_Status;
    if (ui->mPowerBtn->isChecked() != s) {
      ui->mPowerBtn->setChecked(s);
      // ui->mPowerBtn->setCheckState(Qt::Checked);
    }
    s = info.MNSZ_status;
    if (ui->mMnOrSzBtn->isChecked() != s) {
      ui->mMnOrSzBtn->setChecked(s);
    }
    return true;
  }
  if (ui->mBurstVal->getHasFouce() == false)
    ui->mBurstVal->setValue(info.CorrenctBurst);

  v = (double)info.PowerCorrect / 100;
  if (ui->mCurePower->getHasFouce() == false) ui->mCurePower->setValue(v);

  return true;
}
bool mPowerChage::init() {
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(mportManager::instance(), &mportManager::curePowerEnable,
  [&](bool isEnable) {
    ui->mBurstVal->setEnabled(isEnable);
    ui->mCurePower->setEnabled(isEnable);
    ui->mFreqVal->setEnabled(isEnable);
  });

  return true;
}

void mPowerChage::on_mTable_cellActivated(int row, int column) {
  QLOG_DEBUG() << "test" << row << column;
}

void mPowerChage::on_mPowerVal_editingFinished() {
  if (mIsChange) {
    mIsChange = false;
  }
}

void mPowerChage::on_mMnOrSzBtn_clicked(bool checked) {
  // bool s = ui->mMnOrSz->isChecked();
  QByteArray val;
  // s = !s;

  val.push_back(checked);
  mportMg->send(BFB_SWITCH, val);
}

void mPowerChage::on_mPowerBtn_clicked(bool checked) {
  QByteArray val;

  val.push_back(checked);
  mportMg->send(DA_SWITCH, val);
}

void mPowerChage::on_mPowerVal_valueChanged(double val) {
  if (ui->mPowerVal->getHasFouce() == false) return;
  val = val * 1000;
  QByteArray mBuffer;
  uint16_t valInt = qRound(val);
  QDataStream packet(&mBuffer, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << valInt;
  mportMg->send(DA_SET, mBuffer);
}

void mPowerChage::on_mCurePower_valueChanged(double val) {
  if (ui->mCurePower->getHasFouce() == false) return;
  val = val * 100;
  QByteArray mBuffer;
  uint16_t valInt = qRound(val);
  QDataStream packet(&mBuffer, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << valInt;
  mportMg->send(POWER_CUR, mBuffer);
}

void mPowerChage::on_mBurstVal_valueChanged(int val) {
  if (ui->mBurstVal->getHasFouce() == false) return;
  uint16_t v = val;
  QByteArray mBuffer;
  QDataStream packet(&mBuffer, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << v;
  mportMg->send(CUR_BURST, mBuffer);
}

void mPowerChage::on_mFreqVal_valueChanged(int val) {
  if (ui->mFreqVal->getHasFouce() == false) return;
  uint16_t v = val;
  QByteArray mBuffer;
  QDataStream packet(&mBuffer, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << v;
  mportMg->send(FREQ_Z, mBuffer);
}
