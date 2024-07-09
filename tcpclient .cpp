#include "TCPClient.h"
#include "log.h"
#include <QMessageBox>
const int MINSIZE = 11;
TCPClient::TCPClient(QObject *parent)
  : QObject(parent),
    tcpSocket(new QTcpSocket(this)) {
  connect(tcpSocket, &QTcpSocket::connected, this, &TCPClient::onConnected);
  connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
  connect(this, &TCPClient::sendDisConnected, mportManager::instance(), &mportManager::sendDisconnect);
  connect(this, &TCPClient::sendConnected, mportManager::instance(), &mportManager::sendConnect);
  connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
  connect(tcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
          this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));
  QObject::connect(this, &TCPClient::receiveMsg, mportManager::instance(),
                   &mportManager::receiveDataFromTcp, Qt::QueuedConnection);

  connect(this, &TCPClient::connectionFailed, [](const QString & errorString) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(QString(tr("Connection failed:%1.")).arg(errorString));
    QPushButton *yesButton = msgBox.addButton(tr("Confirm"), QMessageBox::AcceptRole);
    msgBox.exec();
  });
}

TCPClient::~TCPClient() {
  delete tcpSocket;
}

void TCPClient::connect_(const QString &host, quint16 port) {
  m_host = host;
  m_port = port;
  QHostAddress address(host);
  tcpSocket->connectToHost(address, port);
  if (!tcpSocket->waitForConnected(5000)) {
    connectionFailed(tr("Connection timed out"));
  }
}

void TCPClient::disconnect(void *) {
  tcpSocket->disconnectFromHost();
}

void TCPClient::sendMessage(const QString &message) {
  if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
    tcpSocket->write(message.toUtf8());
  } else {
    qDebug() << "Not connected to the server";
  }
}

int TCPClient::send(const QByteArray &msg, QString &err, bool isPrint) {
  if (tcpSocket->state() != QAbstractSocket::ConnectedState) {
    err = tr("tcp socket is not connected,please connect.");
    return -1;
  }
  auto r = tcpSocket->write(msg.toStdString().c_str(), msg.length());
  if (r < 0) {
    QLOG_ERROR() << tcpSocket->errorString();
    // QMessageBox::warning(nullptr, tr("is error"),
    //         tr("com send is error:") + errorString());
    err = tr("tcp send is error:") + tcpSocket->errorString();
    return -1;
  }
  return 0;
}

int TCPClient::sendData(const QByteArray &msg) {
  QLOG_INFO() << msg.toHex();
  if (tcpSocket->state() != QAbstractSocket::ConnectedState) {
    return -1;
  }
  auto r = tcpSocket->write(msg.toStdString().c_str(), msg.length());
  if (r < 0) {
    QLOG_ERROR() << tcpSocket->errorString();
  }
  return r;
}

QString TCPClient::name() {
  return  QString("%1:%2").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort());
}

void TCPClient::close() {
  actClose = true;
  tcpSocket->disconnectFromHost();
}

bool TCPClient::connected() {
  if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
    return true;
  } else {
    return false;
  }
}

void TCPClient::onConnected() {
  actClose = false;
  qDebug() << "Connected to the server";
  sendConnected(m_host);
}

void TCPClient::onDisconnected() {
  qDebug() << "Disconnected from the server";
  sendDisConnected(m_host);
  if (actClose == false) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Warning"));
    msgBox.setText(QString(tr("Tcp is disconnected, please reconnect.")));
    QPushButton *yesButton = msgBox.addButton(tr("Confirm"), QMessageBox::AcceptRole);
    msgBox.exec();
  }
}

void TCPClient::onReadyRead() {
  QString buf;

  if (tcpSocket->bytesAvailable() <= 0) {
    return;
  }

  //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
  QByteArray buffer;
  //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
  //所以使用while(bytesAvailable())意义不大，其实只执行一次。
  buffer = tcpSocket->readAll();
  // QLOG_DEBUG() << "Receive[" << buffer.length() << "]" << buffer.toHex();
  // qDebug() << buffer.toHex();
  //上次缓存加上这次数据

  mBuffer.append(buffer);

  ushort mesg_len;
  uchar head[3] = { 0 };
  uchar order[3];

  uchar ox, a;
  uchar stopBit;

  int totalLen = mBuffer.size();

  while (totalLen) {
    //与QDataStream绑定，方便操作。
    cmdData validData = { 0 };
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
    emit receiveMsg(m_host, validData);
    //缓存多余的数据
    buffer = mBuffer.right(totalLen - mesg_len - MINSIZE);

    //更新长度
    totalLen = buffer.size();

    //更新多余数据
    mBuffer = buffer;
  }
}

void TCPClient::onErrorOccurred(QAbstractSocket::SocketError socketError) {
  QString errorString;
  switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
      errorString =  tr("Remote host closed the connection");
      break;
    case QAbstractSocket::HostNotFoundError:
      errorString = tr("Host not found");
      break;
    case QAbstractSocket::ConnectionRefusedError:
      errorString =  tr("Connection refused");
      break;
    default:
      errorString =  tr("An error occurred:") + tcpSocket->errorString();
  }
  emit connectionFailed(errorString);
}
