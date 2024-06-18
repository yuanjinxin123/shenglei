#ifndef MSETTINGDLG_H
#define MSETTINGDLG_H

#include <QDialog>

#include "mserial.h"
#include "portcfg.h"
namespace Ui {
class msettingdlg;
}

class msettingdlg : public QDialog {
  Q_OBJECT

 public:
  explicit msettingdlg(QWidget *parent = nullptr);
  ~msettingdlg();
  bool setOpenCom(mSerial *serial);
  bool getOpenCom(mSerial *serial);
 private slots:
  void on_mSerial_clicked();

  void on_mDbCheck_clicked();
  void on_enable_mDbCheck(bool isenable);

 private:
  Ui::msettingdlg *ui;
};

#endif  // MSETTINGDLG_H
