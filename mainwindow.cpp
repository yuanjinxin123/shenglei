#include "mainwindow.h"

#include <qfile.h>

#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QFuture>
#include <QLabel>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QPair>
#include <QTimer>
#include <QtConcurrent>
#include <QHBoxLayout>

#include "config.h"
#include "defer.h"
#include "dlg_act.h"
#include "log.h"
#include "mCorrectionDlg.h"
#include "mFristPage.h"
#include "mconnectdlg.h"
#include "mdatecheckdlg.h"
#include "mlogindlg.h"
#include "mportmanager.h"
#include "msettingdlg.h"
#include "mwarmInfo.h"
#include "order.h"
#include "portcfg.h"
#include "qserialportinfo.h"
#include "sql.h"
#include "ui_mainwindow.h"
#define g_verStr "v3.0.1"

#include <qdesktopwidget.h>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(NULL),
    ui(new Ui::MainWindow),
    mPowerEditing(false),
    mPowerVal(0) {
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint | windowFlags());
  installEventFilter(ui->mTitleBar);
  ui->mTitleBar->setFrameWindow(parent);
  setWindowTitle(Config::getIns()->Get(config_main_title).toString());

#ifdef DXJG_SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#else
  setWindowIcon(QIcon(":/img/logo_t.png"));
#endif
  setFocusPolicy(Qt::ClickFocus);
  init();
  this->setMinimumSize(1750, 1000);
  this->resize(QSize(1750, 1000));
  //resize(QDesktopWidget().availableGeometry(this).size() * 0.85);
  ui->mPowerSet->setFocusPolicy(Qt::StrongFocus);
  // QDesktopWidget* desktopWidget = QApplication::desktop();
  // int _index = desktopWidget->screenNumber(this);
  // QRect rect = desktopWidget->screenGeometry(_index);  //适配多显示器


  mTimeTimer = new QTimer();
  connect(mTimeTimer, SIGNAL(timeout()), this, SLOT(showTime()));
  mTimeTimer->start(1000);
}
void setTabWidgetStyleSheet(QTabWidget *obj) {}
MainWindow::~MainWindow() {
  pSerial->disconnect(nullptr);
  mportManager::instance()->resetPort();
  // loginDlg->deleteLater();
  delete ui;
}

void MainWindow::init() {
  windowTitle();
  QVariant vtime;
  vtime = Config::getIns()->Get(config_com_valid);
  if (vtime.isValid()) mVtime = vtime.toUInt();
  //考虑 token
  ui->mLoginBtn->setText(tr("user"));

  mGgif.setFileName(":/img/y_g.gif");

  QMap<QDateTime, cmd_info> cmd;
  // mSql->getCmd(QMap<QDateTime,cmd_info>& cmd, int limit, int offset);

//  ui->mCntBtn->setIcons(QString(":/img/cnt1.png"), QString(":/img/cnt.png"),
//                        QString(":/img/cnt.png"));

//  ui->mSetBtn->setIcons(QString(":/img/set1.png"), QString(":/img/set.png"),
//                        QString(":/img/set.png"));
  ui->mLoginBtn->setIcons(QString(":/img/login1.png"),
                          QString(":/img/login1.png"),
                          QString(":/img/login1.png"));

  mMenuLogin = new QMenu(this);
  // mQSql = new QAction(tr("check data"), this);

  mMenuLogin->addAction(tr("login out"), this, SLOT(actionsSlot()));
  // mMenuLogin->addAction(tr("check data"), this, SLOT(checkDlg()));
  mMenuLogin->setStyleSheet(R"(
    QMenu {
		font-size: 12px;
		min-height: 20px;
    }
    QMenu::item {
        padding: 2px 20px 2px 20px;
    }
    QMenu::item:selected {
        background-color: rgb(190,210,224);
    }
	)");
  ui->highestWidget->setWidget(ui->mHighestTab->getTableWidget());

  ui->mWorkSpace->setWidget(ui->mWorkSpaceTop, ui->mWorkSpaceLeft, ui->mTab, ui->highestWidget);
  for (auto i = 0; i < ui->mWorkSpace->count(); i++) {
    mportMg->mWdtList << qMakePair(ui->mWorkSpace->tabText(i), ui->mWorkSpace->widget(i));

    // QLOG_DEBUG() << ui->mWorkSpace->tabText(i);
  }
  ui->mWorkSpace->clear();
  for (auto iter = mportMg->mWdtList.begin(); iter != mportMg->mWdtList.end();
       iter++) {
    if (iter->first == tr("highest") || iter->first == tr("electric")) continue;
    if (iter->first == tr("acousto")) {
      iter->second->setProperty("icon", ":/img/sg.png");
    }
    QString icon = ":/img/home.png";
    auto wd = iter->second;
    auto iconV = wd->property("icon");
    if (iconV.isValid()) {
      icon = iconV.toString();
    }

    ui->mWorkSpace->addTab(iter->second, QIcon(icon), iter->first);
  }

  //状态栏
  QWidget *widget = new QWidget();
  QHBoxLayout *layout = new QHBoxLayout(widget);
  layout->addStretch();
  layout->setMargin(5);
  layout->setSpacing(5);
  statusBar()->addWidget(new QWidget(), 1);
  statusBar()->addWidget(widget, 2);
  statusBar()->addWidget(new QWidget(), 1);

  QString fontSheet = "font-size:14pt;";
  QString verName(tr("Soft Ver:"));
  auto mverLabel = new QLabel(this);
  mverLabel->setStyleSheet(fontSheet);
  mverLabel->setText(verName + g_verStr);
  layout->addWidget(mverLabel);
  layout->addSpacing(15);
  mhverLabel = new QLabel;
  mhverLabel->setStyleSheet(fontSheet);
  verName = tr("Hard Ver:");
  QString decs;
  QVariant hVal;
  if (mSql->getValue("statusbar", "hver", hVal, decs) == false) {
    mHver = "none";
  } else {
    mHver = hVal.toString();
  }
  mhverLabel->setText(verName + mHver);
  layout->addWidget(mhverLabel);
  layout->addSpacing(15);

  QLabel *warmImage = new QLabel;
  warmImage->setFixedSize(45, 45);
  warmImage->setStyleSheet("border-image:url(:/img/state_warm.png);");
  layout->addWidget(warmImage);
  mWarmLbl = new QLabel;
  mWarmLbl->setStyleSheet(fontSheet);
  layout->addWidget(mWarmLbl);

  //lfh
  stateLight = new QLabel;
  stateLight->setFixedSize(20, 20);
  stateLight->setStyleSheet("border-image:url(:/img/status_warn.png);");
  layout->addWidget(stateLight);
  layout->addSpacing(15);

  setWarnStatus(0, 1001);

  QLabel *seedImage = new QLabel;
  seedImage->setFixedSize(45, 45);
  seedImage->setStyleSheet("border-image:url(:/img/state_seed_status.png);");
  layout->addWidget(seedImage);

  mSeedStatus = new QLabel;
  mSeedStatus->setStyleSheet(fontSheet);
  mSeedStatus->setProperty("status", false);
  mSeedStatus->setText(QString(tr("Unlock")));
  //mSeedStatus->setIcon(QIcon(":/img/unlock.png"));
  //mSeedStatus->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
  mSeedStatus->setMinimumHeight(35);
  layout->addWidget(mSeedStatus);
  layout->addSpacing(15);

  QLabel *runtimeImage = new QLabel;
  runtimeImage->setFixedSize(45, 45);
  runtimeImage->setStyleSheet("border-image:url(:/img/state_seed_runtime.png);");
  layout->addWidget(runtimeImage);

  mRuntime = new QLabel(tr("Seed run time:0h"));
  mRuntime->setStyleSheet(fontSheet);
  mRuntime->setProperty("val", 0);
  layout->addWidget(mRuntime);
  layout->addSpacing(15);

  QLabel *powerImage = new QLabel;
  powerImage->setFixedSize(45, 45);
  powerImage->setStyleSheet("border-image:url(:/img/state_power.png);");
  layout->addWidget(powerImage);

  mPowerLabel = new QLabel(tr("Power:0W"));
  mPowerLabel->setStyleSheet(fontSheet); //("color:#006400;font-weight: bold;");
  layout->addWidget(mPowerLabel);
  mPowerLabel->setProperty("val", 0);
  layout->addSpacing(15);

  QLabel *jgtimeImage = new QLabel;
  jgtimeImage->setFixedSize(45, 45);
  jgtimeImage->setStyleSheet("border-image:url(:/img/state_jgtime.png);");
  layout->addWidget(jgtimeImage);

  mJgTime = new QLabel(tr("Laser run time:0h"));
  mJgTime->setStyleSheet(fontSheet);
  mJgTime->setProperty("val", 0);
  layout->addWidget(mJgTime);
  statusBar()->setSizeGripEnabled(true);


  ui->mSnlab->setText(QString("SN:"));
  ui->mAcoustoSdW->setCurrentIndex(0);
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(mportManager::instance(), SIGNAL(changePsoOrPod(uint8_t)),
                   this, SLOT(changePsoOrPod(uint8_t)));
  QObject::connect(mportManager::instance(), SIGNAL(DisConnect(QString)), this,
                   SLOT(DisPortConnect(QString)));

  QObject::connect(ui->mPowerSet, SIGNAL(valueChanged(int)), this,
                   SLOT(PowerChange(int)));

  QObject::connect(ui->mPowerSet, SIGNAL(editingFinished()), this,
                   SLOT(PowerFinish()));
  // uint32_t timeout = 5000;

  connect(&mValitTimer, &QTimer::timeout, this, &MainWindow::ValitTimeout);
  QObject::connect(mportMg, &mportManager::valid, [&]() {
    // if (mIsConnect == true) return;
    if (isChange == false) isChange = true;
    if (mJKStatus == false) mportMg->refresh();
    // mValitTimer.start(timeout);
    // mValitTimer.blockSignals(false);
  });

  pSerial = new mSerial(this);
  mportManager::instance()->resetPort(pSerial);
  pSerial->connect_();
  m_tcpClient = new TCPClient(this);
  mportManager::instance()->resetPort(pSerial);

  ui->mCntTypeCbox->addItem(tr("COM"));
  ui->mCntTypeCbox->addItem(tr("TCP"));

  visibleNetCtl(false);
  QObject::connect(ui->mCntTypeCbox, &QComboBox::currentTextChanged, [this](const QString & text) {
    if (text == tr("TCP")) {
      mportManager::instance()->resetPort(m_tcpClient);
      visibleNetCtl(true);
    } else if (text == tr("COM")) {
      mportManager::instance()->resetPort(pSerial);
      visibleNetCtl(false);
    }
  });
  QString ip = Config::getIns()->Get("NET/ip").toString();
  QString port = Config::getIns()->Get("NET/port").toString();
  ui->lineEdit_ip->setText(ip);
  ui->lineEdit_port->setText(port);
}

void MainWindow::initData() {}

void MainWindow::setWarnStatus(int s, int err) {
  if (err == 1001) {
    ui->mCntTypeCbox->setEnabled(true);
    ui->mCntBtn->setText(tr("Connect"));
  } else {
    ui->mCntTypeCbox->setEnabled(false);
    ui->mCntBtn->setText(tr("Disconnect"));
  }
  QString path;
  QPalette pe;
  if (err == 0) {
    pe.setColor(QPalette::WindowText, Qt::black);
    stateLight->setStyleSheet("border-image:url(:/img/state_green.png);");
    mWarmLbl->setPalette(pe);
  } else {
    pe.setColor(QPalette::WindowText, Qt::red);
    mWarmLbl->setPalette(pe);
    stateLight->setStyleSheet("border-image:url(:/img/status_warn.png);");
  }
  if (mwarmInfo().mWarnmap.contains(err))
    mWarmLbl->setText(mwarmInfo().mWarnmap[err]);
  else
    mWarmLbl->setText(tr("wei zhi cuo wu"));

  if (s == 0) path = QString(":/img/red.png");
  if (s == 1) path = QString(":/img/green.png");
  if (s == 2) {
    ui->mWarnStatus->setMovie(&mGgif);
    mGgif.start();
    return;
  }
  if (ui->mWarnStatus->movie() != nullptr) {
    mGgif.stop();
    ui->mWarnStatus->setMovie(nullptr);
  }
  QImage im;
  im.load(path);
  QPixmap px = QPixmap::fromImage(im);
  // show
  ui->mWarnStatus->setPixmap(px.scaled(ui->mWarnStatus->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  //ui->mWarnStatus->setPixmap(px);
}

void MainWindow::resizeEvent(QResizeEvent *event) {}

void MainWindow::timerEvent(QTimerEvent *event) { mportMg->sendQuery(); }

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->mPowerSet && event->type() == QEvent::FocusIn) {
    mPowerChange = true;
  }
  if (obj == ui->mPowerSet && event->type() == QEvent::FocusOut)
    mPowerChange = false;

  return QWidget::eventFilter(obj, event);
}

void MainWindow::visibleNetCtl(bool visible) {
  ui->label_ip->setVisible(visible);
  ui->label_port->setVisible(false);
  ui->lineEdit_ip-> setVisible(visible);
  ui->lineEdit_port->setVisible(false);
}

void MainWindow::on_MsetBtn_clicked() {
  msettingdlg dlg;
  dlg.exec();
}

void MainWindow::ReadSerial() { qDebug() << "test"; }

#define USE_PASSWORD
void MainWindow::on_mLoginBtn_clicked() {

#ifdef USE_PASSWORD
  if (ui->mLoginBtn->menu()) { //ZHLLLLL
    return;
  }
  if (isGetData == false) {
    QMessageBox::information(nullptr, tr("Prompt"), tr("Connection did not read data."));
    return;
  }
#endif

  mLoginDlg logDlg;
  if (logDlg.exec() != QDialog::Accepted) {
    return;
  }
  QString user = logDlg.user();
  if (user.compare("password", Qt::CaseInsensitive) == 0) {
    dlg_act act;
    act.exec();
    return;
  }
  int index = 0;
  uint32_t pin;
  auto p = logDlg.pwd();
#ifndef USE_PASSWORD
  p = "password"; //ZHLLLLL
#endif
  if (p.isEmpty()) {
    QMessageBox::information(nullptr, tr("Prompt"), tr("The password cannot be empty."));
    return;
  }
  pin = p.toInt();

  if (logDlg.user().compare("engineer", Qt::CaseInsensitive) == 0) {
    //QMessageBox::information(nullptr, tr("mPin1"), QString::number(mPin1));
#ifdef USE_PASSWORD
    if (pin != mPin1) {
      QMessageBox::information(nullptr, tr("Prompt"), tr("wrong password!"));
      return;
    }
#endif
    ui->mLoginBtn->setText(tr("Engineer"));
    mportMg->setLogin(1);
    auto iconV = mportMg->mWdtList[5].second->property("icon");
    QString icon;
    if (iconV.isValid()) {
      icon = iconV.toString();
    }
    //ui->mWorkSpace->insertTab(1, mportMg->mWdtList[1].second, QIcon(icon),
    //                    mportMg->mWdtList[1].first);
    index = 1;
    mLoginDlg::getIns()->setLogRole(index);
    index = mIsPod + 1;
    index %= 3;
    ui->mAcoustoSdW->setCurrentIndex(index);
  } else if (logDlg.user().compare("administrator", Qt::CaseInsensitive) == 0) {
    //QMessageBox::information(this, tr("mPin2"), QString::number(mPin2));
#ifdef USE_PASSWORD
    if (pin != mPin2) {
      QMessageBox::information(nullptr, tr("Prompt"), tr("wrong password!"));
      return;
    }
#endif
    ui->mLoginBtn->setText(tr("Admin"));
    mportMg->setLogin(2);
    index = mIsPod + 1;
    index %= 3;
    ui->mAcoustoSdW->setCurrentIndex(index);
    index = 2;
    mLoginDlg::getIns()->setLogRole(index);

    if (ui->mWorkSpace->count() < 6) {
      auto iconV = mportMg->mWdtList[5].second->property("icon");
      QString icon;
      if (iconV.isValid()) {
        icon = iconV.toString();
      }

      ui->mWorkSpace->insertTab(1, mportMg->mWdtList[1].second, QIcon(icon),
                                mportMg->mWdtList[1].first);
      ui->mWorkSpace->addTab(mportMg->mWdtList[5].second, QIcon(":/img/gj.png"),
                             mportMg->mWdtList[5].first);
    }
  }

  ui->mLoginBtn->setMenu(mMenuLogin);

  return;
}

void MainWindow::actionsSlot() {
  ui->mLoginBtn->setText(tr("user"));
  ui->mLoginBtn->setMenu(nullptr);
  mportMg->setLogin(0);
  mLoginDlg::getIns()->setLogRole(0);
  ui->mAcoustoSdW->setCurrentIndex(0);
  for (auto i = 0; i < ui->mWorkSpace->count(); i++) {
    if (tr("highest") == ui->mWorkSpace->tabText(i) ||
        tr("electric") == ui->mWorkSpace->tabText(i)) {
      ui->mWorkSpace->removeTab(i);
    }
  }
  ui->highestWidget->setVisible(false);
}
void MainWindow::checkDlg() { mDateCheckDlg().exec(); }
void MainWindow::receiveQuery(QString name, queryInfo info, int a) {
  if (a == 0) {
    isGetData = true;
    mPin1 = info.pin1;
    mPin2 = info.PIN2_set;
    bool isState = info.seed_state;
    if (misLock != (info.seed_state > 0)) {
      if (isState) {
        mSeedStatus->setText(QString(tr("Lock")));
        //mSeedStatus->setIcon(QIcon(":/img/lock.png"));
        mSeedStatus->setProperty("status", true);
      } else {
        mSeedStatus->setText(QString(tr("Unlock")));
        //mSeedStatus->setIcon(QIcon(":/img/unlock.png"));
        mSeedStatus->setProperty("status", false);
      }
      misLock = isState;
    }
    if (ui->mPowerSet->getHasFouce() == false &&
        ui->mPowerSet->value() != info.Power_INOUT_BF) {
      ui->mPowerSet->setValue(info.Power_INOUT_BF);
      mPowerVal = info.Power_INOUT_BF;
    }
    double val = info.Power_red * 0.01;

    auto m =
      QString(tr("Power:")) + QString::number(val, 'f', 2) + QString("W");
    mPowerLabel->setText(m);

    if (info.alarm_in == 0) {
      setWarnStatus(1, info.alarm_in);
    } else if (info.alarm_in == 32) {
      setWarnStatus(2, info.alarm_in);
    } else {
      setWarnStatus(0, info.alarm_in);
    }

    std::string sn((char *)info.LaserSN, sizeof(info.LaserSN));
    // mhverLabel->setText()
    ui->mSnlab->setText(QString("SN:") + QString::fromStdString(sn));
    QString vpem;
    for (auto hver = 0; hver < sizeof(info.hard_version); hver++) {
      vpem += QString::number(info.hard_version[hver]);
      if ((hver + 1) != sizeof(info.hard_version)) vpem += QString('.');
    }
    QString verName = tr("Hard Ver:");
    if (mHver != vpem) {
      mHver = vpem;
      mSql->setKey("statusbar", "hver", mHver, "hard version");
      mhverLabel->setText(verName + vpem);
    }
    auto runTime = mRuntime->property("val").toUInt();
    if (runTime != info.seed_run_times) {
      mRuntime->setProperty("val", info.seed_run_times);
      runTime = info.seed_run_times;
      mRuntime->setText(tr("Seed run time:") + QString::number(runTime) + QString("h"));
    }
    uint32_t t;
    uint8_t b[4];
    for (int i = 0; i < 4; i++) {
      b[3 - i] = info.JG_run_times[i];
    }
    memcpy(&t, b, 4);

    runTime = mJgTime->property("val").toUInt();
    if (runTime != t) {
      uint64_t jgt = t * 1.138;
      mJgTime->setProperty("val", t);
      runTime = t;
      mJgTime->setText(tr("Laser run time:") + QString::number(jgt) + QString("h"));
    }
  }
}

void MainWindow::showMsg(QString msg) { mWarmLbl->setText(tr("waring")); }
void MainWindow::changePsoOrPod(uint8_t id) {
  mIsPod = id;

  if (mLoginDlg::getIns()->logRole() == 2 || mLoginDlg::getIns()->logRole() == 1)

    // ui->mAcoustoSdW->setAccessibleName(m);
    ui->mAcoustoSdW->setCurrentIndex(id + 1);
}

void MainWindow::changeConnectIcon(bool isok) {
  if (isok) {
    ui->mCntBtn->setIcons(QString(":/img/connected1.png"),
                          QString(":/img/connected.png"),
                          QString(":/img/connected.png"));
    setWarnStatus(0, 1000);

  } else {
    ui->mCntBtn->setIcons(QString(":/img/cnt1.png"), QString(":/img/cnt.png"),
                          QString(":/img/cnt.png"));
    setWarnStatus(0, 1001);
  }
}

void MainWindow::on_mCntBtn_clicked() {
  // changeConnectIcon(false);
  if (ui->mCntTypeCbox->currentText() == tr("COM")) {
    sender()->blockSignals(true);
    defer(sender()->blockSignals(false));
    mIsConnect = true;
    defer(mIsConnect = false);
    mconnectDlg d;
    uint8_t coms = 0;

    if (mportMg->isConnect()) {
      if (QMessageBox::question(nullptr, tr("Prompt"), tr("Are you sure you want to disconnect?"),
                                QMessageBox::Ok | QMessageBox::Cancel) ==
          QMessageBox::Ok) {
        mportMg->close();
      }
      //  sender()->blockSignals(false);
      return;
    }

    auto ret = d.tryConnectCom(coms);

    if (ret == false && coms <= 0) return;
    if (coms == 1) {
      changeConnectIcon(true);
      mportMg->refresh();
      mValitTimer.start(mVtime);
      return;
    }

    if (d.exec() == QDialog::Accepted) {
      changeConnectIcon(true);
      mportMg->refresh();
      mValitTimer.start(mVtime);
      return;
    }
  } else {
    if (mportMg->isConnect()) {
      if (QMessageBox::question(nullptr, tr("Prompt"), tr("Are you sure you want to disconnect?"),
                                QMessageBox::Ok | QMessageBox::Cancel) ==
          QMessageBox::Ok) {
        mportMg->close();
      }
      return;
    }
    mportManager::instance()->connectTcp(ui->lineEdit_ip->text(), ui->lineEdit_port->text().toInt());
  }
  // sender()->blockSignals(false);
}

void MainWindow::ValitTimeout() {
  // qDebug() << mValitTimer.remainingTime();
  // if (mIsConnect == true) return;
  if (isChange == false) {
    changeConnectIcon(true);
    mJKStatus = false;
  }
  isChange = false;
}

void MainWindow::on_mSetBtn_clicked() {
  msettingdlg dlg;
  // dlg.setOpenCom(pSerial);
  dlg.exec();
}

void MainWindow::on_mOk_clicked() {
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << mPowerVal;

  mportMg->send(GL_NK, val);
  mPowerEditing = false;
}

void MainWindow::DisPortConnect(QString name) {
  mIsConnect = false;
  isChange = false;
  mValitTimer.stop();
  changeConnectIcon(false);

  QLOG_DEBUG() << name << "is disconnect";
}

// void MainWindow::on_mReset_clicked() { PowerFinish(); }

void MainWindow::PowerChange(int d) {
  // if (mPowerEditing == false) mPowerEditing = true;
  if (ui->mPowerSet->getHasFouce() == false) return;
  auto pVal = d;
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << pVal;

  mportMg->send(GL_NK, val);
}

void MainWindow::PowerFinish() {}

void MainWindow::on_mReset_clicked() { PowerFinish(); }

void MainWindow::on_mResetBtn_clicked() {}
void QAbstractSpinBox::wheelEvent(QWheelEvent *e) {
  //啥也不干，屏蔽鼠标滚动
}

void MainWindow::on_mCorr_clicked() {
  mCorrectionDlg correDlg;
  // correDlg.setWindowModality(Qt::WindowModal);
  correDlg.showDlg();
}

void MainWindow::showTime() {
  ui->mLableTime->setText(QTime::currentTime().toString("hh:mm"));
}
