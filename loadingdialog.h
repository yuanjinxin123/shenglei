#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H
#include <QDialog>
#include <QMovie>
namespace Ui {
class LoadingDialog;
}
#define USER_CANCEL -1
class LoadingDialog : public QDialog {
  Q_OBJECT
 public:
  explicit LoadingDialog(QWidget *parent = nullptr);
  ~LoadingDialog();
  void setCanCancel(bool bCanCancel);

 protected:
  void paintEvent(QPaintEvent *event) override;

 private:
  void initUi();
 Q_SIGNALS:
  void cancelWaiting();
 private slots:
  void cancelBtnClicked();

 private:
  QMovie *m_pLoadingMovie;
 private:
  Ui::LoadingDialog *ui;
};
#endif  // LOADINGDIALOG_H
