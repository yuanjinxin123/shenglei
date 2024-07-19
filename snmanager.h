#pragma once
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QMap>

class SNManager : public QObject {
  Q_OBJECT

 public:
  SNManager(QObject *parent = nullptr);

  void addSN(const QString &sn);

  void updateSN(const QString &sn);

  void removeSN(const QString &sn);

  bool isEmpty();

  bool contains(const QString &sn);
  void checkTimeout();
  void clear();
  void setCurrSn(QString &sn);
 public slots:

 signals:
  void sendRemoveSN(QString);
 private:
  QMap<QString, qint64> snMap;
  QTimer timer;
  QMutex mutex;
  QString m_currSN;
};