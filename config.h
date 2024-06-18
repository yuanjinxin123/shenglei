#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QVariant>
static QString config_log = "log/LEVEL";
static QString config_com_time1 = "COM/CHECK_TIMEOUT";
static QString config_com_time2 = "COM/CHECK1_2_TIMEOUT";
static QString config_com_valid = "COM/VALID_TIMEOUT";

static QString config_db_limit = "DB/LIMIT";
static QString config_sql_freq = "SQL/saveFreq";
static QString config_main_title = "main/title";
class Config {
 public:
  static Config* getIns() {
    static Config _f;
    return &_f;
  };
  void init(QString qstrfilename = "");
  virtual ~Config(void);
  void Set(QString, QString, QVariant);
  QVariant Get(QString, QString, QVariant def);

  QVariant Get(const QString& key);

 private:
  Config(){};
  Config(const Config&);
  Config& operator=(const Config&);

 private:
  QString m_qstrFileName;
  QSettings* mpSetting;
};

#endif  // CONFIG_H
