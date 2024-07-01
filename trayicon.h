#pragma once

#include <QWidget>
#include <QSystemTrayIcon>

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
  void ChangeZh();
  void ChangeEn();

 private:
  void CreateActions();
 private:
  QSystemTrayIcon *m_pTrayIcon;
};
