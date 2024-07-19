#ifndef MDATECHECKDLG_H
#define MDATECHECKDLG_H
#include <QDialog>
#include <QFile>
#include <QProgressDialog>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QTemporaryFile>
#include <QToolTip>
#include <QMap>
namespace Ui {
class mDateCheckDlg;
}

class mDateCheckDlg : public QDialog {
  Q_OBJECT

 public:
  explicit mDateCheckDlg(QWidget *parent = nullptr);
  ~mDateCheckDlg();

  bool makeCSV(QProgressDialog *progressDialog, const uint64_t &index,
               const QVariantList &val, void *param);

 protected:
  void resizeEvent(QResizeEvent *event);
 protected slots:
  void showToolTip(const QModelIndex &index);
 private slots:
  void on_mCheckBtn_clicked();

  void on_mJumpBtn_clicked();

  void on_mPage_valueChanged(int arg1);

  void on_mSelDb_currentIndexChanged(int index);

  void on_mExportBtn_clicked();

 protected:
  void tableInit();
  void dataInit();
  void dataParse();

  void updateDl(int index);

 private:
  Ui::mDateCheckDlg *ui;
  QSqlQueryModel *mpMode;
  QStandardItemModel *mPowerModel;
  QStandardItemModel *mElecModel;
  QStandardItemModel *mTempModel;
  uint64_t mOffset = 0;
  uint64_t mLimit = 0;
  bool mIsInitCsv = false;
  QString mCsvFileName;
  QFile mfile;
  QTemporaryFile mTempFile;

  QString mTitleQuery1;
  QString mTitleQuery2;
  QString mDataQuery1;
  QString mDataQuery2;
  QString mCurrentSN;
  QMap<QString, int> mAlarmMap;
};

#endif  // MDATECHECKDLG_H
