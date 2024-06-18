#include "mexportdatedlg.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTime>

#include "sql.h"
#include "ui_mexportdatedlg.h"
mExportDateDlg::mExportDateDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::mExportDateDlg) {
  ui->setupUi(this);

  Qt::WindowFlags windowFlag = Qt::Dialog;
  windowFlag |= Qt::WindowCloseButtonHint;
  setWindowFlags(windowFlag);
  setWindowTitle(tr("date export"));
#ifdef DXJG_SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#else
  setWindowIcon(QIcon(":/img/logo_t.png"));
#endif
  QVariant dateTime;
  if (mSql->query("select min(log_date) from equip_param", dateTime) == true &&
      dateTime.isValid()) {
    ui->mStartTime->setDateTime(dateTime.toDateTime());
  }
  ui->mEndTime->setDateTime(QDateTime::currentDateTime());

  mFileDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
  QVariant val;
  QString desc;
  if (mSql->getValue("file dir", "default", val, desc) == false) {
    mSql->setKey("file dir", "default", mFileDir, "save query file");
  } else {
    mFileDir = val.toString() + "/";
  }


  QPushButton *btnOk = ui->buttonBox->button(QDialogButtonBox::Ok);
  QPushButton *btnCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);

  btnOk->setFixedSize(100, 50);
  btnCancel->setFixedSize(100, 50);

  btnOk->setText("确定");
  btnCancel->setText("取消");

  QString style = "QPushButton { border-image:url(:/img/button_normal.png);font:18pt;color:rgb(98, 98, 98);} "
  "QPushButton:hover { border-image:url(:/img/button_select.png); color:rgb(255, 255, 255); }";
  btnOk->setStyleSheet(style);
  btnCancel->setStyleSheet(style);
}

mExportDateDlg::~mExportDateDlg() { delete ui; }

void mExportDateDlg::getDateTime(QDateTime &startTime, QDateTime &endTime,
                                 QString &fileDir) {
  startTime = ui->mStartTime->dateTime();
  endTime = ui->mEndTime->dateTime();
  fileDir = mFileDir;
}

void mExportDateDlg::on_buttonBox_accepted() { accept(); }

void mExportDateDlg::on_buttonBox_rejected() { reject(); }

void mExportDateDlg::on_mOpenFileBtn_clicked() {
  QDateTime curTime = QDateTime::currentDateTime();
  auto timsStr =
      mFileDir + curTime.toString("yyyyMMddHH-mm-ss").toLocal8Bit() + ".zip";
  QString csvFileName = QFileDialog::getSaveFileName(
      this, tr("save dir"), timsStr, "zip files(*.zip)");
  if (csvFileName.isEmpty()) {
    return;
  }
  mFileDir = csvFileName;
  auto dir = QFileInfo(mFileDir).absolutePath();

  mSql->setKey("file dir", "default", dir, "save query file");
}
