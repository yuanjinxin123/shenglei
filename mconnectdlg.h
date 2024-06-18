#ifndef MCONNECTDLG_H
#define MCONNECTDLG_H

#include <QDialog>

#include "loadingdialog.h"
namespace Ui {
class mconnectDlg;
}

class mconnectDlg : public QDialog {
  Q_OBJECT

 public:
  explicit mconnectDlg(QWidget *parent = nullptr);
  ~mconnectDlg();
  bool tryConnectCom(uint8_t &num);
 private slots:
  void on_mConnectBtn_clicked();

  void on_mCloseBtn_clicked();

  void on_mComBox_currentIndexChanged(int index);

 private:
  Ui::mconnectDlg *ui;
  LoadingDialog mLoadDlg;
};

#endif  // MCONNECTDLG_H
