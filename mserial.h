#ifndef MSERIAL_H
#define MSERIAL_H

#include <QByteArray>
#include <QObject>
#include <QSerialPort>

#include "iport.h"
#include "mportmanager.h"
class mSerial : public QSerialPort, public iport {
  Q_OBJECT
 public:
  explicit mSerial(QObject *parent = nullptr);
  virtual ~mSerial();
  int setParam(const QString &name, const QSerialPort::BaudRate &rate,
               const QSerialPort::StopBits &stopbits,
               const QSerialPort::DataBits &databits,
               const QSerialPort::Parity &checkbit);

  int getParam(QString &name, QSerialPort::BaudRate &rate,
               QSerialPort::StopBits &stopbits, QSerialPort::DataBits &databits,
               QSerialPort::Parity &checkbit);
  int send(const QByteArray &msg, QString &err, bool isPrint = true);
  int sendData(const QByteArray &msg);
  int connect_();
  int disconnect(void *);
  virtual QString name();
  virtual void close() override;
  void read_Data(bool &isCtn);
  bool connected();
 protected slots:
  void read_Data();
  void Disconnect(QSerialPort::SerialPortError err);
  bool waitForSend(int times);
 signals:
  void receiveMsg(QString, cmdData);
  void status(QString, int, QString);
  void signalErr(QSerialPort::SerialPortError);

 protected:
  QByteArray mBuffer;
  QString MCom;
  bool mIsTimeout = false;
};

#endif  // MSERIAL_H
