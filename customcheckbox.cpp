#include "customcheckbox.h"

void CustomCheckBox::mousePressEvent(QMouseEvent *event) {
  // ������갴���¼��������û����mousePressEvent
  event->accept();
}

void CustomCheckBox::mouseReleaseEvent(QMouseEvent *event) {
  // ��������ͷ��¼��������û����mouseReleaseEvent
  event->accept();
  emit customClicked(!isChecked());
}
