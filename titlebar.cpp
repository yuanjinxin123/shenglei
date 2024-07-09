#include "titlebar.h"

#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QMessageBox>
#include <QProcess>
#include "config.h"
#include "mportmanager.h"

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

static void restartApplication() {
  mportManager::instance()->close();
  QString program = QCoreApplication::applicationFilePath();
  QStringList arguments = QCoreApplication::arguments();

  QProcess::startDetached(program, arguments);
  exit(0);
}

TitleBar::TitleBar(QWidget *parent) : QWidget(parent) {
  setFixedHeight(50);
  setAttribute(Qt::WA_StyledBackground);

  m_pIconLabel = new QLabel(this);
  m_pTitleLabel = new QLabel(this);
  m_pChinese = new QLabel(this);
  m_pEnglish = new QLabel(this);
  m_pChineseButton = new QRadioButton;
  m_pEnglishButton = new QRadioButton;
  m_pMinimizeButton = new QPushButton(this);
  m_pMaximizeButton = new QPushButton(this);
  m_pCloseButton = new QPushButton(this);

  //三个按钮 在按钮定义的时候添加图片
//    QString pic = ":/img/small.png";
//    m_pMinimizeButton->setIconSize(QPixmap(pic).size());
//    m_pMinimizeButton->setFixedSize(QPixmap(pic).size());

//    pic = ":/img/big.png";
//    m_pMaximizeButton->setIconSize(QPixmap(pic).size());
//    m_pMaximizeButton->setFixedSize(QPixmap(pic).size());

//    pic = ":/img/close.png";
//    m_pCloseButton->setIconSize(QPixmap(pic).size());
//    m_pCloseButton->setFixedSize(QPixmap(pic).size());

  m_pMinimizeButton->setFixedSize(25, 25);
  m_pMaximizeButton->setFixedSize(25, 25);
  m_pCloseButton->setFixedSize(25, 25);

  m_pIconLabel->setFixedSize(32, 32);
  m_pIconLabel->setScaledContents(true);

  // m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  //pic = ":/img/logo_t2.png";
  m_pTitleLabel->setFixedSize(QSize(360, 32));
  m_pChinese->setFixedSize(QSize(48, 32));
  m_pEnglish->setFixedSize(QSize(80, 32));
  m_pChinese->setObjectName("mChinese");
  m_pChinese->setText("中文");
  m_pEnglish->setObjectName("mEnglish");
  m_pEnglish->setText("English");
  m_pTitleLabel->setObjectName("whiteLabel");
  m_pMinimizeButton->setObjectName("minimizeButton");
  m_pMaximizeButton->setObjectName("maximizeButton");
  m_pCloseButton->setObjectName("closeButton");

  m_pMinimizeButton->setToolTip("Minimize");
  m_pMaximizeButton->setToolTip("Maximize");
  m_pCloseButton->setToolTip("Close");

  QHBoxLayout *pLayout = new QHBoxLayout(this);
  pLayout->addWidget(m_pIconLabel);
  pLayout->addSpacing(5);
  pLayout->addWidget(m_pTitleLabel);
  pLayout->addStretch();
  pLayout->addWidget(m_pChinese);
  pLayout->addWidget(m_pChineseButton);
  pLayout->addSpacing(15);
  pLayout->addWidget(m_pEnglish);
  pLayout->addWidget(m_pEnglishButton);
  pLayout->addSpacing(60);
  pLayout->addWidget(m_pMinimizeButton);
  pLayout->addSpacing(15);
  pLayout->addWidget(m_pMaximizeButton);
  pLayout->addSpacing(15);
  pLayout->addWidget(m_pCloseButton);
  pLayout->setSpacing(3);
  setLayout(pLayout);

  m_pIconLabel->setVisible(false);
  if (Config::getIns()->Get("main/lang").toString() == "zh") {
    m_pChineseButton->setChecked(true);
  } else {
    m_pEnglishButton->setChecked(true);
  }
  connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
  connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
  connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
  connect(m_pChineseButton, &QRadioButton::toggled, this, &TitleBar::onChangeChinese);
  connect(m_pEnglishButton, &QRadioButton::toggled, this, &TitleBar::onChangeEnglish);
}

TitleBar::~TitleBar() {}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  emit m_pMaximizeButton->clicked();
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
#ifdef Q_OS_WIN
  if (ReleaseCapture()) {
    QWidget *pWindow = this->getFrameWindow();
    if (pWindow->isTopLevel()) {
      SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION,
                  0);
    }
  }
  event->ignore();
#else
#endif
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event) {
  switch (event->type()) {
//    case QEvent::WindowTitleChange: {
//        QWidget *pWidget = qobject_cast<QWidget *>(obj);
//        if (pWidget) {
//            m_pTitleLabel->setText(pWidget->windowTitle());
//            return true;
//        }
//    }
    case QEvent::WindowIconChange: {
      QWidget *pWidget = qobject_cast<QWidget *>(obj);
      if (pWidget) {
        QIcon icon = pWidget->windowIcon();
        m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
        return true;
      }
    }
    case QEvent::WindowStateChange:
    case QEvent::Resize:
      updateMaximize();
      return true;
    default:
      return QWidget::eventFilter(obj, event);
  }
  return QWidget::eventFilter(obj, event);
}

void TitleBar::onClicked() {
  QPushButton *pButton = qobject_cast<QPushButton *>(sender());
  QWidget *pWindow = this->getFrameWindow();
  if (pWindow->isTopLevel()) {
    if (pButton == m_pMinimizeButton) {
      pWindow->showMinimized();
    } else if (pButton == m_pMaximizeButton) {
      pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
    } else if (pButton == m_pCloseButton) {
      pWindow->close();
    }
  }
}

void TitleBar::onChangeChinese() {
  sender()->blockSignals(true);
  if (Config::getIns()->Get("main/lang").toString() == "zh") {
    return;
  } else {
    QMessageBox msgBox;
    msgBox.setWindowTitle("重启");
    msgBox.setText("你确认重启程序切换到中文吗?");
    QPushButton *yesButton = msgBox.addButton("确认", QMessageBox::AcceptRole);
    QPushButton *noButton = msgBox.addButton("取消", QMessageBox::RejectRole);
    msgBox.exec();
    if (msgBox.clickedButton() == yesButton) {
      Config::getIns()->Set("main", "lang", "zh");
      restartApplication();
    } else {
      m_pEnglishButton->setChecked(true);
    }
  }
  sender()->blockSignals(false);
}

void TitleBar::onChangeEnglish() {
  sender()->blockSignals(true);
  if (Config::getIns()->Get("main/lang").toString() == "en") {
    return;
  } else {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Restart");
    msgBox.setText("Do you want to switch the program to English and restart it?");
    QPushButton *yesButton = msgBox.addButton("Yes", QMessageBox::AcceptRole);
    QPushButton *noButton = msgBox.addButton("No", QMessageBox::RejectRole);
    msgBox.exec();
    if (msgBox.clickedButton() == yesButton) {
      Config::getIns()->Set("main", "lang", "en");
      restartApplication();
    } else {
      m_pChineseButton->setChecked(true);
    }
  }
  sender()->blockSignals(false);
}

void TitleBar::updateMaximize() {
  QWidget *pWindow = this->getFrameWindow();
  if (pWindow->isTopLevel()) {
    bool bMaximize = pWindow->isMaximized();
    if (bMaximize) {
      m_pMaximizeButton->setToolTip(tr("Restore"));
      m_pMaximizeButton->setProperty("maximizeProperty", true);
    } else {
      m_pMaximizeButton->setProperty("maximizeProperty", false);
      m_pMaximizeButton->setToolTip(tr("Maximize"));
    }

    m_pMaximizeButton->setStyle(QApplication::style());
  }
}
