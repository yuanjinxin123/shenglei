#ifndef DLG_ACT_H
#define DLG_ACT_H

#include <QDialog>

#include "mportmanager.h"
namespace Ui {
class dlg_act;
}

class dlg_act : public QDialog {
  Q_OBJECT

 public:
  explicit dlg_act(QWidget *parent = nullptr);
  ~dlg_act();

 private slots:
  void on_btn_pin1_clicked();

  void on_pin_btn2_clicked();

  void on_pin_btn3_clicked();

  void on_close_btn_clicked();
  void receive_pin_status(QString, cmdData);

 private:
  Ui::dlg_act *ui;
};

#endif  // DLG_ACT_H
