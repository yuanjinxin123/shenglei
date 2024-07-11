#include "trayicon.h"
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "GraphicsView.h"
#include "config.h"
#include <QProcess>
#include "mdatecheckdlg.h"
#include "mportManager.h"
#include "define.h"


static void restartApplication() {
  QString program = QCoreApplication::applicationFilePath();
  QStringList arguments = QCoreApplication::arguments();

  QProcess::startDetached(program, arguments);
  exit(0);
}

TrayIcon::TrayIcon(QWidget *parent)
  : QWidget(parent) {
  m_pTrayIcon = new QSystemTrayIcon(this);
#ifdef SHENGXIONG
  m_pTrayIcon->setIcon(QIcon(":/img/logo.png"));
  m_pTrayIcon->setToolTip(tr("ShengXiong Laser"));
#elif defined(SHENGLEI)
  m_pTrayIcon->setIcon(QIcon(":/img/logo_t.png"));
  m_pTrayIcon->setToolTip(tr("ShengLei Laser"));
#else
  m_pTrayIcon->setIcon(QIcon(":/img/logo_no.png"));
  m_pTrayIcon->setToolTip(tr("Laser Control Software"));
#endif
  m_pTrayIcon->show();
  connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
  CreateActions();
}

TrayIcon::~TrayIcon() {
}

void TrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::Trigger:
      GraphicsView::GetInstancePointer()->show();
      break;
    case QSystemTrayIcon::DoubleClick:
      GraphicsView::GetInstancePointer()->show();
      break;
    default:
      break;
  }
}

void TrayIcon::CreateActions() {
  QFont font;
  font.setPointSize(12); // 设置字体大小
  QMenu *trayMenu = new QMenu();
  trayMenu->setFont(font);
  QAction *actionLang = new QAction(QIcon(":/img/lang.png"), tr("lang"), nullptr);
  QMenu *subMenu = new QMenu(tr("lang"), nullptr);
  actionLang->setMenu(subMenu);
  QAction *ZhAction = new QAction(QIcon(":/img/Chinese.png"), tr("中文"), nullptr);
  QAction *EnAction = new QAction(QIcon(":/img/English.png"), tr("English"), nullptr);
  connect(ZhAction, SIGNAL(triggered()), this, SLOT(ChangeZh()));
  connect(EnAction, SIGNAL(triggered()), this, SLOT(ChangeEn()));
  subMenu->addAction(ZhAction);
  subMenu->addAction(EnAction);
  subMenu->setFont(font);
  //trayMenu->addAction(actionLang);
  m_PuerryData = new QAction(QIcon(":/img/query.png"), tr("query"), nullptr);
  trayMenu->addAction(m_PuerryData);
  connect(m_PuerryData, SIGNAL(triggered()), this, SLOT(ShowHisData()));
  QAction *actionAbout = new QAction(QIcon(":/img/about.png"), tr("about"), nullptr);
  trayMenu->addAction(actionAbout);
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
  QAction *actionQuit = new QAction(QIcon(":/img/exit.png"), tr("exit"), nullptr);
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(DoQuit()));
  trayMenu->addAction(actionQuit);
  m_pTrayIcon->setContextMenu(trayMenu);
  connect(m_pTrayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::onIconActivated);
}

void TrayIcon::DoQuit() {
  mportMg->close();
  m_pTrayIcon->setVisible(false);
  this->close();
  exit(0);
}


void TrayIcon::ShowAbout() {
#ifdef SHENGXIONG
  QMessageBox::about(NULL, tr("about"), tr("ShengXiong Laser"));
#elif defined(SHENGLEI)
  QMessageBox::about(NULL, tr("about"), tr("ShengLei Laser"));
#else
  QMessageBox::about(NULL, tr("about"), tr("Laser Control Software"));
#endif
}

void TrayIcon::ShowHisData() {
  mDateCheckDlg dlg;
  dlg.exec();
}

void TrayIcon::ChangeZh() {
  if (Config::getIns()->Get("main/lang").toString() == "zh") {
    return;
  } else {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, tr("Restart"), tr("Do you want to restart the application?"), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
      Config::getIns()->Set("main", "lang", "zh");
      restartApplication();
    }
  }
}

void TrayIcon::ChangeEn() {
  if (Config::getIns()->Get("main/lang").toString() == "en") {
    return;
  } else {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, tr("Restart"), tr("Do you want to restart the application?"), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
      Config::getIns()->Set("main", "lang", "en");
      restartApplication();
    }
  }
}

void TrayIcon::onIconActivated() {
  if (mportMg->getLogin() != 2) {
    m_PuerryData->setEnabled(false);
  } else {
    m_PuerryData->setEnabled(true);
  }
}
