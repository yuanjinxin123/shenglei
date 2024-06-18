#ifndef MWIDGET_H
#define MWIDGET_H

#include <QWidget>

class mWidget : public QWidget {
  Q_OBJECT
 public:
  explicit mWidget(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event);
 signals:
};

#endif  // MWIDGET_H
