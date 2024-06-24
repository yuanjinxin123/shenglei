#include "customcheckbox.h"

void CustomCheckBox::mousePressEvent(QMouseEvent *event) {
  // 拦截鼠标按下事件，不调用基类的mousePressEvent
  event->accept();
}

void CustomCheckBox::mouseReleaseEvent(QMouseEvent *event) {
  // 拦截鼠标释放事件，不调用基类的mouseReleaseEvent
  event->accept();
  emit customClicked(!isChecked());
}
