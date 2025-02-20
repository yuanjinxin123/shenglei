﻿#ifndef IPORT
#define IPORT

#include <QByteArray>

class iport {
 public:
  iport();
  virtual int send(const QByteArray &msg, QString &err, bool isPrint = true) = 0;
  virtual int sendData(const QByteArray &msg) = 0;
  virtual bool connected() = 0;
  virtual QString name() = 0;
  virtual void close() = 0;
};

#endif  // IPORT
