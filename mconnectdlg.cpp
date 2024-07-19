#include "mconnectdlg.h"

#include <QMessageBox>

#include "mportmanager.h"
#include "portcfg.h"
#include "ui_mconnectdlg.h"
#include "define.h"
mconnectDlg::mconnectDlg(QWidget *parent)
  : QDialog(parent), ui(new Ui::mconnectDlg) {
  ui->setupUi(this);
  Qt::WindowFlags flags = Qt::Dialog;

  flags |= Qt::WindowCloseButtonHint;
  setWindowFlags(flags);
#ifdef SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#elif defined(SHENGLEI)
  setWindowIcon(QIcon(":/img/logo_t.png"));
#else

#endif
  setWindowTitle(tr("connect"));
  connect(&mLoadDlg, &LoadingDialog::cancelWaiting,
  [&]() { mportMg->setCancelTry(); });
}

mconnectDlg::~mconnectDlg() { delete ui; }

bool mconnectDlg::tryConnectCom(uint8_t &num) {
  QVector<QString> coms;
  QMap<QString, QString> sn;
#if 0

  if (!mportMg->tryConnect(coms, sn) || coms.isEmpty()) {
    QMessageBox::warning(this, tr("Waring"), tr("Not find coms"),
                         QMessageBox::StandardButton::Ok);

    return false;
  }
  for (auto i = 0; i < coms.size(); i++) {
    ui->mComBox->addItem(coms[i]);
  }
  num = coms.size();
  return false;
#else

  QVector<com_param> p;
  mportMg->close();
  mLoadDlg.show();
  if (!mportMg->tryConnect(coms, sn) || coms.isEmpty()) {
    mLoadDlg.close();
    QMessageBox::warning(this, tr("Waring"), tr("Not find coms"),
                         QMessageBox::StandardButton::Ok);
    mLoadDlg.close();
    return false;
  }
  mLoadDlg.close();
  num = sn.size();
  if (sn.size() == 0) {
    for (auto i = 0; i < coms.size(); i++) {
      ui->mComBox->addItem(coms[i], "none");
    }
  } else {
    for (auto i = sn.begin(); i != sn.end(); i++) {
      ui->mComBox->addItem(i.key(), i.value());
    }
  }
  if (sn.size() == 1) {
    if (mportMg->connectPort(ui->mComBox->currentText()) != 0) {
      QMessageBox::warning(this, tr("Waring"),
                           QString(tr("Connect coms:%1 is failed"))
                           .arg(ui->mComBox->currentText()),
                           QMessageBox::StandardButton::Ok);
      return true;
    }
    // num = sn.size();
    return true;
  }
  if (num) {
    // num = sn.size();
    return true;
  }

  //num = coms.size();

#endif
  return true;
}

void mconnectDlg::on_mConnectBtn_clicked() {
  if (mportMg->connectPort(ui->mComBox->currentText()) != 0) {
    QMessageBox::warning(this, tr("Waring"),
                         QString(tr("Connect coms:%1 is failed"))
                         .arg(ui->mComBox->currentText()),
                         QMessageBox::StandardButton::Ok);
    reject();
    return;
  }
  accept();
}

void mconnectDlg::on_mCloseBtn_clicked() {
  mportMg->close();
  reject();
}

void mconnectDlg::on_mComBox_currentIndexChanged(int index) {
  auto v = ui->mComBox->currentData().toByteArray().toStdString();
  ui->mSnLab->setText(QString::fromStdString(v));
  // QString sn = "sn:" + v;
  // ui->mSNLab->setText(sn);
}
