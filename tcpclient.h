#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include "iport.h"
#include "mportmanager.h"

#define DISCONNECT 0
#define CONNECTED 1
#define CONNECT_ERR 2
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
  void onReconnectTip(int cmd);
 signals:
  void receiveMsg(QString, cmdData);
  void sendDisConnected(QString);
  void sendConnected(QString);
  void connectionFailed(QString);
  void reconnectTip(int cmd);
 private:
  QTcpSocket *tcpSocket;
  QByteArray mBuffer;
  bool mIsTimeout = false;
  QString m_host;
  int m_port;
  bool actClose = false;
  QMessageBox *msgBox;
};

#endif // TCPCLIENT_H
