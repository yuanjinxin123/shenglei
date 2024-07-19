#include "snmanager.h"
#include "mportmanager.h"

SNManager::SNManager(QObject *parent) : QObject(parent) {
  // 设置定时器，每秒执行一次 checkTimeout 函数
  connect(&timer, &QTimer::timeout, this, &SNManager::checkTimeout);
  timer.start(1000); // 每秒更新一次
}

void SNManager::addSN(const QString &sn) {
  QMutexLocker locker(&mutex);
  qint64 currentTime = QDateTime::currentSecsSinceEpoch();
  snMap.insert(sn, currentTime);
}

void SNManager::updateSN(const QString &sn) {
  QMutexLocker locker(&mutex);
  qint64 currentTime = QDateTime::currentSecsSinceEpoch();
  if (snMap.contains(sn)) {
    snMap[sn] = currentTime;
  }
}

void SNManager::removeSN(const QString &sn) {
  QMutexLocker locker(&mutex);
  snMap.remove(sn);
}

bool SNManager::isEmpty() {
  QMutexLocker locker(&mutex);
  return snMap.isEmpty();
}

bool SNManager::contains(const QString &sn) {
  QMutexLocker locker(&mutex);
  return snMap.contains(sn);
}
void SNManager::checkTimeout() {
  QMutexLocker locker(&mutex);
  qint64 currentTime = QDateTime::currentSecsSinceEpoch();
  for (auto it = snMap.begin(); it != snMap.end(); ) {
    if (currentTime - it.value() > 5 && it.key() != m_currSN) {
      qDebug() << "SN" << it.key() << "has expired.";
      emit sendRemoveSN(it.key());
      it = snMap.erase(it);
    } else {
      ++it;
    }
  }
}

void SNManager::clear() {
  QMutexLocker locker(&mutex);
  snMap.clear();
}

void SNManager::setCurrSn(QString &sn) {
  m_currSN = sn;
}

