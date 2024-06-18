#ifndef PORTCFG_H
#define PORTCFG_H

#include <QDialog>
#include <QVariant>
#include <QVector>

#include "mserial.h"
namespace Ui {
class portCfg;
}
struct com_param {
  QString name;
  int rate;
  int bits;
  int parity;
  int stopBit;
};
class portCfg : public QDialog {
  Q_OBJECT

 public:
  explicit portCfg(QWidget *parent = nullptr);
  ~portCfg();
  bool setOpenCom(mSerial *serial);
  bool getOpenCom(mSerial *serial);
  void getComParam(QVector<com_param> &param);
  bool serialCfg();
  static portCfg *ins();

  void getComParam(const QString &name, com_param &param);

 private:
  void init();
  bool getParam(const QString &k, QString &val);
  bool parseJson(const QString &name, const QString &json);
  bool parseJson(const QString &name, const QString &json,
                 QVector<QVariant> &data);
  QString makeComJson(const QString &name);
  void resetDefaultParam();
 public slots:

  void on_cboxPortName_currentIndexChanged(const QString &arg1);
 private slots:

  void on_mCacel_clicked();

  void on_mOk_clicked();

 private:
  Ui::portCfg *ui;

 private:
  mSerial *m_serial = nullptr;
  QString mRate, mBit, mParity, mStopbit;
};
#define mportcfg portCfg::ins()
#endif  // PORTCFG_H
