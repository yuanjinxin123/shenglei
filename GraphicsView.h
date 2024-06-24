
#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QGraphicsView>
#include "mainwindow.h"

class GraphicsView : public QGraphicsView {
  Q_OBJECT
 public:
  explicit GraphicsView(QWidget *parent = nullptr);

 signals:

 protected:
  void resizeEvent(QResizeEvent *event) override;
  bool event(QEvent *event) override;
 private:
  MainWindow *w;
  QGraphicsScene *scene;
};

#endif // GRAPHICSVIEW_H
