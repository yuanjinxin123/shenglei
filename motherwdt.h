#ifndef MOTHERWDT_H
#define MOTHERWDT_H

#include <QButtonGroup>
#include <QWidget>

#include "mportmanager.h"
#include "mwidget.h"
namespace Ui {
class mOtherWdt;
}

class mOtherWdt : public mWidget {
  Q_OBJECT

 public:
  explicit mOtherWdt(QWidget *parent = nullptr);
  ~mOtherWdt();

 protected:
  void updataparam(QString name, queryInfo info, int a);
  void init();
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);
  void updateDw(int);
 private slots:
  void on_mChangePointBtn_clicked();

  void on_mCorrectSwitch_clicked(bool checked);

  void on_mDangwei_clicked(bool checked);

  void on_mCorrectSwitch_clicked();

 private:
  Ui::mOtherWdt *ui;
  QButtonGroup *pDWGroup;
};

#endif  // MOTHERWDT_H
