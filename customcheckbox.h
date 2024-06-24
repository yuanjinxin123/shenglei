#ifndef CUSTOMCHECKBOX_H
#define CUSTOMCHECKBOX_H

#include <QCheckBox>
#include <QMouseEvent>

class CustomCheckBox : public QCheckBox {
  Q_OBJECT
 public:
  using QCheckBox::QCheckBox;
 signals:
  void customClicked(bool checked);
 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CUSTOMCHECKBOX_H
