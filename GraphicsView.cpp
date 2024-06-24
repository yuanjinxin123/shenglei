
#include "GraphicsView.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

#include <QLineEdit>

GraphicsView::GraphicsView(QWidget *parent)
  : QGraphicsView{parent} {
  setWindowFlags(Qt::FramelessWindowHint | windowFlags());
  setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明，可选
  // 关闭水平和垂直滚动条
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  int taskbarHeight = 30;
  // 获取主屏幕
  QScreen *screen = QApplication::primaryScreen();
  if (screen) {
    // 获取桌面尺寸
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    // 获取任务栏的高度
    taskbarHeight = screen->geometry().bottom() - desktopRect.bottom();
  }
  //resize(1024,768-taskbarHeight);
  //viewport()->resize(1024, 768-taskbarHeight);

  w = new MainWindow(this);
  scene = new QGraphicsScene(this);
  QGraphicsProxyWidget *proxy = scene->addWidget(w);
  this->setScene(scene);

  QSize screenSize = screen->size();
  QSize minSize = screenSize * 0.6;
  this->setMinimumSize(minSize);
  this->resize(minSize);

  // 获取屏幕的可用几何尺寸（考虑任务栏等）
  QRect availableGeometry = screen->availableGeometry();
  this->move((availableGeometry.width() - this->width()) / 2 + availableGeometry.left(),
             (availableGeometry.height() - this->height()) / 2 + availableGeometry.top());
}

void GraphicsView::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);

  // 获取视图和场景的大小
  QSizeF viewSize = this->viewport()->size();
  QSizeF sceneSize = scene->sceneRect().size();

  // 计算缩放因子
  qreal scaleX = viewSize.width() / sceneSize.width();
  qreal scaleY = viewSize.height() / sceneSize.height();
  // qreal scale = qMin(scaleX, scaleY);

  // 设置缩放因子
  this->setTransform(QTransform::fromScale(scaleX, scaleY));

//    scale(ratioX, ratioY);
}

bool GraphicsView::event(QEvent *event) {
  return QGraphicsView::event(event);
}

