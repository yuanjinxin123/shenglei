#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

#include "iport.h"
#include "mportmanager.h"

class TCPClient : public QObject, public iport {
  Q_OBJECT

 public:
  explicit TCPClient(QObject *parent = nullptr);
  ~TCPClient();

  void connect_(const QString &host, quint16 port);
  void disconnect(void *);
  void sendMessage(const QString &message);
  int send(const QByteArray &msg, QString &err, bool isPrint = true);
  int sendData(const QByteArray &msg);
  virtual QString name();
  virtual void close() override;
  bool connected();
 private slots:
  void onConnected();
  void onDisconnected();
  void onReadyRead();
  void onErrorOccurred(QAbstractSocket::SocketError socketError);
 signals:
  void receiveMsg(QString, cmdData);
  void sendDisConnected(QString);
 private:
  QTcpSocket *tcpSocket;
  QByteArray mBuffer;
  bool mIsTimeout = false;
  QString m_host;
  int m_port;
};

#endif // TCPCLIENT_H
