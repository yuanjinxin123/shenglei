#ifndef MELETDLGADMIN_H
#define MELETDLGADMIN_H

#include <qcheckbox.h>

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QVector>

#include "mportmanager.h"
namespace Ui {
class mEletDlgAdmin;
}

class mEletDlgAdmin : public QDialog {
  Q_OBJECT

 public:
  explicit mEletDlgAdmin(QWidget* parent = nullptr);
  ~mEletDlgAdmin();

 protected:
  bool init();
  void paintEvent(QPaintEvent* event);
  bool updataHome(QStringView name, const queryInfo& info, int a);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);
 private slots:
  void on_mElcOk_clicked();

  void on_mResetBtn_2_clicked();

  void on_mElecEdt1_clicked(bool checked);

  void on_mElecEdt2_clicked(bool checked);

  void on_mElecEdt3_clicked(bool checked);

  void on_mElecEdt4_clicked(bool checked);

  void on_mElecEdt5_clicked(bool checked);

 private:
  Ui::mEletDlgAdmin* ui;
  bool mStatus = false;
  bool mReflush = false;
  QVector<QCheckBox*> mStatusEdt;
  QVector<QDoubleSpinBox*> mSpin1;
  QVector<QDoubleSpinBox*> mSpin2;
  QVector<QDoubleSpinBox*> mSpin3;
};

#endif  // MELETDLGADMIN_H
