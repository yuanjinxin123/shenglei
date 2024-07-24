#include "sql.h"

#include <QApplication>
#include <QHostInfo>   // 计算机名
#include <QSqlDriver>  //数据库驱动种类相关头文件
#include <QSqlError>   //数据库执行出错相关头文件
#include <QSqlQuery>   //SQL语句执行相关头文件
#include <QSqlRecord>
#include <QStandardPaths>
#include <QDir>
#include "config.h"
#include "log.h"

Q_GLOBAL_STATIC(sql, g_sql);
sql::sql(QObject *parent) : QObject(parent) {
  //mTimes[QUERY1] = 0;
  //mTimes[QUERY2] = 0;
}

sql::~sql() { mDb.close(); }

sql *sql::ins() { return g_sql; }

int sql::init() {
  QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(appDataPath);
  QString dbFilePath = appDataPath + "/mult_sn_database.db";
  mUserName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
              .section("/", -1, -1);
  mCompute = QHostInfo::localHostName();
  if (QSqlDatabase::contains("qt_sql_default_connection")) {
    mDb = QSqlDatabase::database("qt_sql_default_connection");
  } else {
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(dbFilePath);
    mDb.setUserName("test");
    mDb.setPassword("123456");
  }

  if (!mDb.open()) {
    QLOG_ERROR() << "Error: Failed to connect database." << mDb.lastError();
    return -1;
  } else {
    QLOG_INFO() << "Succeed to connect database.";
  }
  loadTables();
  //创建表格
  QSqlQuery sql_query(mDb);
  if (!sql_query.exec(
        R"(create table IF NOT EXISTS sys_param (
    param_section TEXT    NOT NULL,
    param_key     TEXT    NOT NULL,
    string_value  BLOB    NOT NULL,
    param_desc    TEXT DEFAULT NULL,
    PRIMARY KEY (
        param_section,
        param_key
    )
    );)")) {
    QLOG_ERROR() << "Error: Fail to create table." << sql_query.lastError();
  } else {
    QLOG_INFO() << "Table created!";
  }
  mFreq = Config::getIns()->Get(config_sql_freq).toInt();
  return 0;
}

int sql::createTable(QString tableName) {
  QSqlQuery sql_query(mDb);
  QString fullTableName = "sn_" + tableName;
  QString createTableQuery = QString(
                               R"(CREATE TABLE IF NOT EXISTS %1 (
              ID       INTEGER  PRIMARY KEY AUTOINCREMENT
                                NOT NULL,
              log_date DATETIME DEFAULT (datetime('now', 'localtime') )
                                NOT NULL,
              order_   INT      NOT NULL,
              user     STRING,
              cmd      BLOB     NOT NULL,
              computer STRING,
              name     STRING
          );)").arg(fullTableName);

  if (!sql_query.exec(createTableQuery)) {
    qDebug() << "Error: Failed to create table." << sql_query.lastError();
    return -1;
  } else {
    qDebug() << "Table created!";
    mTableSet.insert(fullTableName); // Update the QSet
    return 0;
  }
}

bool sql::query(QString sql, query_func func) {
  QSqlQuery q(mDb);
  if (!q.exec(sql)) {
    QLOG_ERROR() << q.lastError();
    return false;
  }
  QSqlRecord record = q.record();

  auto indexs = record.count();
  uint64_t count = q.size() / indexs;
  int k = 0;
  while (q.next()) {
    QVariantList vlist;
    for (int i = 0; i < indexs; i++) {
      vlist << q.value(i);
    }
    if (func)
      if (func(k, vlist, &count) == false) return false;
    k++;
  }

  return true;
}

bool sql::query(const QString sql, QVariant &val) {
  QSqlQuery q(mDb);
  if (!q.exec(sql)) {
    QLOG_ERROR() << q.lastError();
    return false;
  }
  while (q.next()) {
    val = q.value(0);
  }
  return true;
}

bool sql::setKey(const QString &s, const QString &k, const QVariant &val,
                 const QString &decs) {
  QSqlQuery query(mDb);
  query.prepare(
    "INSERT OR REPLACE INTO "
    "sys_param(param_section,param_key,string_value,param_desc) "
    "VALUES(?,?,?,?)");

  query.bindValue(0, s);
  query.bindValue(1, k);
  query.bindValue(2, val);
  query.bindValue(3, decs);

  if (!query.exec())
    QLOG_ERROR() << "Error: Fail to create table." << query.lastError();
  else
    QLOG_INFO() << "Set ok";
  return true;
}

bool sql::getValue(const QString &s, const QString &k, QVariant &val,
                   QString &decs) {
  QSqlQuery q(mDb);
  val.setValue(nullptr);
  if (!q.exec(QString("SELECT string_value,param_desc FROM sys_param WHERE "
                      "param_section = '%1' AND "
                      "param_key = '%2';")
              .arg(s)
              .arg(k))) {
    QLOG_ERROR() << q.lastError();
    return false;
  }
  // if (q.next() == 0) return false;
  while (q.next()) {
    val = q.value(0).toByteArray();
    decs = q.value(1).toString();
  }
  return true;
}

bool sql::setCmd(const int &order, const QByteArray &cmd, const QString &name, const QString &sn) {
  QSqlQuery query(mDb);
  QString fullTableName = "sn_" + sn;
  if (!tableExists(sn)) {
    createTable(sn);
  }
  if (order != QUERY1 && order != QUERY2) return true;
  if (mTimes[sn][order]++ % mFreq) {
    return true;
  }

  query.prepare(QString(
                  "INSERT INTO "
                  "%1(order_,user,cmd,computer,name) "
                  "VALUES(?,?,?,?,?)").arg(fullTableName));
  query.addBindValue(order);

  query.addBindValue(mUserName);
  query.addBindValue(cmd.toHex());
  query.addBindValue(mCompute);
  query.addBindValue(name);

  if (!query.exec())
    QLOG_ERROR() << "Error: Fail to create table." << query.lastError();
  else
    QLOG_INFO() << "Set ok";
  return true;
}

bool sql::getCmd(QMap<QDateTime, cmd_info> &cmd, int limit, int offset) {
  QString sql(
    "select log_date,order_,cmd,name from equip_param order by log_date desc "
    "limit %1 offset %2;");
  sql.arg(limit).arg(offset);
  QSqlQuery q(mDb);
  if (!q.exec(sql)) {
    return false;
  }
  while (q.next()) {
    cmd_info info;
    auto d = q.value(0).toDateTime();
    info.order = q.value(1).toInt();
    info.cmd = q.value(2).toByteArray();
    info.name = q.value(3).toString();
    cmd.insert(d, info);
  }
  return true;
}

bool sql::getTableCounts(const QString &tablename, const QString &where,
                         uint64_t &count) {
  count = 0;
  QSqlQuery query(mDb);
  QString sql = QString("select count(*) from %1 %2").arg(tablename).arg(where);
  if (query.exec(sql)) {
    if (query.next()) {
      count = query.value(0).toUInt();
    }
  }
  return true;
}

const QSqlDatabase &sql::getDb() { return mDb; }

bool sql::getSnList(QStringList &sn_list) {
  for (auto it = mTableSet.begin(); it != mTableSet.end(); it++) {
    sn_list << (*it).mid(3);
  }
  return true;
}

void sql::loadTables() {
  QSqlQuery sql_query(mDb);
  sql_query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'sn_%'");

  while (sql_query.next()) {
    QString tableName = sql_query.value(0).toString();
    mTableSet.insert(tableName);
  }
}

bool sql::getEarliestTime(QDateTime &time) {
  QSqlQuery sql_query(mDb);
  for (auto it = mTableSet.begin(); it != mTableSet.end(); it++) {
    QString sql = QString("select min(log_date) from %1").arg(*it);
    if (!sql_query.exec(sql)) {
      return false;
    }
    if (sql_query.next()) {
      if (time.isNull() || time > sql_query.value(0).toDateTime()) {
        time = sql_query.value(0).toDateTime();
      }
    }
  }
  return true;
}

bool sql::tableExists(const QString &tableName) {
  return mTableSet.contains("sn_" + tableName);
}