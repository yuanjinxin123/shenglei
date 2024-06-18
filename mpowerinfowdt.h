#ifndef MPOWERINFOWDT_H
#define MPOWERINFOWDT_H

#include <qstring.h>

#include <QDoubleSpinBox>
#include <QHash>
#include <QVBoxLayout>
#include <QWidget>

#include "mportmanager.h"

namespace Ui {
class mPowerInfoWdt;
}

class mPowerInfoWdt : public QWidget {
  Q_OBJECT

 public:
  explicit mPowerInfoWdt(QWidget *parent = nullptr);
  ~mPowerInfoWdt();
  // void layoutInit();

 protected:
  void init();

  void layoutInit(queryInfo info);

  void updataparam(QString name, queryInfo info, int a);
  // bool eventFilter(QObject *watched, QEvent *event);
  void clearWidgets(QLayout *layout);
  void clearLayout(QLayout *layout);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);

 private:
  void releaseItems();

 private:
  Ui::mPowerInfoWdt *ui;
  double mMin;
  double mMax;
  QHash<QString, QDoubleSpinBox *> mValItems;
  //QVBoxLayout *mLay;
  QGridLayout *mLay;
};

#endif  // MPOWERINFOWDT_H
