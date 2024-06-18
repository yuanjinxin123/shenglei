#include "mlogindlg.h"

#include "qpainter.h"
#include "ui_mlogindlg.h"

#include <QListView>
Q_GLOBAL_STATIC(mLoginDlg, g_loginDlg);
mLoginDlg::mLoginDlg(QWidget *parent) : QDialog(parent), ui(new Ui::mLoginDlg) {
  ui->setupUi(this);
  Qt::WindowFlags flags = Qt::Dialog;
  flags |= Qt::WindowCloseButtonHint;
  setWindowFlags(flags);
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
  ui->mUser->setStyleSheet("QComboBox QAbstractItemView::item{height:60px;} QAbstractItemView{font-size:16pt;}");

  ui->mUser->setCurrentIndex(-1);
  ui->mUser->addItem("engineer", "engineer");
  ui->mUser->addItem("administrator", "administrator");

  ui->mPwd->setFocus();
  ui->mCancel->setVisible(false);
  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);
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

void mLoginDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width()-1);
    rect.setHeight(rect.height()-1);
    painter.drawRoundedRect(rect, 50, 50);

    QDialog::paintEvent(event);
}

void mLoginDlg::on_mOk_clicked() { accept(); }

void mLoginDlg::on_mCancel_clicked() { reject(); }

void mLoginDlg::on_btn_close_clicked()
{
  this->close();
}

