#ifndef MPOWERCHAGE_H
#define MPOWERCHAGE_H

#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mPowerChage;
}

class mPowerChage : public QWidget {
  Q_OBJECT

 public:
  explicit mPowerChage(QWidget *parent = nullptr);
  ~mPowerChage();
  // bool eventFilter(QObject *obj, QEvent *event);

 protected:
  bool updataHome(QStringView name, const queryInfo &info, int a);
  bool init();

 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);
 private slots:

  void on_mTable_cellActivated(int row, int column);

  void on_mPowerVal_editingFinished();

  void on_mMnOrSzBtn_clicked(bool checked);

  void on_mPowerBtn_clicked(bool checked);

  void on_mPowerVal_valueChanged(double arg1);

  void on_mCurePower_valueChanged(double arg1);

  void on_mBurstVal_valueChanged(int arg1);

  void on_mFreqVal_valueChanged(int arg1);

 private:
  Ui::mPowerChage *ui;
  bool mS = false;
  bool mIsChange = false;
};

#endif  // MPOWERCHAGE_H
