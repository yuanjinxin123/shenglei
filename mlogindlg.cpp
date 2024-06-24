#include "mlogindlg.h"

#include "qpainter.h"
#include "ui_mlogindlg.h"
#include <QScreen>
#include <QListView>
#include <QGraphicsDropShadowEffect>

Q_GLOBAL_STATIC(mLoginDlg, g_loginDlg);
mLoginDlg::mLoginDlg(QWidget *parent) : QDialog(parent), ui(new Ui::mLoginDlg) {
  ui->setupUi(this);
#ifdef DXJG_SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#else
  setWindowIcon(QIcon(":/img/logo_t.png"));
#endif
  // QStringList l;
  // l << tr("engineer") << tr("administrator");
  // ui->mUser->addItems(l);
  // ui->mUser
  // ui->mUser->setText("engineer");
  ui->mUser->setView(new QListView());
  ui->mUser->setStyleSheet("QComboBox QAbstractItemView::item{height:40px;} QAbstractItemView{font-size:16pt;}");

  ui->mUser->setCurrentIndex(-1);
  ui->mUser->addItem("engineer", "engineer");
  ui->mUser->addItem("administrator", "administrator");

  ui->mPwd->setFocus();
  ui->mCancel->setVisible(false);
  setWindowFlags(Qt::FramelessWindowHint);

  //ÊµÀýÒõÓ°shadow
  setAttribute(Qt::WA_TranslucentBackground, true);
  QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
  shadowEffect->setBlurRadius(15);
  shadowEffect->setOffset(0, 0);
  shadowEffect->setColor(QColor(0, 0, 0, 160));
  setGraphicsEffect(shadowEffect);
  QScreen *screen = QGuiApplication::primaryScreen();
  qreal scaleFactor = screen->devicePixelRatio();
  resize(size() / scaleFactor);
}

mLoginDlg::~mLoginDlg() { delete ui; }

mLoginDlg *mLoginDlg::getIns() { return g_loginDlg; }

QString mLoginDlg::user() { return ui->mUser->currentText(); }

QString mLoginDlg::pwd() { return ui->mPwd->text(); }

uint8_t mLoginDlg::logRole() { return mLogRole; }

void mLoginDlg::setLogRole(uint8_t role) {
  mLogRole = role % 3;
  emit logRoleChanged(mLogRole);
}

void mLoginDlg::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QRect rect = this->rect();
  int radius = 10;
  QPainterPath path;
  path.addRoundedRect(rect, radius, radius);
  painter.fillPath(path, QBrush(QColor(190, 210, 224)));

  QDialog::paintEvent(event);
}

void mLoginDlg::on_mOk_clicked() { accept(); }

void mLoginDlg::on_mCancel_clicked() { reject(); }

void mLoginDlg::on_btn_close_clicked() {
  this->close();
}

