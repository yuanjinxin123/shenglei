#include "macousto_pod.h"

#include "log.h"
#include "mcorrectiondlg.h"
#include "mlogindlg.h"
#include "ui_macousto_pod.h"
mAcousto_pod::mAcousto_pod(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::mAcousto_pod),
    mIsDebug(true),
    mIsPowerMode(true) {
  ui->setupUi(this);

  //setAutoFillBackground(true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setPowerMode(true);
  ui->mPowerModeBtn->setProperty("mIsPowerMode", mIsPowerMode);
  ui->mPowerModeBtn->setStyle(QApplication::style());
  init();
  ui->mPowerModeSwt->setVisible(false);
}

mAcousto_pod::~mAcousto_pod() { delete ui; }

bool mAcousto_pod::mode() const { return mISMode; }

void mAcousto_pod::setMode(uint m) {
  mISMode = m;
  uint8_t v = m;

  // if (m != false) v = 0x2;
  if (m == 0) m = 1;

  QByteArray val;
  val.push_back(v);

  mportMg->send(MODE_SWITCH, val);

  emit modeChanged(m);
}

bool mAcousto_pod::podGate() const { return mIsPodGate; }

void mAcousto_pod::setPodGate(bool m) {
  sender()->blockSignals(true);
  mIsPodGate = m;
  uint8_t v = 0x00;
  mUpdate = false;
  if (m != false) v = 0x01;
  QByteArray val;
  val.push_back(v);

  mportMg->send(POD_OR_GATE, val);

  emit podGateChanged(m);
  mUpdate = true;
  sender()->blockSignals(false);
}

bool mAcousto_pod::hasDebug() const { return mIsDebug; }

void mAcousto_pod::setDebug(bool m) {
  mIsDebug = m & 0x01;
  emit debugChanged(m);
}

bool mAcousto_pod::hasPowerMode() const { return mIsPowerMode; }

void mAcousto_pod::setPowerMode(bool m) {
  mIsPowerMode = m;
  emit powerModeChanged(m);
}

void mAcousto_pod::on_mDebunBtn_clicked() {}

void mAcousto_pod::on_mPowerModeBtn_clicked() {
  mUpdate = false;
  sender()->blockSignals(true);
  mIsPowerMode = !mIsPowerMode;
  if (mIsPowerMode)
    ui->mPowerModeSwt->setText(tr("out ctl"));
  else
    ui->mPowerModeSwt->setText(tr("in ctl"));

  setPowerMode(mIsPowerMode);
  ui->mPowerModeBtn->setProperty("mIsPowerMode", mIsPowerMode);
  ui->mPowerModeBtn->setStyle(QApplication::style());
  QByteArray v;
  v.push_back(mIsPowerMode);
  mportMg->send(GL_SWITCH, v);
  sender()->blockSignals(false);
  mUpdate = true;
}

void mAcousto_pod::on_mSetBtn_clicked() {
  uint16_t f, b, p;

  f = ui->mFreqSetVal->value();
  if (f != ui->mFreqCur->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << f;

    mportMg->send(FREQ, val);
  }
  b = ui->mBurstSetVal->value();
  if (b != ui->mBurstVal->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << b;

    mportMg->send(BURST_NUM, val);
  }
  p = ui->mPowerSetVal->value();
  if (p != ui->mPowerCurSet->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << p;

    mportMg->send(GL_NK, val);
  }
}
bool mAcousto_pod::init() {
  pModeGroup = new QButtonGroup(this);
  pPodGateGroup = new QButtonGroup(this);
  pCfGroup = new QButtonGroup(this);
  pModeGroup->addButton(ui->mRMode1, 1);
  pModeGroup->addButton(ui->mRMode2, 2);

  pPodGateGroup->addButton(ui->mRPODMode, 0);
  pPodGateGroup->addButton(ui->mRGateMode, 1);

  pCfGroup->addButton(ui->mNcf, 0);
  pCfGroup->addButton(ui->mWcf1, 1);
  pCfGroup->addButton(ui->mWcf2, 2);

  connect(pPodGateGroup, SIGNAL(buttonClicked(int)), this,
          SLOT(mAcousto_pod::setPodGate(int)));

  // connect(this, &mAcousto_pod::debugChanged, [&](bool b) { updateDebug(b);
  // });
  connect(pCfGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateCFMode(int)));

  connect(pModeGroup, SIGNAL(buttonClicked(int)), this,
          SLOT(modeButtonsClicked(int)));
  connect(this, &mAcousto_pod::powerModeChanged,
  [&](bool b) { updatePowerMode(b); });
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  return true;
}
void mAcousto_pod::receiveQuery(QString name, queryInfo info, int a) {
  if (mUpdate == false) return;
  updataHome(name, info, a);
}
void mAcousto_pod ::modeButtonsClicked(int id) {
  // QLOG_DEBUG() << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++";
  setMode(id);
}
bool mAcousto_pod::updataHome(QStringView name, const queryInfo &info, int a) {
  if (a != 0) return false;

  if (info.Freq_set != ui->mFreqCur->value()) {
    ui->mFreqCur->setValue(info.Freq_set);
    // ui->mFreqSetVal->setValue(info.Freq_set);
  }
  if (ui->mBurstVal->value() != info.Burst_num) {
    ui->mBurstVal->setValue(info.Burst_num);
    // ui->mBurstSetVal->setValue(info.Burst_num);
  }
  if (ui->mPowerCurSet->value() != info.Power_INOUT_BF) {
    ui->mPowerCurSet->setValue(info.Power_INOUT_BF);
    // ui->mPowerSetVal->setValue(info.Power_INOUT_BF);
  }
  if (mInitVal) {
    mInitVal = false;
    ui->mFreqSetVal->setValue(info.Freq_set);
    ui->mBurstSetVal->setValue(info.Burst_num);
    ui->mPowerSetVal->setValue(info.Power_INOUT_BF);
  }
  auto mode = info.mode;
  if (info.mode == 0 || info.mode > 2) {
    mode = 1;
  }
  if (pModeGroup->id(pModeGroup->checkedButton()) != info.mode)
    pModeGroup->button(mode)->setChecked(true);
  // auto p = pPodGateGroup->checkedButton();
  if (pPodGateGroup->id(pPodGateGroup->checkedButton()) != info.POD_GATE)
    pPodGateGroup->button(info.POD_GATE)->setChecked(true);
  // QLOG_DEBUG() << "cf" << info.tri_state_in;
  if (pCfGroup->id(pCfGroup->checkedButton()) != (info.tri_state_in & 0x03)) {
    pCfGroup->button(info.tri_state_in % 0x03)->setChecked(true);
  }

  setPowerMode(info.Power_INOUT_status);

  return true;
}

void mAcousto_pod::updateDebug(const bool &b) {}

void mAcousto_pod::updateCFMode(int b) {
  QByteArray val;
  uint8_t v = (uint8_t)b;
  mUpdate = false;
  val.push_back(v);

  QLOG_DEBUG() << "id =" << b;
  mportMg->send(CF_MODE, val);
  mUpdate = true;
}
void mAcousto_pod::on_finish() { mUpdate = true; }
void mAcousto_pod::updatePowerMode(const bool &b) {
  if (b)
    ui->mPowerModeSwt->setText(tr("out ctl"));
  else
    ui->mPowerModeSwt->setText(tr("in ctl"));
  ui->mPowerModeBtn->setProperty("mIsPowerMode", b);
  ui->mPowerModeBtn->setStyle(QApplication::style());
}
void mAcousto_pod::on_mFreqSetVal_valueChanged(int v) {
  uint16_t f = v;

  if (f != ui->mFreqCur->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << f;
    mportMg->send(FREQ, val);
  }
}

void mAcousto_pod::on_mBurstSetVal_valueChanged(int v) {
  uint16_t b = v;
  if (b != ui->mBurstVal->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << b;
    mportMg->send(BURST_NUM, val);
  }
}

void mAcousto_pod::on_mPowerSetVal_valueChanged(int v) {
  uint16_t p = v;
  if (p != ui->mPowerCurSet->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << p;

    mportMg->send(GL_NK, val);
  }
}

void mAcousto_pod::on_mCorre_clicked() {
  mCorrectionDlg correDlg;
  // correDlg.setWindowModality(Qt::WindowModal);
  // correDlg.setModal(true);
  // correDlg.setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
  correDlg.show();
  QEventLoop loop;
  connect(&correDlg, SIGNAL(dlg_exit()), &loop, SLOT(quit()));
  loop.exec(QEventLoop::ExcludeSocketNotifiers);
}
