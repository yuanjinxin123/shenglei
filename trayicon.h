#pragma once

#include <QWidget>
#include "mdatecheckdlg.h"
#include <QSystemTrayIcon>
#include <QMap>

class TrayIcon : public QWidget {
  Q_OBJECT

 public:
  TrayIcon(QWidget *parent = 0);
  ~TrayIcon();
 public:

 private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void DoQuit();
  void ShowAbout();
  void ShowHisData();
  void ChangeZh();
  void ChangeEn();
  void onIconActivated();
  void on_alarmSn(QString sn, int alarm_num);

 private:
  void CreateActions();
 private:
  QSystemTrayIcon *m_pTrayIcon;
  QAction *m_PuerryData;
  mDateCheckDlg *m_pDlg;
};
