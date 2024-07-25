#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qimage.h>

#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMovie>
#include <atomic>
#include <QVBoxLayout>
#include <QRadioButton>
#include "QSerialPortInfo"
#include "mconnectdlg.h"
#include "mportmanager.h"
#include "mserial.h"
#include "qserialport.h"
#include "tcpclient.h"
#include <QButtonGroup>


class ButtonWidget : public QWidget {
 public:
  ButtonWidget(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    setLayout(layout);
  }

  void addButton(const QString &text, bool checked) {
    QRadioButton *button = new QRadioButton(text, this);
    button->setMinimumHeight(35);
    button->setStyleSheet("QRadioButton {"
                          "border-image: url(:/img/button_normal.png);"
                          "font: 13pt;"
                          "color: rgb(98, 98, 98);"
                          "padding-left: 4px;"
                          "padding-right: 4px;"
                          "}");
    button->setChecked(checked);
    layout()->addWidget(button);
    radioButtons.append(button);
    connect(button, &QRadioButton::toggled, mportManager::instance(), &mportManager::setCurrentSn);
  }

  void removeRadioButton(const QString &text) {
    for (auto it = radioButtons.begin(); it != radioButtons.end(); ++it) {
      if ((*it)->text() == text) {
        QRadioButton *radioButton = *it;
        layout()->removeWidget(radioButton);
        radioButtons.erase(it);
        delete radioButton;
        return;
      }
    }
  }

  void alarmRadioButtons(const QString &text) {
    for (auto it = radioButtons.begin(); it != radioButtons.end(); ++it) {
      if ((*it)->text() == text) {
        QRadioButton *radioButton = *it;
        radioButton->setStyleSheet("QRadioButton {"
                                   "border-image: url(:/img/button_alarm.png);"
                                   "font: 13pt;"
                                   "color: rgb(200, 200, 200);"
                                   "padding-left: 4px;"
                                   "padding-right: 4px;"
                                   "}");
        return;
      }
    }
  }
  void clearRadioButtons() {
    while (!radioButtons.isEmpty()) {
      QRadioButton *radioButton = radioButtons.takeFirst();
      layout()->removeWidget(radioButton);
      delete radioButton;
    }
  }
 private:
  QList<QRadioButton *> radioButtons;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void init();
  void initData();
  void setWarnStatus(int s, int err);
  void resizeEvent(QResizeEvent *);
  virtual void timerEvent(QTimerEvent *event);
  bool eventFilter(QObject *obj, QEvent *event);
  void visibleNetCtl(bool);
 protected slots:
  void changePsoOrPod(uint8_t id);
  void changeConnectIcon(bool isok);
 private slots:
  void on_MsetBtn_clicked();
  void ReadSerial();

  void on_mLoginBtn_clicked();
  void actionsSlot();
  void checkDlg();
  void receiveQuery(QString name, queryInfo info, int a);
  void showMsg(QString msg);

  void on_mCntBtn_clicked();
  void ValitTimeout();
  void on_mSetBtn_clicked();

  void on_mOk_clicked();
  void DisPortConnect(QString name);

  // void on_mReset_clicked();
  void PowerChange(int d);
  void PowerFinish();

  void on_mReset_clicked();

  void on_mResetBtn_clicked();

  void on_mCorr_clicked();

  void showTime();
  void on_SnUpdate(QString sn, bool add, bool curr);
  void on_alarmSn(QString sn, int alarm_num);
 signals:
  void sendClearSn();
 private:
  Ui::MainWindow *ui;
  bool mopenflage = false;
  mSerial *pSerial;
  QImage _image;
  QMenu *mMenuLogin;
  QAction *mQSql;
  QLabel *mWarmLbl;
  QMovie mGgif;
  QLabel *mSeedStatus;
  std::atomic_bool mPowerEditing;
  std::atomic_bool isChange = false;
  bool mJKStatus = true;
  uint16_t mPowerVal;
  bool isGetData = false;
  uint32_t mPin1;
  uint32_t mPin2;
  int m_nTimerID = -1;
  QLabel *mhverLabel;
  QLabel *mPowerLabel;
  QLabel *mRuntime;
  QLabel *mJgTime;
  QString mHver;
  uint8_t mIsPod = 0;
  bool misLock = false;
  bool mPowerChange = false;
  QTimer mValitTimer;
  QTimer *mTimeTimer;
  uint32_t mVtime = 8000;
  bool mIsConnect = false;
  QLabel *stateLight;

  TCPClient *m_tcpClient;
  ButtonWidget *mPButtonWidget;
  QButtonGroup *pSnGroup;
};
#endif  // MAINWINDOW_H
