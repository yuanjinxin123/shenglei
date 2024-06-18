#ifndef MFRISTPAGE_H
#define MFRISTPAGE_H

#include <QString>
#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mFristPage;
}

class mFristPage : public QWidget {
  Q_OBJECT

 public:
  explicit mFristPage(QWidget *parent = nullptr);
  ~mFristPage();

 protected:
  QImage _image;
  bool init();
  bool updataHome(QStringView cmd, const queryInfo &info, int a);
  void resizeEvent(QResizeEvent *event);
  void paintEvent(QPaintEvent *event) override;
 protected slots:
  void receiveData(QString, queryInfo);
  void receiveQuery(QString, queryInfo, int);
  void changeRose(uint8_t id);

 private:
  Ui::mFristPage *ui;
  QString mCmd;
};

#endif  // MFRISTPAGE_H
