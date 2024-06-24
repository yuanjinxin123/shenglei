#include "mwidget.h"

#include <QPainter>
#include <QStyleOption>
mWidget::mWidget(QWidget *parent) : QWidget {parent} {}

void mWidget::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
