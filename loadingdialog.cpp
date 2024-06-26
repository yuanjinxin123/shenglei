#include "LoadingDialog.h"

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsDropShadowEffect>
#include "ui_loadingdialog.h"
LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoadingDialog) {
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
  initUi();
}

void LoadingDialog::initUi() {

  //加载Loading动画
  m_pLoadingMovie = new QMovie(":/img/load.gif");
  m_pLoadingMovie->setScaledSize(QSize(120, 120));
  ui->label_move->setMovie(m_pLoadingMovie);
  m_pLoadingMovie->start();

  //取消按钮
  connect(ui->pushButton_cance, &QPushButton::clicked, this,
          &LoadingDialog::cancelBtnClicked);

  //实例阴影shadow
  setAttribute(Qt::WA_TranslucentBackground, true);
  QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
  shadowEffect->setBlurRadius(15);
  shadowEffect->setOffset(0, 0);
  shadowEffect->setColor(QColor(0, 0, 0, 160));
  setGraphicsEffect(shadowEffect);

}

/**
 * @brief LoadingDialog::setCanCancel 设置是够允许用户点击取消等待按钮
 * @param bCanCancel 是够允许
 */
void LoadingDialog::setCanCancel(bool bCanCancel) {
  ui->pushButton_cance->setEnabled(bCanCancel);
}

void LoadingDialog::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect rect = this->rect();
  int radius = 10;
  QPainterPath path;
  path.addRoundedRect(rect, radius, radius);
  painter.fillPath(path, QBrush(QColor(199, 212, 227)));
  QDialog::paintEvent(event);
}


/**
 * @brief LoadingDialog::cancelBtnClicked 取消按钮槽函数
 */
void LoadingDialog::cancelBtnClicked() {
  emit cancelWaiting();
  this->done(USER_CANCEL);
}

/**
 * @brief LoadingDialog::paintEvent 界面绘制
 * @param event
 */

LoadingDialog::~LoadingDialog() {
  delete m_pLoadingMovie;
}
