#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qimage.h>

#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMovie>
#include <atomic>

#include "QSerialPortInfo"
#include "mconnectdlg.h"
#include "mportmanager.h"
#include "mserial.h"
#include "qserialport.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 protected:
  void init();
  void initData();
  void setWarnStatus(int s, int err);
  void resizeEvent(QResizeEvent*);
  virtual void timerEvent(QTimerEvent* event);
  bool eventFilter(QObject* obj, QEvent* event);
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
 private:
  Ui::MainWindow* ui;
  bool mopenflage = false;
  mSerial* pSerial;
  QImage _image;
  QMenu* mMenuLogin;
  QAction* mQSql;
  QLabel* mWarmLbl;
  QMovie mGgif;
  QLabel* mSeedStatus;
  std::atomic_bool mPowerEditing;
  std::atomic_bool isChange = false;
  bool mJKStatus = true;
  uint16_t mPowerVal;
  bool isGetData = false;
  uint32_t mPin1;
  uint32_t mPin2;
  int m_nTimerID = -1;
  QLabel* mhverLabel;
  QLabel* mPowerLabel;
  QLabel* mRuntime;
  QLabel* mJgTime;
  QString mHver;
  uint8_t mIsPod = 0;
  bool misLock = false;
  bool mPowerChange = false;
  QTimer mValitTimer;
  QTimer *mTimeTimer;
  uint32_t mVtime = 8000;
  bool mIsConnect = false;
  QLabel *stateLight;
};
#endif  // MAINWINDOW_H
