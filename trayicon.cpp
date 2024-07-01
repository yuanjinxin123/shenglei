#include "trayicon.h"
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "GraphicsView.h"
#include "config.h"
#include <QProcess>

static void restartApplication() {
  QString program = QCoreApplication::applicationFilePath();
  QStringList arguments = QCoreApplication::arguments();

  QProcess::startDetached(program, arguments);
  QCoreApplication::exit(0);
}

TrayIcon::TrayIcon(QWidget *parent)
  : QWidget(parent) {
  m_pTrayIcon = new QSystemTrayIcon(this);
  m_pTrayIcon->setIcon(QIcon(":/img/logo_t.png"));
  m_pTrayIcon->setToolTip(tr("ShengLei Laser"));
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
  QAction *actionTheme = new QAction(QIcon(":/img/lang.png"), tr("lang"), nullptr);
  QMenu *subMenu = new QMenu(tr("lang"), nullptr);
  actionTheme->setMenu(subMenu);
  QAction *ZhAction = new QAction(tr("Chinese"), nullptr);
  QAction *EnAction = new QAction(tr("English"), nullptr);
  connect(ZhAction, SIGNAL(triggered()), this, SLOT(ChangeZh()));
  connect(EnAction, SIGNAL(triggered()), this, SLOT(ChangeEn()));
  subMenu->addAction(ZhAction);
  subMenu->addAction(EnAction);
  trayMenu->addAction(actionTheme);
  QAction *actionAbout = new QAction(QIcon(":/img/about.png"), tr("about"), nullptr);
  trayMenu->addAction(actionAbout);
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()));
  QAction *actionQuit = new QAction(QIcon(":/img/exit.png"), tr("exit"), nullptr);
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(DoQuit()));
  trayMenu->addAction(actionQuit);
  m_pTrayIcon->setContextMenu(trayMenu);
}

void TrayIcon::DoQuit() {
  m_pTrayIcon->setVisible(false);
  this->close();
  exit(0);
}


void TrayIcon::ShowAbout() {
  QMessageBox::about(NULL, tr("about"), tr("ShengLei Laser"));
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
