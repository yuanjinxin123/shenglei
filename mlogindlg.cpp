#include "mlogindlg.h"

#include "qpainter.h"
#include "ui_mlogindlg.h"
#include <QScreen>
#include <QListView>
#include <QGraphicsDropShadowEffect>
#include "define.h"

Q_GLOBAL_STATIC(mLoginDlg, g_loginDlg);
mLoginDlg::mLoginDlg(QWidget *parent) : QDialog(parent), ui(new Ui::mLoginDlg) {
  ui->setupUi(this);
#ifdef SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#elif defined(SHENGLEI)
  setWindowIcon(QIcon(":/img/logo_t.png"));
#else

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
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
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


void mLoginDlg::on_mOk_clicked() { accept(); }

void mLoginDlg::on_mCancel_clicked() { reject(); }



