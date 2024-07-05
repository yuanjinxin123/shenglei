#include "mserial.h"

#include <qapplication.h>
#include <qdebug.h>
#include <qeventloop.h>
#include <qfuture.h>

#include <QDataStream>
#include <QMessageBox>
#include <memory>
#include <qtconcurrent>
#include <string>

#include "cstdio"
#include "log.h"
#include "order.h"
const int MINSIZE = 11;
mSerial::mSerial(QObject *parent) : iport() {}

mSerial::~mSerial() { close(); }

int mSerial::setParam(const QString &name, const BaudRate &rate,
                      const StopBits &stopbits, const DataBits &databits,
                      const Parity &checkbit) {
  setPortName(name);
  setBaudRate(rate);
  setStopBits(stopbits);
  setDataBits(databits);
  setParity(checkbit);
  return 0;
}

int mSerial::getParam(QString &name, BaudRate &rate, StopBits &stopbits,
                      DataBits &databits, Parity &checkbit) {
  name = portName();
  rate = BaudRate();
  stopbits = stopBits();
  databits = DataBits();
  checkbit = parity();

  return 0;
}

int mSerial::send(const QByteArray &msg, QString &err, bool isPrint) {
  if (isPrint) QLOG_INFO() << msg.toHex();
  if (isOpen() == false) {
    // QMessageBox::warning(nullptr, tr("is error"), tr("com is not open"));
    err = tr("com is not open");
    return -1;
  }

  auto r = write(msg.toStdString().c_str(), msg.length());
  if (r < 0) {
    QLOG_ERROR() << errorString();
    // QMessageBox::warning(nullptr, tr("is error"),
    //         tr("com send is error:") + errorString());
    err = tr("com send is error:") + errorString();
    return -1;
  }
  int waitTime = 0;

  // QFuture<bool> retWait = QtConcurrent::run(this, &mSerial::waitForSend,
  // 1000);
  flush();
  while (!waitForBytesWritten(300)) {
    waitTime++;
    qApp->processEvents();
    // std::this_thread::sleep_for(std::chrono::milliseconds(200));
    if (waitTime >= 5 && mIsTimeout) {
      QLOG_DEBUG() << "send timeout";
      return -1;
    }
  }

  if (mIsTimeout) {
    mportMg->updateTimes();
    mIsTimeout = false;
  }
  // QTime dieTime = QTime::currentTime().addMSecs(500);

  // while (QTime::currentTime() < dieTime)
  //  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

  // flush();
// err = tr("Is ok");
#if 0
  err = QString();
  QEventLoop loop;
  auto c = QObject::connect(this, SIGNAL(readyRead()), &loop, SLOT(quit()));
  QTimer::singleShot(2000, &loop, &QEventLoop::quit);
  loop.exec();
  bool cont = true;
  while (bytesAvailable() > 0 && cont == true) {
    read_Data(cont);
  }
#endif
  return r;
}
int mSerial::sendData(const QByteArray &msg) {
  QLOG_INFO() << msg.toHex();
  if (isOpen() == false) {
    return -1;
  }
  auto r = write(msg.toStdString().c_str(), msg.length());
  if (r < 0) {
    QLOG_ERROR() << errorString();
  }
  return r;
}
int mSerial::connect_() {
  QObject::connect(this, SIGNAL(readyRead()), this, SLOT(read_Data()),
                   Qt::QueuedConnection);

  QObject::connect(this, &mSerial::receiveMsg, mportManager::instance(),
                   &mportManager::receiveData, Qt::QueuedConnection);

  connect(this, &mSerial::signalErr, mportMg, &mportManager::portErr);
  QObject::connect(this, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
                   this, SLOT(Disconnect(QSerialPort::SerialPortError)));
  return 0;
}

int mSerial::disconnect(void *) {
  QObject::disconnect(this, SIGNAL(readyRead()), this, SLOT(read_Data()));
  QObject::disconnect(this, &mSerial::receiveMsg, mportManager::instance(),
                      &mportManager::receiveData);
  QObject::disconnect(this, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
                      this, SLOT(disconnect(QSerialPort::SerialPortError)));
  return 0;
}

QString mSerial::name() { return portName(); }

void mSerial::read_Data() {
  QString buf;

  if (bytesAvailable() <= 0) {
    return;
  }

  //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
  QByteArray buffer;
  //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
  //所以使用while(bytesAvailable())意义不大，其实只执行一次。
  buffer = readAll();
  // QLOG_DEBUG() << "Receive[" << buffer.length() << "]" << buffer.toHex();
  // qDebug() << buffer.toHex();
  //上次缓存加上这次数据

  mBuffer.append(buffer);

  ushort mesg_len;
  uchar head[3] = {0};
  uchar order[3];

  uchar ox, a;
  uchar stopBit;

  int totalLen = mBuffer.size();

  while (totalLen) {
    //与QDataStream绑定，方便操作。
    cmdData validData = {0};
    QDataStream packet(mBuffer);
    packet.setByteOrder(QDataStream::BigEndian);

    //不够包头的数据直接就不处理。
    if (totalLen < MINSIZE) {
      break;
    }

    if (packet.readRawData((char *)head, 3) != 3) break;
    if (head[0] != 0x7e || head[1] != 0xe7 ||
        head[2] != 0x7e) {  //缓存多余的数据
      buffer = mBuffer.right(totalLen - 1);

      //更新长度
      totalLen = buffer.size();

      //更新多余数据
      mBuffer = buffer;
      continue;
    }

    //查找命令
    //{}
    if (packet.readRawData((char *)order, 3) != 3) break;
    char *p = (char *)&validData.cmd;
    for (auto i = 2; i >= 0; i--) {
      *p = order[i];
      p++;
    }
    // memcpy((void *)&validData.cmd, order, 3);
    // std::copy(order, order + 3, &validData.cmd);
    packet >> mesg_len;

    //如果不够长度等够了在来解析
    if ((totalLen - 3 - 3 - 2) < mesg_len) {
      break;
    }
    uchar buf[1024];
    if (packet.readRawData((char *)buf, mesg_len) != mesg_len) break;
    if (packet.readRawData((char *)&ox, 1) != 1) break;
    if (packet.readRawData((char *)&a, 1) != 1) break;
    if (packet.readRawData((char *)&stopBit, 1) != 1) break;

    if (stopBit != 0x0d) {
      buffer = mBuffer.right(totalLen - 1);
      //更新长度
      totalLen = buffer.size();
      //更新多余数据
      mBuffer = buffer;
      continue;
    }

    //处理校验位
    uchar xx = 0;
    uchar aa = 0;
    for (int i = 3; i < mesg_len + 5 + 3; i++) {
      uchar k = mBuffer[i];
      xx ^= k;
      aa += mBuffer[i];
    }
#if 0
    if (xx != ox || aa != a) {
      buffer = mBuffer.right(totalLen - 1);

      //更新长度
      totalLen = buffer.size();

      //更新多余数据
      mBuffer = buffer;
      continue;
    }
#endif
    //数据足够多，且满足我们定义的包头的几种类型
    validData.data.append(QByteArray::fromRawData((const char *)buf, mesg_len));
    // QVariant varValue = QVariant::fromValue(validData);
    // QLOG_DEBUG() << mBuffer.toHex();
    emit receiveMsg(portName(), validData);
    //缓存多余的数据
    buffer = mBuffer.right(totalLen - mesg_len - MINSIZE);

    //更新长度
    totalLen = buffer.size();

    //更新多余数据
    mBuffer = buffer;
  }
}
void mSerial::close() {
  // if (isOpen()) {
  QSerialPort::close();
  Disconnect(QSerialPort::ResourceError);
  //}
}
void mSerial::read_Data(bool &isCtn) {
  QString buf;
  isCtn = true;
  if (bytesAvailable() <= 0) {
    return;
  }

  //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
  QByteArray buffer;
  //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
  //所以使用while(bytesAvailable())意义不大，其实只执行一次。
  buffer = readAll();

  mBuffer.append(buffer);

  ushort mesg_len;
  uchar head[3] = {0};
  uchar order[3];

  uchar ox, a;
  uchar stopBit;

  int totalLen = mBuffer.size();

  while (totalLen) {
    //与QDataStream绑定，方便操作。
    cmdData validData = {0};
    QDataStream packet(mBuffer);
    packet.setByteOrder(QDataStream::BigEndian);

    //不够包头的数据直接就不处理。
    if (totalLen < MINSIZE) {
      break;
    }

    if (packet.readRawData((char *)head, 3) != 3) break;
    if (head[0] != 0x7e || head[1] != 0xe7 ||
        head[2] != 0x7e) {  //缓存多余的数据
      buffer = mBuffer.right(totalLen - 1);

      //更新长度
      totalLen = buffer.size();

      //更新多余数据
      mBuffer = buffer;
      continue;
    }

    //查找命令
    //{}
    if (packet.readRawData((char *)order, 3) != 3) break;
    char *p = (char *)&validData.cmd;
    for (auto i = 2; i >= 0; i--) {
      *p = order[i];
      p++;
    }
    // memcpy((void *)&validData.cmd, order, 3);
    // std::copy(order, order + 3, &validData.cmd);
    packet >> mesg_len;

    //如果不够长度等够了在来解析
    if ((totalLen - 3 - 3 - 2) < mesg_len) {
      break;
    }
    uchar buf[1024];
    if (packet.readRawData((char *)buf, mesg_len) != mesg_len) break;
    if (packet.readRawData((char *)&ox, 1) != 1) break;
    if (packet.readRawData((char *)&a, 1) != 1) break;
    if (packet.readRawData((char *)&stopBit, 1) != 1) break;

    if (stopBit != 0x0d) {
      buffer = mBuffer.right(totalLen - 1);
      //更新长度
      totalLen = buffer.size();
      //更新多余数据
      mBuffer = buffer;
      continue;
    }

    //处理校验位
    uchar xx = 0;
    uchar aa = 0;
    for (int i = 3; i < mesg_len + 5 + 3; i++) {
      uchar k = mBuffer[i];
      xx ^= k;
      aa += mBuffer[i];
    }
#if 0
    if (xx != ox || aa != a) {
      buffer = mBuffer.right(totalLen - 1);

      //更新长度
      totalLen = buffer.size();

      //更新多余数据
      mBuffer = buffer;
      continue;
    }
#endif
    //数据足够多，且满足我们定义的包头的几种类型
    validData.data.append(QByteArray::fromRawData((const char *)buf, mesg_len));

    emit receiveMsg(portName(), validData);
    isCtn = false;

    buffer = mBuffer.right(totalLen - mesg_len - MINSIZE);

    //更新长度
    totalLen = buffer.size();

    //更新多余数据
    mBuffer = buffer;
    return;
  }
}
bool mSerial::connected() {
  return isOpen();
}
void mSerial::Disconnect(SerialPortError err) {
  QLOG_DEBUG() << "Err = " << err;
  if (err == QSerialPort::TimeoutError) {
    mIsTimeout = true;
    mportMg->updateTimes(5000);
  }
  if (err == QSerialPort::ResourceError) {
    mportMg->sendDisconnect(name());
  }
}

bool mSerial::waitForSend(int times) { return waitForBytesWritten(times); }
