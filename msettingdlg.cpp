#include "msettingdlg.h"

#include "mdatecheckdlg.h"
#include "mportManager.h"
#include "portcfg.h"
#include "ui_msettingdlg.h"
#include "define.h"
msettingdlg::msettingdlg(QWidget *parent)
  : QDialog(parent), ui(new Ui::msettingdlg) {
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
  setWindowTitle(tr("setting"));

  ui->mDbCheck->setEnabled(mportMg->getLogin() == 2);
  // connect(mportMg, &mportManager::curePowerEnable, this,
  // &msettingdlg::on_enable_mDbCheck);
}

msettingdlg::~msettingdlg() { delete ui; }

bool msettingdlg::setOpenCom(mSerial *serial) {
  // mCfg.setOpenCom(serial);
  return true;
}

bool msettingdlg::getOpenCom(mSerial *serial) { return true; }

void msettingdlg::on_mSerial_clicked() {
  portCfg mCfg;
  // portCfg mCfg;
  // if (portCfg::ins()->serialCfg() == false) return;
  mCfg.exec();
  // portCfg::ins()->close();
}

void msettingdlg::on_mDbCheck_clicked() {
  mDateCheckDlg dlg;
  dlg.exec();
}
void msettingdlg::on_enable_mDbCheck(bool isenable) {
  ui->mDbCheck->setEnabled(isenable);
}
