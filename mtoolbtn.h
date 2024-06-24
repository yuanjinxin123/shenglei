#ifndef MTOOLBTN_H
#define MTOOLBTN_H

#include <QEvent>
#include <QObject>
#include <QToolButton>
class mToolBtn : public QToolButton {
  Q_OBJECT
 public:
  mToolBtn(QWidget *parent = nullptr);

  void setIcons(QStringView hover, QStringView img, QStringView press);

 protected:
  bool event(QEvent *event);
  QIcon mImg, mImgHover, mImgPressed;
};

#endif  // MTOOLBTN_H
