#include "mcorrectiondlg.h"

#include <QEventLoop>

#include "mportmanager.h"
#include "ui_mcorrectiondlg.h"
mCorrectionDlg::mCorrectionDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::mCorrectionDlg) {
  ui->setupUi(this);
  init();
}

mCorrectionDlg::~mCorrectionDlg() { delete ui; }

void mCorrectionDlg::closeEvent(QCloseEvent *e) { on_mCacel_clicked(); }

void mCorrectionDlg::showDlg() {
  show();
  QEventLoop loop;
  connect(this, SIGNAL(dlg_exit()), &loop, SLOT(quit()));
  loop.exec(QEventLoop::ExcludeSocketNotifiers);
}

void mCorrectionDlg::init() {
  Qt::WindowFlags windowFlag = Qt::Dialog | Qt::WindowStaysOnTopHint;
  // setWindowModality(Qt::ApplicationModal);
  windowFlag |= Qt::WindowCloseButtonHint;
  setModal(true);
  setWindowFlags(windowFlag);
#ifdef DXJG_SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#else
  setWindowIcon(QIcon(":/img/logo_t.png"));
#endif
  setWindowTitle(tr("corre"));
  ui->mWarningLab->setWordWrap(true);
  ui->mWarningLab->setText(
      tr("Please be sure to use a power meter to cover the light outlet, "
         "otherwise high-power laser will be emitted."));
  ui->mTipLab->setText(tr("Correct switch status"));
  ui->mOpBtn->setEnabled(true);
  if (mportMg->getCureEnable())
    ui->mStartLab->setText(tr(" start"));
  else {
    ui->mStartLab->setText(tr("not start"));
    ui->mOpBtn->setEnabled(false);
  }
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
}

void mCorrectionDlg::receiveQuery(QString name, queryInfo info, int a) {
  if (a != 1) return;
  if (mStep <= 2) return;

  // ui->mTipLab->setText(tr("correnction val:"));
  // auto val = (double)info.PowerCorrect * 0.01;
  //  QString valStr = QString::number(val, 'f', 2);
  if (info.CorrenctStatus == 0)
    ui->mStartLab->setText(tr("Not start"));
  else if (info.CorrenctStatus == 1)
    ui->mStartLab->setText(tr("corr run"));
  else
    ui->mStartLab->setText(tr("corr exit"));
}

void mCorrectionDlg::on_mOpBtn_clicked() {
  mStep++;
  mStep %= 4;

  if (mStep == 1) {
    ui->mTipLab->setText(tr("Determination of light outlet status"));
    ui->mStartLab->setText("");
    return;
  }
  if (mStep == 2) {
    ui->mTipLab->setText(tr("Calibration start determination"));
    ui->mStartLab->setText(tr("not Start"));
    return;
  }
  if (mStep == 3) {
    uint8_t b = 1;
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << b;
    ui->mOpBtn->setEnabled(false);
    mportMg->send(CUR_ENABLE, val);
    mportMg->refresh();
    loop.exec();
  }
  accept();
  emit dlg_exit();
}

void mCorrectionDlg::on_mCacel_clicked() {
  if (mStep == 3) {
    uint8_t b = 0;
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    packet << b;
    mportMg->send(CUR_ENABLE, val);
    // return;
  }
  mStep = 0;
  loop.quit();
  ui->mOpBtn->setEnabled(true);
  reject();
  emit dlg_exit();
}
