#ifndef MLOGINDLG_H
#define MLOGINDLG_H

#include <QDialog>

namespace Ui {
class mLoginDlg;
}

class mLoginDlg : public QDialog {
  Q_OBJECT
  Q_PROPERTY(
    uint8_t mLogRole READ logRole WRITE setLogRole NOTIFY logRoleChanged)

 public:
  explicit mLoginDlg(QWidget *parent = nullptr);
  ~mLoginDlg();
  static mLoginDlg *getIns();
  QString user();
  QString pwd();
  uint8_t logRole();
  void setLogRole(uint8_t role);

 signals:
  void logRoleChanged(uint8_t);
 private slots:
  void on_mOk_clicked();

  void on_mCancel_clicked();

 private:
  uint8_t mLogRole = 0;

 private:
  Ui::mLoginDlg *ui;
};
#define loginDlg mLoginDlg::getIns()
#endif  // MLOGINDLG_H
