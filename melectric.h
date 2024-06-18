#ifndef MELECTRIC_H
#define MELECTRIC_H

#include <qlineedit.h>

#include <QCheckBox>
#include <QVector>
#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mElectric;
}

class mElectric : public QWidget {
  Q_OBJECT

 public:
  explicit mElectric(QWidget *parent = nullptr);
  ~mElectric();
  bool init();

 protected:
  bool updataHome(QStringView name, const queryInfo &info, int a);
  void paintEvent(QPaintEvent *event);
 private slots:
  void on_pushButton_clicked();
  void receiveQuery(QString, queryInfo, int);
  void changeRose(uint8_t);
  void on_mSetBtn_clicked();

 private:
  Ui::mElectric *ui;
  QVector<QCheckBox *> mEditVer1;
  QVector<QLineEdit *> mEditVer2;
  QVector<QLineEdit *> mEditVer3;

  QVector<QLineEdit *> mEditVer11;
  QVector<QLineEdit *> mEditVer22;
  QVector<QLineEdit *> mEditVer33;
  bool mStatus = false;
};

#endif  // MELECTRIC_H
