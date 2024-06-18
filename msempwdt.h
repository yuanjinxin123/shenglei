#ifndef MSEMPWDT_H
#define MSEMPWDT_H

#include <QWidget>

#include "mportmanager.h"
#include "mwidget.h"
namespace Ui {
class msempwdt;
}

class msempwdt : public mWidget {
  Q_OBJECT

 public:
  explicit msempwdt(mWidget *parent = nullptr);
  ~msempwdt();

 protected:
  void init();
  // void paintEvent(QPaintEvent *event);
  void updataparam(QString name, queryInfo info, int a);
  bool eventFilter(QObject *obj, QEvent *event);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);

  void changeRose(uint8_t id);

 private slots:
  void on_mSempSetBtn_clicked();
  void on_mResetBtn_clicked();

  void on_mSHGWorkVal_3_valueChanged(double arg1);

  void on_mTHGWorkVal_3_valueChanged(double arg1);

 private:
  Ui::msempwdt *ui;
  bool mReflush = false;
  bool mTHGChange = false;
  bool mSHGChange = false;
};

#endif  // MSEMPWDT_H
