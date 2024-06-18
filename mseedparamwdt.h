#ifndef MSEEDPARAMWDT_H
#define MSEEDPARAMWDT_H

#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVector>
#include <QWidget>

#include "mWidget.h"
#include "mportmanager.h"
namespace Ui {
class mSeedparamWdt;
}

class mSeedparamWdt : public mWidget {
  Q_OBJECT

 public:
  explicit mSeedparamWdt(QWidget *parent = nullptr);
  ~mSeedparamWdt();
  void init();

 protected:
  void updataparam(QString name, queryInfo info, int a);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);

 private slots:
  void on_mDl1SetBtn_clicked();

  void on_mDl2SetBtn_clicked();

  void on_mSemp3SetBtn_clicked();

 private:
  Ui::mSeedparamWdt *ui;
  QVector<QSpinBox *> mDlSet = {};
  QVector<QSpinBox *> mDlWork = {};
  QVector<QDoubleSpinBox *> mSempSet = {};

  QVector<QDoubleSpinBox *> mSempWork = {};
  bool mIsInit = true;
};

#endif  // MSEEDPARAMWDT_H
