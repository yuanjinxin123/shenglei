
#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QGraphicsView>
#include "mainwindow.h"
#include "singleton.h"
#include "trayicon.h"

class GraphicsView : public QGraphicsView, public Singleton<GraphicsView> {
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
  TrayIcon m_trayIcon;
};

#endif // GRAPHICSVIEW_H
