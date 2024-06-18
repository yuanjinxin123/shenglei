#ifndef MFUCWDT_H
#define MFUCWDT_H

#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mFucWdt;
}

class mFucWdt : public QWidget {
  Q_OBJECT

 public:
  explicit mFucWdt(QWidget *parent = nullptr);
  ~mFucWdt();
  void updatePixmap(const QString &img);

 protected:
  void paintEvent(QPaintEvent *event);
  void updataStatus();

 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);
 private slots:
  void on_mSwitchBtn_clicked();

  void on_mResetWarnBtn_clicked();
  void DisPortConnect(QString name);

 private:
  Ui::mFucWdt *ui;
  QImage mImg;
  uint32_t mPin1;
  uint32_t mPin2;
  bool isGetData = false;
  bool mIsValid;
  bool mIsRun = false;
};

#endif  // MFUCWDT_H
