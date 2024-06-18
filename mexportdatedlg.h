#ifndef MEXPORTDATEDLG_H
#define MEXPORTDATEDLG_H

#include <QDialog>
#include <QString>
#include <QTime>
namespace Ui {
class mExportDateDlg;
}

class mExportDateDlg : public QDialog {
  Q_OBJECT

 public:
  explicit mExportDateDlg(QWidget* parent = nullptr);
  ~mExportDateDlg();
  void getDateTime(QDateTime& startTime, QDateTime& endTime, QString& fileDir);
 private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_mOpenFileBtn_clicked();

 private:
  Ui::mExportDateDlg* ui;
  QString mFileDir;
};

#endif  // MEXPORTDATEDLG_H
