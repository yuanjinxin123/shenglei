#include "config.h"

#include <QDebug>
#include <QMap>>
#include <QtCore/QtCore>
// Config *Config::getIns()

static QMap<QString, QVariant> s_keys = {
  {"log/LEVEL", 0},
  {"COM/CHECK_TIMEOUT", 3000},
  {"COM/CHECK1_2_TIMEOUT", 500},
  {"DB/LIMIT", 50},
  {"SQL/saveFreq", 60},
  {"COM/VALID_TIMEOUT", 8000},
  {"main/title", "SL LASER CONTROL SOFTWARE"},
  {"main/lang", "zh"}
};

void Config::init(QString qstrfilename) {
  if (qstrfilename.isEmpty()) {
    m_qstrFileName = QCoreApplication::applicationDirPath() + "/Config.ini";
  } else {
    m_qstrFileName = qstrfilename;
  }

  mpSetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
  if (mpSetting->status() != QSettings::NoError) {
    mpSetting->clear();
  }
  mpSetting->setIniCodec(QTextCodec::codecForName("utf-8"));
  //for (auto it = s_keys.begin(); it != s_keys.end(); it++) {
  //  auto key = it.key();
  //  if (!mpSetting->contains(key)) {
  //    mpSetting->setValue(key, it.value());
  //  }
  //}
}

Config::~Config() { mpSetting->deleteLater(); }

void Config::Set(QString qstrnodename, QString qstrkeyname,
                 QVariant qvarvalue) {
  mpSetting->setValue(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname),
                      qvarvalue);
}
QVariant Config::Get(const QString &key) {
  QVariant qvar = mpSetting->value(QString("%1").arg(key));
  return qvar;
}
QVariant Config::Get(QString qstrnodename, QString qstrkeyname, QVariant def) {
  QVariant qvar =
    mpSetting->value(QString("/%1/%2").arg(qstrnodename).arg(qstrkeyname));
  return qvar;
}
