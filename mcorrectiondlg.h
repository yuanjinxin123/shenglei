#ifndef MCORRECTIONDLG_H
#define MCORRECTIONDLG_H

#include <QDialog>
#include <QEventLoop>

#include "mportmanager.h"
namespace Ui {
class mCorrectionDlg;
}

class mCorrectionDlg : public QDialog {
  Q_OBJECT

 public:
  explicit mCorrectionDlg(QWidget *parent = nullptr);
  ~mCorrectionDlg();
  void closeEvent(QCloseEvent *e);
  void showDlg();
 signals:
  void dlg_exit();
  void stopCorr();
 private slots:
  void receiveQuery(QString name, queryInfo info, int a);
  void on_mOpBtn_clicked();

  void on_mCacel_clicked();

 private:
  void init();

 private:
  Ui::mCorrectionDlg *ui;
  uint16_t mStep = 0;
  QEventLoop loop;
};

#endif  // MCORRECTIONDLG_H
