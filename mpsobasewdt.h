#ifndef MPSOBASEWDT_H
#define MPSOBASEWDT_H
#include <qlineedit.h>
#include <qspinbox.h>
#include <qvector.h>

#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mPsoBaseWdt;
}

class mPsoBaseWdt : public QWidget {
  Q_OBJECT

 public:
  explicit mPsoBaseWdt(QWidget *parent = nullptr);
  ~mPsoBaseWdt();

 public slots:
  void receiveQuery(QString name, queryInfo info, int a);
 protected slots:
  void spinFinish();

 protected:
  bool updataHome(QStringView name, const queryInfo &info, int a);
  bool init();

 private:
  Ui::mPsoBaseWdt *ui;
  QVector<QSpinBox *> mEditVec;
};

#endif  // MPSOBASEWDT_H
