//#pragma execution_character_set("utf-8")
#include "LoadingDialog.h"

#include <QPainter>
#include <QStyleOption>
LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent) {
  //如果需要显示任务栏对话框则删除Qt::Tool
  setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setObjectName("LoadingDialog");
  initUi();
}

/**
 * @brief LoadingDialog::initUi UI元素初始化
 */
void LoadingDialog::initUi() {
  this->setFixedSize(620, 400);
  m_pCenterFrame = new QFrame(this);
  m_pCenterFrame->setGeometry(200, 50, 230, 330);

  //加载Loading动画
  m_pLoadingMovie = new QMovie(":/img/load.gif");
  m_pLoadingMovie->setScaledSize(QSize(120, 120));
  m_pMovieLabel = new QLabel(m_pCenterFrame);
  m_pMovieLabel->setGeometry(55, 10, 120, 120);
  m_pMovieLabel->setScaledContents(true);
  m_pMovieLabel->setMovie(m_pLoadingMovie);
  m_pLoadingMovie->start();

  //提示文本
  m_pTipsLabel = new QLabel(m_pCenterFrame);
  m_pTipsLabel->setGeometry(5, 160, 220, 35);
  m_pTipsLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  m_pTipsLabel->setObjectName("tipslabel");
  m_pTipsLabel->setText(tr("loading,please wait"));
  /*  m_pTipsLabel->setStyleSheet(
        "QLabel#tips{font-family:\"Microsoft YaHei\";font-size: 15px;color: "
        "#413d8f;}")*/
  ;

  //取消按钮
  m_pCancelBtn = new QPushButton(m_pCenterFrame);
  m_pCancelBtn->setObjectName("load_cancelBtn");
  m_pCancelBtn->setText(tr("Cancel waiting"));
  // m_pCancelBtn->setStyleSheet(
  //    "QPushButton#cancelBtn{"
  //    "background-color: #edeef6;"
  //    "border-radius: 4px;"
  //    "font-family: \"Microsoft YaHei\";"
  //    "font-size: 14px;"
  //    "color: #333333;"
  //    "}"
  //    "QPushButton#cancelBtn::hover{"
  //    "background:#dcdeea"
  //    "}");
  m_pCancelBtn->setGeometry(25, 250, 180, 50);
  m_pCancelBtn->setEnabled(true);
  connect(m_pCancelBtn, &QPushButton::clicked, this,
          &LoadingDialog::cancelBtnClicked);

  //实例阴影shadow
  QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
  shadow->setOffset(0, 0);
  shadow->setColor(QColor(32, 101, 165));
  shadow->setBlurRadius(10);
  this->setGraphicsEffect(shadow);

  setAutoFillBackground(true);

  QPalette pal(palette());

  pal.setColor(QPalette::Background, Qt::black);  //设置背景黑色

  setPalette(pal);
}

/**
 * @brief LoadingDialog::setTipsText 设置提示文本
 * @param strTipsText 提示文本
 */
void LoadingDialog::setTipsText(QString strTipsText) {
  m_pTipsLabel->setText(strTipsText);
}

/**
 * @brief LoadingDialog::setCanCancel 设置是够允许用户点击取消等待按钮
 * @param bCanCancel 是够允许
 */
void LoadingDialog::setCanCancel(bool bCanCancel) {
  m_pCancelBtn->setEnabled(bCanCancel);
}

/**
 * @brief LoadingDialog::moveToCenter 移动对话框到指定窗口中间
 * @param pParent 指定窗口指针
 */
void LoadingDialog::moveToCenter(QWidget *pParent) {
  if (pParent != nullptr && pParent != NULL) {
    int nParentWidth = pParent->width();
    int nParentHeigth = pParent->height();

    int nWidth = this->width();
    int nHeight = this->height();

    int nParentX = pParent->x();
    int nParentY = pParent->y();

    int x = (nParentX + (nParentWidth - nWidth) / 2);
    int y = (nParentY + (nParentHeigth - nHeight) / 2);

    this->move(x, y);
  }
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
void LoadingDialog::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);  //反锯齿
  painter.setBrush(QBrush(Qt::white));
  painter.setPen(Qt::transparent);
  QRect rect = this->rect();
  rect.setLeft(9);
  rect.setTop(9);
  rect.setWidth(rect.width() - 9);
  rect.setHeight(rect.height() - 9);
  painter.drawRoundedRect(rect, 8, 8);

  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  QWidget::paintEvent(event);
}

LoadingDialog::~LoadingDialog() {
  delete m_pLoadingMovie;
  delete m_pMovieLabel;
  delete m_pTipsLabel;
  delete m_pCancelBtn;
  delete m_pCenterFrame;
}
