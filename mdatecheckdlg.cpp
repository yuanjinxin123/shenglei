#include "mdatecheckdlg.h"

#include <QFile>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QTextStream>
#include <QTime>

#include "QTemporaryFile.h"
#include "config.h"
#include "jlcompressex.h"
#include "mexportdatedlg.h"
#include "mportmanager.h"
#include "sql.h"
#include "ui_mdatecheckdlg.h"
mDateCheckDlg::mDateCheckDlg(QWidget *parent)
  : QDialog(parent), ui(new Ui::mDateCheckDlg) {
  ui->setupUi(this);
  //setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint );
  tableInit();
  dataInit();

  //this->resize(QSize(1600, 800));
  connect(ui->tableView, SIGNAL(entered(QModelIndex)), this,
          SLOT(showToolTip(QModelIndex)));
  on_mCheckBtn_clicked();
}

mDateCheckDlg::~mDateCheckDlg() { delete ui; }

bool mDateCheckDlg::makeCSV(QProgressDialog *progressDialog,
                            const uint64_t &index, const QVariantList &val,
                            void *param) {
  if (val.empty()) return false;

  uint64_t counts = *(uint64_t *)param;
  queryInfo info;
  if (val.size() != 3) return false;
  if (val[1].toUInt() == QUERY1) {
    if (!mDataQuery1.isEmpty()) {
      mTitleQuery1.clear();
      mTitleQuery2.clear();
      mDataQuery1.clear();
      mDataQuery2.clear();
    }

    mportMg->parseQuery1(info, QByteArray::fromHex(val[2].toByteArray()));

    if (mTitleQuery1.isEmpty()) {
      QString str;
      str += tr("date,");
      str += tr("SN,");
      for (auto j = 0; j < info.DL_set.size(); j++) {
        str += QString(tr("LD%1")).arg(j);
        str += ",";
      }
      str += tr("red,") + tr("SHG,") + tr("THG,");
      for (auto j = 0; j < info.Amp_work.size(); j++) {
        str += QString(tr("Amp%1 Temp")).arg(j);
        str += ",";
      }
      for (auto j = 0; j < info.JTWD_work.size(); j++) {
        str += QString(tr("Cry%1 Temp")).arg(j);
        str += ",";
      }
      mTitleQuery1 = str;
    }

    //设置数据
    QString str;
    str += val[0].toDateTime().toString("yyyy-MM-dd HH:mm:ss") + ",";
    str += QString::fromStdString(std::string((char *)info.LaserSN, sizeof(info.LaserSN))) + ",";

    for (auto j = 0; j < info.DL_set.size(); j++) {
      double v = (double)info.DL_set[j] * 0.01;
      str += QString::number(v, 'f', 2);
      str += ",";
    }

    str += QString::number(info.Power_red * 0.01, 'f', 2) + ",";
    str += QString::number(info.SHG_WD_work * 0.01, 'f', 2) + ",";
    str += QString::number(info.THG_WD_work * 0.01, 'f', 2) + ",";
    for (auto d : info.Amp_work) {
      double n = (double)d * 0.1;
      str += QString::number(n, 'f', 1) + ",";
    }

    for (auto d : info.JTWD_work) {
      double n = (double)d * 0.1;
      str += QString::number(n, 'f', 2) + ",";
      // if(d+1 == info.JTWD_work.end()){}
    }

    mDataQuery1 = str;
  } else if (val[1].toUInt() == QUERY2) {
    if (mDataQuery1.isEmpty() || !mDataQuery2.isEmpty()) {
      mTitleQuery1.clear();
      mTitleQuery2.clear();
      mDataQuery1.clear();
      mDataQuery2.clear();
      return true;
    }

    mportMg->parseQuery2(info, QByteArray::fromHex(val[2].toByteArray()));

    if (mTitleQuery2.isEmpty()) {
      QString str;
      for (auto j = 0; j < info.Power.size(); j++) {
        str += QString(tr("power%1")).arg(j);
        if (j == info.Power.size() - 1) break;
        str += ",";
      }
      str += "\n";
      mTitleQuery2 = str;
    }

    QString str;
    for (auto d : info.Power) {
      double n = (double)d * 0.01;
      str += QString::number(n, 'f', 2) + ",";
    }
    str = str.left(str.size() - 1);
    str += "\n";
    mDataQuery2 = str;
  } else {
    mDataQuery1.clear();
    mDataQuery2.clear();
    return true;
  }

  if (!mDataQuery1.isEmpty() && !mDataQuery2.isEmpty()) {
    QTextStream csvOutPut(&mTempFile);
    if (!mIsInitCsv) {
      mIsInitCsv = true;
      progressDialog->setRange(0, counts);
      csvOutPut << mTitleQuery1.toUtf8() << mTitleQuery2.toUtf8();
    }
    csvOutPut << mDataQuery1.toUtf8() << mDataQuery2.toUtf8();
    mDataQuery1.clear();
    mDataQuery2.clear();
  }

  progressDialog->setValue(index);

  return true;
}

void mDateCheckDlg::resizeEvent(QResizeEvent *event) {
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
// int i = 5;
void mDateCheckDlg::on_mCheckBtn_clicked() {
  uint64_t count = 0;

  mSql->getTableCounts("equip_param", "", count);
  ui->mTotalCount->setValue(count);
  QString sql;
  sql = QString(
          "select log_date,order_,user,cmd from equip_param order by "
          "log_date desc limit %1 offset %2")
        .arg(mLimit)
        .arg(mOffset);
  mpMode->setQuery(sql);
  uint totalPage = count / mLimit;
  ui->mPage->setMinimum(1);
  ui->mPage->setMaximum(totalPage + 1);
  ui->mPage->setSuffix(QString("/%1").arg(totalPage + 1));
  // ui->tableView->setModel(mpMode);
  dataParse();
  on_mSelDb_currentIndexChanged(ui->mSelDb->currentIndex());
}
void mDateCheckDlg::showToolTip(const QModelIndex &index) {
  if (!index.isValid()) {
    return;
  }

  QToolTip::showText(QCursor::pos(), index.data().toString());
}
void mDateCheckDlg::on_mJumpBtn_clicked() {}

void mDateCheckDlg::on_tableView_entered(const QModelIndex &index) {
  ui->tableView->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  ui->tableView->setColumnWidth(0, 200);

  ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->tableView->setColumnWidth(1, 200);
}

void mDateCheckDlg::tableInit() {
  mpMode = new QSqlQueryModel(ui->tableView);
  mPowerModel = new QStandardItemModel(ui->tableView);
  mElecModel = new QStandardItemModel(ui->tableView);
  mTempModel = new QStandardItemModel(ui->tableView);

  ui->tableView->setMouseTracking(true);
  ui->tableView->verticalHeader()->setVisible(false);  //水平表头不显示
  ui->tableView->setSortingEnabled(true);
  ui->tableView->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  // ui->tableView->setFrameShape(QFrame::NoFrame);
  ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
  ui->tableView->setSelectionBehavior(
    QAbstractItemView::SelectRows);  //设置选中整行
  ui->tableView->setAlternatingRowColors(true);
  ui->tableView->setEditTriggers(
    QAbstractItemView::NoEditTriggers);  //设置不可编辑

}

void mDateCheckDlg::dataInit() {
  // setWindowTitle(tr("date export"));
#ifdef DXJG_SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#else
  setWindowIcon(QIcon(":/img/logo_t.png"));
#endif
  setWindowTitle(tr("Query Data"));
  QStringList items;
  items << tr("LD") << tr("temp") << tr("power");
  ui->mSelDb->addItems(items);
  mLimit = Config::getIns()->Get(config_db_limit).toInt();
  // setWindowTitle(tr("date export"));
  // progressDialog->setRange(0, num);
}

void mDateCheckDlg::dataParse() {
  QFont font;
  font.setPointSize(14);
  mElecModel->clear();
  mTempModel->clear();
  mPowerModel->clear();
  bool isElecHead = false;
  bool isSempHead = false;
  bool isPowerHead = false;
  int q1 = 0, q2 = 0;
  for (auto i = 0; i < mpMode->rowCount(); i++) {
    uint64_t cmd = mpMode->index(i, 1).data().toUInt();
    QByteArray a =
      QByteArray::fromHex(mpMode->index(i, 3).data().toByteArray());
    queryInfo info;
    QDateTime time = mpMode->index(i, 0).data().toDateTime();
    static QString strSNQuery1;
    if (cmd == QUERY1) {
      mportMg->parseQuery1(info, a);
      if (isElecHead == false) {
        isElecHead = true;
        QStringList elecHead;
        elecHead << tr("date");
        elecHead << tr("SN");
        for (auto j = 0; j < info.DL_set.size(); j++) {
          QString elec = QString(tr("LD%1")).arg(j);
          elecHead << elec;
        }
        elecHead << tr("red") << tr("SHG") << tr("THG");
        mElecModel->setHorizontalHeaderLabels(elecHead);
      }
      QStandardItem *itemTime = new QStandardItem(time.toString("yyyy-MM-dd HH:mm:ss"));
      itemTime->setFont(font);
      mElecModel->setItem(q1, 0, itemTime);
      mElecModel->item(q1, 0)->setData(time.toString(), Qt::ToolTip);

      strSNQuery1 = QString::fromStdString(std::string((char *)info.LaserSN, sizeof(info.LaserSN)));
      QStandardItem *itemSNElec = new QStandardItem(strSNQuery1);
      itemSNElec->setFont(font);
      mElecModel->setItem(q1, 1, itemSNElec);
      mElecModel->item(q1, 1)->setTextAlignment(Qt::AlignCenter);

      int index = 2;
      // 电流
      for (auto d : info.DL_set) {
        double n = (double)d * 0.01;
        QStandardItem *itemElectric1 = new QStandardItem(QString::number(n, 'f', 2));
        itemElectric1->setFont(font);
        mElecModel->setItem(q1, index, itemElectric1);
        mElecModel->item(q1, index)->setTextAlignment(Qt::AlignCenter);
        index++;
      }
      QStandardItem *itemElectric2 = new QStandardItem(QString::number(info.Power_red * 0.01, 'f', 2));
      itemElectric2->setFont(font);
      mElecModel->setItem(q1, index, itemElectric2);
      mElecModel->item(q1, index)->setTextAlignment(Qt::AlignCenter);
      index++;
      QStandardItem *itemElectric3 = new QStandardItem(QString::number(info.SHG_WD_work * 0.01, 'f', 2));
      itemElectric3->setFont(font);
      mElecModel->setItem(q1, index, itemElectric3);
      mElecModel->item(q1, index)->setTextAlignment(Qt::AlignCenter);
      index++;
      QStandardItem *itemElectric4 = new QStandardItem(QString::number(info.THG_WD_work * 0.01, 'f', 2));
      itemElectric4->setFont(font);
      mElecModel->setItem(q1, index, itemElectric4);
      mElecModel->item(q1, index)->setTextAlignment(Qt::AlignCenter);
      index++;
      // 温度
      QStandardItem *itemTemp1 = new QStandardItem(time.toString("yyyy-MM-dd HH:mm:ss"));
      itemTemp1->setFont(font);
      mTempModel->setItem(q1, 0, itemTemp1);
      mTempModel->item(q1, 0)->setData(time.toString(), Qt::ToolTip);
      if (isSempHead == false) {
        isSempHead = true;
        QStringList sempHead;
        sempHead << tr("date");
        sempHead << tr("SN");
        for (auto j = 0; j < info.Amp_work.size(); j++) {
          QString semp = QString(tr("Amp%1 Temp")).arg(j);
          sempHead << semp;
        }
        for (auto j = 0; j < info.JTWD_work.size(); j++) {
          QString semp = QString(tr("Cry%1 Temp")).arg(j);
          sempHead << semp;
        }
        mTempModel->setHorizontalHeaderLabels(sempHead);
      }

      QStandardItem *itemSNTemp = new QStandardItem(strSNQuery1);
      itemSNTemp->setFont(font);
      mTempModel->setItem(q1, 1, itemSNTemp);
      mTempModel->item(q1, 1)->setTextAlignment(Qt::AlignCenter);

      index = 2;
      for (auto d : info.Amp_work) {
        double n = (double)d * 0.1;
        QStandardItem *item = new QStandardItem(QString::number(n, 'f', 1));
        item->setFont(font);
        mTempModel->setItem(q1, index, item);
        mTempModel->item(q1, index)->setTextAlignment(Qt::AlignCenter);
        index++;
      }

      for (auto d : info.JTWD_work) {
        double n = (double)d * 0.1;
        QStandardItem *item = new QStandardItem(QString::number(n, 'f', 2));
        item->setFont(font);
        mTempModel->setItem(q1, index, item);
        mTempModel->item(q1, index)->setTextAlignment(Qt::AlignCenter);
        index++;
      }
      q1++;
    }
    if (cmd == QUERY2) {
      mportMg->parseQuery2(info, a);
      if (isPowerHead == false) {
        isPowerHead = true;
        QStringList powerHead;
        powerHead << tr("date");
        powerHead << tr("SN");
        for (auto j = 0; j < info.Power.size(); j++) {
          QString power = QString(tr("power%1")).arg(j);
          powerHead << power;
        }
        mPowerModel->setHorizontalHeaderLabels(powerHead);
      }
      QStandardItem *item = new QStandardItem(time.toString("yyyy-MM-dd HH:mm:ss"));
      item->setFont(font);
      mPowerModel->setItem(q2, 0, item);
      mPowerModel->item(q2, 0)->setData(time.toString(), Qt::ToolTip);

      QStandardItem *itemSNPower = new QStandardItem(strSNQuery1);
      itemSNPower->setFont(font);
      mPowerModel->setItem(q2, 1, itemSNPower);
      mPowerModel->item(q2, 1)->setTextAlignment(Qt::AlignCenter);
      strSNQuery1.clear();

      int index = 2;
      for (auto d : info.Power) {
        double n = (double)d * 0.01;
        QStandardItem *item = new QStandardItem(QString::number(n, 'f', 2));
        item->setFont(font);
        mPowerModel->setItem(q2, index, item);
        mPowerModel->item(q2, index)->setTextAlignment(Qt::AlignCenter);
        index++;
      }
      q2++;
    }
  }
}

void mDateCheckDlg::updateDl(int i) {}

void mDateCheckDlg::on_mPage_valueChanged(int n) {
  mOffset = mLimit * (n - 1);
  on_mCheckBtn_clicked();
}

void mDateCheckDlg::on_mSelDb_currentIndexChanged(int index) {
  ui->tableView->setModel(nullptr);

  if (index == 0 && mElecModel->rowCount()) {
    ui->tableView->setModel(mElecModel);
  }

  if (index == 1 && mTempModel->rowCount()) {
    ui->tableView->setModel(mTempModel);
  }
  if (index == 2 && mPowerModel->rowCount()) {
    ui->tableView->setModel(mPowerModel);
  }

}

void mDateCheckDlg::on_mExportBtn_clicked() {
  mExportDateDlg exportDlg;
  if (exportDlg.exec() != QDialog::Accepted) {
    return;
  }
  QDateTime startTime;
  QDateTime endTime;
  exportDlg.getDateTime(startTime, endTime, mCsvFileName);

  // QFile file(mCsvFileName);
  QDateTime curTime = QDateTime::currentDateTime();
  auto timsStr = curTime.toString("yyyyMMddHH-mm-ss").toLocal8Bit() + ".csv";
  // QTemporaryFile tmpFile(timsStr);
  mTempFile.setFileName(timsStr);
  if (!mTempFile.exists()) {
    mIsInitCsv = false;
  }
  mTempFile.setAutoRemove(true);
  QProgressDialog *progressDialog;
  progressDialog = new QProgressDialog(this);
  progressDialog->setWindowModality(Qt::WindowModal);
  progressDialog->setMinimumDuration(5);
  progressDialog->setWindowTitle(tr("Please Wait"));
  progressDialog->setLabelText(tr("Copying..."));
  progressDialog->setCancelButtonText(tr("Cancel"));
  progressDialog->setMinimumSize(400, 180);
  QString style = "QProgressDialog { background-image:url(:/img/beijing.png); background-position:center;"
                  "background-repeat: no-repeat;border:none;}";
  progressDialog->setStyleSheet(style);

  if (!mTempFile.open()) {
    QMessageBox::warning(nullptr, tr("error"), tr("file not open"));
    return;
  }
  QString sql;
  sql =
    QString(
      "select log_date,order_,cmd from equip_param where log_date>=\"%1\" "
      "and log_date <=\"%2\";")
    .arg(startTime.toString("yyyy-MM-dd HH:mm:ss"))
    .arg(endTime.toString("yyyy-MM-dd HH:mm:ss"));
  mSql->query(sql, std::bind(&mDateCheckDlg::makeCSV, this, progressDialog,
                             std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3));

  // 文件压缩到内存buffer，zipIoDevice可以使用QBuffer zipBuffer;
  QString temp = ".zip";
  if (mCsvFileName.length() > temp.length() && mCsvFileName.right(temp.length()) == temp) {
    mfile.setFileName(mCsvFileName);
    JlCompressEx::CompressToBuffer(mTempFile.fileName(), "112233", mfile);
    mfile.close();
  }

  mTempFile.flush();
  mTempFile.close();
  progressDialog->cancel();
  progressDialog->deleteLater();
}
