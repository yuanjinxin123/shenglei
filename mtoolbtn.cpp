#include "mtoolbtn.h"

mToolBtn::mToolBtn(QWidget *parent) : QToolButton(parent) {}
void mToolBtn::setIcons(QStringView hover, QStringView img, QStringView press) {
  mImg = QIcon(img.toString());
  mImgHover = QIcon(hover.toString());
  mImgPressed = QIcon(press.toString());
  setIcon(mImg);
}
bool mToolBtn::event(QEvent *pevent) {
  switch (pevent->type()) {
    case QEvent::MouseButtonRelease:
    case QEvent::Enter:
      setIcon(mImgHover);
      break;
    case QEvent::Leave:
      setIcon(mImg);
      break;
    case QEvent::MouseButtonPress:
      setIcon(mImgPressed);

      break;
    default:
      break;
  }
  return QToolButton::event(pevent);
}
