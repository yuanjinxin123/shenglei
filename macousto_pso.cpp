#include "macousto_pso.h"

#include "log.h"
#include "mCorrectionDlg.h"
#include "ui_macousto_pso.h"
mAcousto_pso::mAcousto_pso(QWidget *parent)
  : QWidget(parent), ui(new Ui::mAcousto_pso) {
  ui->setupUi(this);
  //setAutoFillBackground(true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setPowerMode(true);
  init();
}

mAcousto_pso::~mAcousto_pso() { delete ui; }
bool mAcousto_pso::mode() const { return mISMode; }

void mAcousto_pso::setMode(uint m) {
  mISMode = m;
  uint8_t v = m;

  // if (m != false) v = 0x2;
  if (m == 0) m = 1;
  QByteArray val;
  val.push_back(v);
  mportMg->send(MODE_SWITCH, val);

  emit modeChanged(m);
}

bool mAcousto_pso::podGate() const { return mIsPodGate; }

void mAcousto_pso::setPodGate(bool m) {
  mIsPodGate = m;
  uint8_t v = 0x2b;
  // QLOG_DEBUG() << "id =" << m;
  if (m != false) v = 0x2a;
  QByteArray val;
  val.push_back(v);
  mportMg->send(POD_OR_GATE, val);
  emit podGateChanged(m);
}

bool mAcousto_pso::hasDebug() const { return mIsDebug; }

void mAcousto_pso::setDebug(bool m) {
  mIsDebug = m;
  emit debugChanged(m);
}

bool mAcousto_pso::hasDebug_2() const { return mIsDebug_2; }

void mAcousto_pso::setDebug_2(bool m) {
  mIsDebug_2 = m;
  emit debug2Changed(m);
}

bool mAcousto_pso::hasPowerMode() const { return mIsPowerMode; }

void mAcousto_pso::setPowerMode(bool m) {
  mIsPowerMode = m;
  emit powerModeChanged(m);
}
bool mAcousto_pso::init() {
  pModeGroup = new QButtonGroup(this);
  pPowerCtlGroup = new QButtonGroup(this);
  pDebugGroup = new QButtonGroup(this);
  pDebugGroup1 = new QButtonGroup(this);

  pModeGroup->addButton(ui->mRMode1, 1);
  pModeGroup->addButton(ui->mRMode2, 2);

  pPowerCtlGroup->addButton(ui->mPowerIntCtl, 0);
  pPowerCtlGroup->addButton(ui->mPowerExtCtl, 1);

  pDebugGroup->addButton(ui->mROpen, 1);
  pDebugGroup->addButton(ui->mRClose, 0);

  pDebugGroup1->addButton(ui->mROpen1, 1);
  pDebugGroup1->addButton(ui->mRClose1, 0);
  /*
  connect(this, &mAcousto_pso::debugChanged, [&](bool b) { updateDebug(b); });
  connect(this, &mAcousto_pso::debug2Changed, [&](bool b) { updateDebug_2(b); });
  connect(this, &mAcousto_pso::powerModeChanged,
  [&](bool b) { updatePowerMode(b); });
  */
  connect(pModeGroup, SIGNAL(buttonClicked(int)), this,
          SLOT(modeButtonsClicked(int)));

  connect(pPowerCtlGroup, SIGNAL(buttonClicked(int)), this, SLOT(updatePowerMode(int)));
  connect(pDebugGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateDebugMode(int)));
  connect(pDebugGroup1, SIGNAL(buttonClicked(int)), this, SLOT(updateDebug1Mode(int)));

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  return true;
}
void mAcousto_pso::receiveQuery(QString name, queryInfo info, int a) {
  if (mUpdate == false) return;
  updataHome(name, info, a);
}

bool mAcousto_pso::updataHome(QStringView name, const queryInfo &info, int a) {

  if (a == 0) {
    ui->mFreqCur->setValue(info.Freq_set);
    if (mIsInitVal) ui->mFreqSetVal->setValue(info.Freq_set);
    ui->mBurstVal->setValue(info.Burst_num);
    if (mIsInitVal) ui->mBurstSetVal->setValue(info.Burst_num);
    ui->mPowerCurSet->setValue(info.Power_INOUT_BF);
    if (mIsInitVal) ui->mPowerSetVal->setValue(info.Power_INOUT_BF);

    // QLOG_DEBUG() << "mode" << info.mode_status;
    pModeGroup->button(info.mode)->setChecked(true);


    if (pDebugGroup->id(pDebugGroup->checkedButton()) != info.Debug)
      pDebugGroup->button(info.Debug)->setChecked(true);


    if (pPowerCtlGroup->id(pPowerCtlGroup->checkedButton()) != info.Power_INOUT_status)
      pPowerCtlGroup->button(info.Power_INOUT_status)->setChecked(true);
    mIsInitVal = false;

    bool visible = false;
    if (info.QDNC_QDC == 1) {
      visible = true;
    }
    if (ui->label_9->isVisible() != visible) {
      ui->label_9->setVisible(visible);
      ui->mPowerCurSet_2->setVisible(visible);
      ui->mPowerSetVal_2->setVisible(visible);
      ui->label_11->setVisible(visible);
      ui->mGateFreCurSet_2->setVisible(visible);
      ui->mGateFreSetVal_2->setVisible(visible);
      ui->label_8->setVisible(visible);
    }

    return true;
  } else if (a == 1) {
    ui->mPowerCurSet_2->setValue(info.Power_INOUT_BF_2);
    if (mIsInitVal_2) ui->mPowerSetVal_2->setValue(info.Power_INOUT_BF_2);
    ui->mGateFreCurSet->setValue(info.GateFre1Set);
    if (mIsInitVal_2) ui->mGateFreSetVal->setValue(info.GateFre1Set);
    ui->mGateFreCurSet_2->setValue(info.GateFre2Set);
    if (mIsInitVal_2) ui->mGateFreSetVal_2->setValue(info.GateFre2Set);

    if (pDebugGroup1->id(pDebugGroup1->checkedButton()) != info.Debug_2)
      pDebugGroup1->button(info.Debug_2)->setChecked(true);
    mIsInitVal_2 = false;
    return true;
  }

  return false;
}

void mAcousto_pso ::modeButtonsClicked(int id) { setMode(id); }
void mAcousto_pso::updatePowerMode(int id) {
  QByteArray val;
  uint8_t v = (uint8_t)id;
  mUpdate = false;
  val.push_back(v);
  mportMg->send(GL_SWITCH, val);
  mUpdate = true;
}

void mAcousto_pso::updateDebugMode(int id) {
  QByteArray val;
  uint8_t v = (uint8_t)id;
  mUpdate = false;
  val.push_back(v);
  mportMg->send(DEBUG_SWITCH, val);
  mUpdate = true;
}
void mAcousto_pso::updateDebug1Mode(int id) {
  QByteArray val;
  uint8_t v = (uint8_t)id;
  mUpdate = false;
  val.push_back(v);

  mportMg->send(DEBUG_SWITCH_2, val);
  mUpdate = true;
}

void mAcousto_pso::on_mSetBtn_clicked() {
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

void mAcousto_pso::on_mFreqSetVal_valueChanged(int n) {
  uint16_t f = n;
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << f;

  mportMg->send(FREQ, val);
}

void mAcousto_pso::on_mBurstSetVal_valueChanged(int n) {
  uint16_t b = n;
  if (b != ui->mBurstVal->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << b;

    mportMg->send(BURST_NUM, val);
  }
}

void mAcousto_pso::on_mPowerSetVal_valueChanged(int n) {
  uint16_t p = n;
  if (p != ui->mPowerCurSet->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << p;

    mportMg->send(GL_NK, val);
  }
}

void mAcousto_pso::on_mPowerSetVal_2_valueChanged(int n) {
  uint16_t p = n;
  if (p != ui->mPowerCurSet_2->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << p;

    mportMg->send(GL_NK_2, val);
  }
}

void mAcousto_pso::on_mGateFreSetVal_valueChanged(int n) {
  uint16_t p = n;
  if (p != ui->mGateFreCurSet->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << p;

    mportMg->send(GATE_FRE, val);
  }
}

void mAcousto_pso::on_mGateFreSetVal_2_valueChanged(int n) {
  uint16_t p = n;
  if (p != ui->mGateFreCurSet_2->value()) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << p;

    mportMg->send(GATE_FRE_2, val);
  }
}

void mAcousto_pso::on_mCorre_clicked() {
  mCorrectionDlg correDlg;
  // correDlg.setWindowModality(Qt::WindowModal);
  // correDlg.setModal(true);
  // correDlg.setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
  correDlg.show();
  QEventLoop loop;
  connect(&correDlg, SIGNAL(dlg_exit()), &loop, SLOT(quit()));
  loop.exec(QEventLoop::ExcludeSocketNotifiers);
}
