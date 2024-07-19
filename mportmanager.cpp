#include "mportmanager.h"

#include <qapplication.h>
#include <qfuture.h>
#include <qserialportinfo.h>

#include <QDataStream>
#include <QDebug>
#include <QEventLoop>
#include <QMessageBox>
#include <QTime>
#include <chrono>
#include <thread>
#include <QDateTime>
#include "config.h"
#include "defer.h"
#include "log.h"
#include "mserial.h"
#include "tcpclient.h"
#include "order.h"
#include "portcfg.h"
#include "sql.h"
#include <QRadioButton>
Q_GLOBAL_STATIC(mportManager, g_portMg);
const uint8_t g_head[3] = {0x7E, 0xE7, 0x7E};
QMutex mSendLock;
mportManager::mportManager(QObject *parent)
  : QObject{parent}, mCacelTry(false) {
  qRegisterMetaType<queryInfo>("queryInfo");
  qRegisterMetaType<cmdData>("cmdData");
  connect(this, &mportManager::sendCmdToPort, this,
          &mportManager::sendDataToSerial, Qt::DirectConnection);
  mTimer = new QTimer();

  mTimer->setInterval(Config::getIns()->Get(config_com_time1).toInt());
  // connect(mTimerThd, SIGNAL(started()), mTimer, SLOT(start()));
  connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  connect(this, &mportManager::sendCmd, this, &mportManager::saveCmdData,
          Qt::QueuedConnection);
  connect(&m_SNManage, SIGNAL(sendRemoveSN(QString)), this, SLOT(onRemoveSn(QString)));
  // connect(mTimerThd, SIGNAL(finished()), mTimerThd, SLOT(deleteLater()));
  // mTimerThd->start();
  mTimer->stop();
  // mTimer->setTimerType(Qt::PreciseTimer);//如果不设置，默认为CoarseTimer类型
}

int mportManager::sendQuery() {
  if (mSendPort == nullptr) return -1;
  QByteArray data;
  emit sendCmdToPort(QUERY1, data, false, false, false);
  qApp->processEvents();
  QTime slp;
  slp.start();
  while (slp.elapsed() < 100)  // 1000ms = 1s
    QCoreApplication::processEvents();
  emit sendCmdToPort(QUERY2, data, false, false, false);
  return 0;
}

bool mportManager::isConnect() {
  if (mSendPort == nullptr) return false;
  return mSendPort->connected();
}

void mportManager::setCancelTry() { mCacelTry = true; }

int mportManager::send(const uint32_t &cmd, const QByteArray &data,
                       bool isGetErr, bool isRe) {
  mTimer->stop();
  emit sendCmdToPort(cmd, data, isRe, isGetErr, true);
  // sendQuery();
#if 1 ////TODO: 功能位置 是否去除？
  QTimer timer;
  QEventLoop loop;
  timer.setSingleShot(true);
  connect(this, &mportManager::valid, &loop, &QEventLoop::quit);
  connect(&timer, &QTimer::timeout, [&] { loop.quit(); });
  timer.start(2000);

  loop.exec();
  if (timer.isActive() == false) {
    emit sendCmdToPort(cmd, data, isRe, isGetErr, true);
  }
#endif
  // std::this_thread::sleep_for(std::chrono::seconds(1));
  mTimers = Config::getIns()->Get(config_com_time1).toInt();
  mTimer->start(mTimers);
  return true;
}

mportManager::~mportManager() { mTimer->stop(); }

int mportManager::resetPort(iport *p) {
  mSendPort = p;
  return 0;
}

int mportManager::saveFreqVer(uint8_t id) {
  mfreqVer = id;
  return 0;
}

void mportManager::updateTimes(int t) {
  mTimer->stop();
  mTimers = t;
  mTimer->start(t);
}

int mportManager::getFreqVer() { return mfreqVer; }

int mportManager::connectPort(const QString &name, uint8_t type) {
  if (type == 0) {
    mSerial *s = (mSerial *)getSerial("com");

    if (s->isOpen() && s->name() == name) {
      mTimer->start(mTimers);
      return true;
    }  //是否重连
    // if (s->isOpen()) s->close();
    s->close();
    if (QSerialPortInfo::availablePorts().empty()) {
      QMessageBox::critical(NULL, tr("error"), tr("Please insert com"),
                            QMessageBox::Yes);
      return -1;
    }
    bool isFound = false;
    for (auto it : QSerialPortInfo::availablePorts()) {
      if (it.portName() == name) {
        isFound = true;
        break;
      }
    }
    if (isFound == false) {
      QMessageBox::critical(NULL, tr("error"), tr("Not find com"),
                            QMessageBox::Yes);
      return -1;
    }
    s->setPortName(name);
    //  mportcfg->on_cboxPortName_currentIndexChanged(name);

    if (s->open(QIODevice::ReadWrite) == false) return -1;
    mTimer->start(mTimers);
  }
  return 0;
}

int mportManager::connectTcp(QString ip, int port) {
  TCPClient *pTcpClient = (TCPClient *)getSerial("tcp");
  pTcpClient->connect_(ip, port);
  mTimer->start(200);
  return 0;
}

bool mportManager::tryConnect(QVector<QString> &coms,
                              QMap<QString, QString> &viald) {
#if 0
  mSerial *s = (mSerial *)mportMg->getSerial("com");
  s->close();
  for (auto it : QSerialPortInfo::availablePorts()) {
    s->close();
    if (mCacelTry == true) return false;
    if (it.isBusy() || it.isNull()) continue;
    coms << it.portName();
  }
#else
  coms.clear();
  mTimer->stop();
  mCacelTry = false;
  if (mSendPort == nullptr) return false;
  if (QSerialPortInfo::availablePorts().empty()) {
    return false;
  }
  mSerial *s = (mSerial *)mportMg->getSerial("com");
  QTimer t;
  t.setSingleShot(true);
  QEventLoop loop;
  if (s->isOpen()) s->close();
  defer(mTimer->blockSignals(false));
  mTimer->blockSignals(true);
  for (auto it : QSerialPortInfo::availablePorts()) {
    s->close();
    if (mCacelTry == true) return false;
    if (it.isBusy() || it.isNull()) continue;
    s->setPortName(it.portName());
    // mportcfg->on_cboxPortName_currentIndexChanged(it.portName());
    if (s->isOpen()) s->close();
    if (s->open(QIODevice::ReadWrite) == false) continue;
    coms << it.portName();

    auto conn = QObject::connect(
                  this, &mportManager::sendInfo,
    [&](QString name, queryInfo info, int index) {
      if (index != 0) return;
      std::string sn((char *)info.LaserSN, sizeof(info.LaserSN));

      viald[it.portName()] = QByteArray::fromStdString(sn);

      t.stop();
      loop.quit();
    });
    // sendQuery();
    // auto conn = QObject::connect(this, &mportManager::valid, [&](QString
    // name) {
    //  t.stop();
    //  viald << it.portName();
    //  loop.quit();
    //});

    auto timecnt = QObject::connect(&t, &QTimer::timeout, [&t, &loop]() {
      t.stop();
      loop.quit();
    });
    send(QUERY1, QByteArray(), false);
    t.start(2000);
    refresh();
    loop.exec();
    QObject::disconnect(timecnt);
    t.stop();
    QObject::disconnect(conn);
    s->close();
    // }
  }
  mTimer->blockSignals(false);
#endif

  return true;
}

void mportManager::sendDisconnect(QString name) {
  mTcpIsConnected = false;
  if (mTimer->isActive()) mTimer->stop();
  emit DisConnect(name);
}
void mportManager::sendConnect(QString name) {
  if (!mTimer->isActive())mTimer ->start(400);
  mTcpIsConnected = true;
}
void mportManager::close() {
  m_SNManage.clear();
  mTimer->stop();
  if (mSendPort == nullptr) return;
  mSendPort->close();
}
void mportManager::refresh() {
  mRefresh[0] = true;
  mRefresh[1] = true;
}

iport *mportManager::getSerial(const QString &name) { return mSendPort; }

void mportManager::receiveData(QString name, cmdData data) {
  if (mSendPort == nullptr) return;
  if (mSendPort->name().compare(name)) return;

  emit valid(name);
  if (data.cmd == QUERY1) {
    queryInfo info;
    parseQuery1(info, data.data);
    // if (mQuery1 == data.data && mRefresh[0] == false) return;
    mRefresh[0] = false;
    QString sn = QString::fromLocal8Bit((char *)info.LaserSN, 14);
    if (m_SNManage.isEmpty()) {
      m_currSN = sn;
      m_SNManage.setCurrSn(m_currSN);
      emit sendSn(sn, true, true);
      m_SNManage.addSN(m_currSN);
    } else {
      if (!m_SNManage.contains(sn)) {
        emit sendSn(sn, true, false);
        m_SNManage.addSN(sn);
      } else {
        m_SNManage.updateSN(sn);
      }
    }
    if (m_currSN == sn) {
      emit sendInfo(name, info, 0);
    } else {
      if (info.alarm_in != 0) {
        emit alarmSn(sn);
      }
    }
    emit sendCmd(name, data, sn);
    mQuery1 = data.data;
    return;
  }
  if (data.cmd == QUERY2) {
    queryInfo info;
    parseQuery2(info, data.data);
    // if (mQuery2 == data.data && mRefresh[1] == false) return;
    mRefresh[1] = false;
    QString sn = QString::fromLocal8Bit((char *)info.LaserSN, 14);
    if (m_currSN == sn) {
      emit sendInfo(name, info, 1);
    }
    mQuery2 = data.data;
    emit sendCmd(name, data, sn);
    return;
  }

}

void mportManager::receiveDataFromTcp(QString name, cmdData data) {
  emit valid(name);

  if (data.cmd == QUERY1) {
    queryInfo info;
    parseQuery1(info, data.data);
    QString sn = QString::fromLocal8Bit((char *)info.LaserSN, 14);
    // if (mQuery1 == data.data && mRefresh[0] == false) return;
    mRefresh[0] = false;
    emit sendInfo(name, info, 0);
    emit sendCmd(name, data, sn);
    mQuery1 = data.data;

    return;
  }
  if (data.cmd == QUERY2) {
    queryInfo info;
    parseQuery2(info, data.data);
    QString sn = QString::fromLocal8Bit((char *)info.LaserSN, 14);
    // if (mQuery2 == data.data && mRefresh[1] == false) return;
    mRefresh[1] = false;
    emit sendInfo(name, info, 1);
    mQuery2 = data.data;
    emit sendCmd(name, data, sn);
    return;
  }

}


int mportManager::parseQuery1(queryInfo &info, const QByteArray &data) {
  if (data.isEmpty()) return 0;
  // QLOG_DEBUG() << data.toHex();
  QDataStream packet(data);
  packet.setByteOrder(QDataStream::BigEndian);
  uint16_t bytes;
  uint8_t byte;
  // 电流
  packet >> bytes;
  info.DL_set << bytes;
  packet >> bytes;
  info.DL_set << bytes;
  packet >> bytes;
  info.DL_set << bytes;

  //电流状态

  packet >> byte;
  info.DL_Status << byte;
  packet >> byte;
  info.DL_Status << byte;
  packet >> byte;
  info.DL_Status << byte;

  packet >> info.Freq_set;
  packet >> info.Burst_num;

  packet >> bytes;
  info.YS_set << bytes;
  packet >> bytes;
  info.YS_set << bytes;

  packet >> info.Freq_DA;
  packet >> info.Freq_DA_Status;
  info.Freq_DA_Status &= 0x01;
  packet >> info.tri_state_in;

  packet >> bytes;
  info.YS_set << bytes;

  packet >> info.JG_Run;
  packet >> info.MK_set2;

  packet >> bytes;
  info.DL_JX << bytes;
  packet >> bytes;
  info.DL_JX << bytes;
  packet >> bytes;
  info.DL_JX << bytes;

  packet >> info.alarm_in;
  // seed lock
  packet >> info.seed_state;

  packet >> bytes;
  info.DL_GZ << bytes;

  packet >> bytes;
  info.DL_GZ << bytes;
  packet >> bytes;
  info.DL_GZ << bytes;

  packet >> bytes;

  packet >> bytes;
  info.Amp_work << bytes;
  packet >> bytes;
  info.Amp_work << bytes;
  packet >> bytes;
  info.Amp_work << bytes;

  packet >> bytes;
  info.JTWD_work << bytes;
  packet >> bytes;
  info.JTWD_work << bytes;
  uint8_t b;
  packet >> b;

  packet >> info.QTSD1;
  packet >> info.SLL;
  // 64
  packet >> info.Debug;
  info.Debug &= 0x01;
  packet >> info.SHG_WD_set;
  packet >> info.THG_WD_set;
  packet >> info.SHG_WD_work;
  packet >> info.THG_WD_work;
  packet >> b;
  packet >> info.QTSD2;
  packet >> info.MNSZ_status;
  packet >> info.Power_INOUT_status;
  info.Power_INOUT_status &= 0x01;

  packet >> info.Power_INOUT_BF;
  packet >> info.Power_red;
  packet >> info.PSO_POD_version;
  packet.readRawData((char *)info.LaserSN, sizeof(info.LaserSN));

  packet >> bytes;
  info.SEED_LD_set << bytes;
  packet >> bytes;
  info.Seed_LD_work << bytes;
  packet >> byte;
  info.seed_LD_run << bytes;

  packet >> bytes;
  info.SEED_LD_set << bytes;
  packet >> bytes;
  info.Seed_LD_work << bytes;
  packet >> byte;
  info.seed_LD_run << bytes;

  for (auto i = 0; i < 3; i++) {
    packet >> bytes;
    info.seed_T_set << bytes;
    packet >> bytes;
    info.seed_T_work << bytes;
  }

  packet >> info.PIN2_set;
  packet >> info.Warning1_switch;
  packet >> bytes;

  packet >> bytes;
  info.SX_YS << bytes;
  info.YS_set << bytes;
  packet >> bytes;
  info.XH_YS << bytes;
  info.YS_set << bytes;
  packet >> info.FP0;

  for (auto i = 0; i < 4; i++) {
    packet >> bytes;
    info.SX_YS << bytes;
  }

  packet >> info.POD_GATE;
  info.POD_GATE &= 0x01;
  packet >> info.pin1;
  packet >> info.Warning2_switch;
  packet >> info.QDNC_QDC;
  info.QDNC_QDC &= 0x01;
  packet >> info.F_max_set_out;
  packet >> info.F_min_set_out;
  packet >> info.Burst_max_set_out;
  packet >> info.Burst_min_set_out;
  packet >> info.BP;
  packet >> b;
  packet >> info.QT_WD1;
  packet >> b;
  packet >> info.QT_WD2;
  packet.readRawData((char *)info.JG_run_times, 4);
  packet >> bytes;
  info.SX_YS << bytes;
  packet.readRawData((char *)info.hard_version, 4);
  packet >> bytes;
  info.DL_set << bytes;
  packet >> byte;
  info.DL_Status << byte;

  packet >> bytes;
  info.DL_JX << bytes;

  packet >> bytes;
  info.DL_GZ << bytes;

  for (auto i = 0; i < 5; i++) {
    packet >> bytes;
    info.XH_YS << bytes;
  }

  packet >> info.seed_run_point;

  packet >> bytes;
  info.DL_set << bytes;

  packet >> byte;
  info.DL_Status << byte;
  packet >> bytes;
  info.DL_JX << bytes;

  packet >> bytes;
  info.DL_GZ << bytes;

  packet >> info.m20_50;
  info.m20_50 &= 0x01;
  packet >> info.Warning3_switch;

  packet >> bytes;
  info.Amp_work << bytes;
  packet >> bytes;
  info.Amp_work << bytes;

  packet >> bytes;
  info.JTWD_work << bytes;
  packet >> bytes;
  info.JTWD_work << bytes;
  packet >> bytes;
  info.JTWD_work << bytes;

  packet >> info.freq_zb;
  packet >> info.freq_fb;
  // packet >> b;
  for (auto i = 0; i < 4; i++) {
    packet >> bytes;
    info.XH_YS << bytes;
  }

  packet >> info.mode;
  info.mode &= 0x3;
  packet >> info.seed_run_times;
  // emit sendCmdToPort(QUERY2, data, false, false);
  return 1;
}

int mportManager::parseQuery2(queryInfo &info, const QByteArray &data) {
  if (data.isEmpty()) return 0;

  QDataStream packet(data);
  packet.setByteOrder(QDataStream::BigEndian);
  uint16_t bytes;
  for (auto i = 0; i < 5; i++) {
    packet >> bytes;
    info.SXMK << bytes;
  }

  for (auto i = 0; i < 10; i++) {
    packet >> bytes;
    info.XHMK << bytes;
  }
  packet >> info.FP1;
  packet >> info.FP2;
  packet >> info.PowerC;
  packet >> info.PowerPz;
  packet >> info.KG;
  for (auto i = 0; i < 5; i++) {
    packet >> bytes;
    info.Power << bytes;
  }

  packet >> info.SL2;
  packet >> info.PowerCorrect;
  packet >> info.Dangwei;
  packet >> info.CorrectSwitch;

  packet >> info.CorrectCmd;
  packet >> info.CorrenctBurst;
  packet >> info.CorrenctStatus;

  packet.readRawData((char *)info.LaserSN, sizeof(info.LaserSN));
  uint8_t byte;
  /* for (auto i = 0; i < 14; i++) {
     packet >> byte;
   }*/

  packet >> info.CollectTime;
  packet >> info.SaveFlashEnableState;
  packet >> info.slLimit1Set;
  packet >> info.slLimit2Set;
  packet >> info.slzSet;
  packet >> info.slfSet;
  packet >> info.GateFre1Set;
  packet >> info.GateFre2Set;
  packet >> info.Power_INOUT_BF_2;
  packet >> info.Debug_2;

  return 1;
}

mportManager *mportManager::instance() { return g_portMg; }


void mportManager::timerSlot() { sendQuery(); }

void mportManager::sendDataToSerial(const uint32_t &cmd, const QByteArray &data,
                                    bool isRefresh, bool getErr, bool isPrint) {
  if (mSendPort == nullptr) {
    return;
  }
  if (cmd != QUERY1 && cmd != QUERY2) {
    return;
  }
  // mTimer->blockSignals(true);
  QByteArray s;
  QByteArray calData;
  QDataStream packet(&s, QIODevice::ReadWrite);
  QDataStream pCal(&calData, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  // packet << g_head;

  packet.writeRawData((const char *)g_head, sizeof(g_head));
  // s.push_back(QByteArray::fromRawData((const char*)g_head, sizeof(g_head)));
  //命令字
  uint32_t c = cmd & 0xffffff;
  uint8_t c1 = c >> 16;
  uint8_t c2 = c >> 8;
  uint8_t c3 = c & 0xff;
  pCal << c1 << c2 << c3;

  pCal << (uint16_t)data.length();
  pCal.writeRawData(data.data(), data.length());
  uint8_t a = 0, x = 0;
  for (auto i : calData) {
    a += i;
    x ^= i;
  }

  packet.writeRawData(calData.data(), calData.length());
  packet << x << a;
  packet << (uint8_t)0xd;

  QString err;
  if (dynamic_cast<TCPClient *>(mSendPort) && mTcpIsConnected == true)
    mSendPort->send(s, err, isPrint);
  if (dynamic_cast<mSerial *>(mSendPort))
    mSendPort->send(s, err, isPrint);
  if (isRefresh) refresh();
  if (getErr && err.isEmpty() == false) {
    QMessageBox::warning(nullptr, tr("is error"), err);
    mTimer->stop();
  }

  return;
}

void mportManager::saveCmdData(QString name, cmdData cmd, QString sn) {
  mSql->setCmd(cmd.cmd, cmd.data, name, sn);
}

void mportManager::portErr(int err) {
  if (err == QSerialPort::TimeoutError) {
    mTimer->stop();
    mTimers = 5000;
    QLOG_DEBUG() << "time out";
    mTimer->start(mTimers);
  }
}

void mportManager::setLogin(const uint32_t &isLogin) {
  mIsLogin = isLogin;
  //  emit curePowerEnable(mIsLogin);
}

uint32_t mportManager::getLogin() { return mIsLogin; }
void mportManager::setEnableCorr(int isenable) {
  mIsEnableCorr = (isenable != 0);
}
void mportManager::setCurrentSn() {
  QRadioButton *radioButton = qobject_cast<QRadioButton *>(sender());
  radioButton->setStyleSheet("QRadioButton {"
                             "border-image: url(:/img/button_normal.png);"
                             "font: 13pt;"
                             "color: rgb(98, 98, 98);"
                             "padding-left: 4px;"
                             "padding-right: 4px;"
                             "}");
  m_currSN = radioButton->text();
  m_SNManage.setCurrSn(m_currSN);
}
void mportManager::onRemoveSn(QString sn) {
  if (sn != m_currSN) {
    emit sendSn(sn, false, false);
  }
}
void mportManager::onClearSn() {
  return m_SNManage.clear();
}
bool mportManager::getCureEnable() { return mIsEnableCorr; }
