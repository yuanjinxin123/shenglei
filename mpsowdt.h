#ifndef MPSOWDT_H
#define MPSOWDT_H
#include <QWidget>

#include "mportmanager.h"

namespace Ui {
class mPsoWdt;
}

class mPsoWdt : public QWidget {
  Q_OBJECT

 public:
  explicit mPsoWdt(QWidget *parent = nullptr);
  ~mPsoWdt();
 public slots:
  void receiveQuery(QString name, queryInfo info, int a);
 protected slots:
  void spinFinished();

 protected:
  bool updataHome(QStringView name, const queryInfo &info, int a);
  bool init();

  void paintEvent(QPaintEvent *event);
 protected slots:
  void changeRose(uint8_t id);

 private slots:
  void on_mYs1Edit_valueChanged(int arg1);

 private:
  Ui::mPsoWdt *ui;
};

#endif  // MPSOWDT_H
