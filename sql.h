#ifndef SQL_H
#define SQL_H
#include <QByteArray>
#include <QDate>
#include <QMap>
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QVariant>
#include <functional>

#include "order.h"
struct cmd_info {
  int order;
  QString name;
  QByteArray cmd;
};
using query_func = std::function<bool(const uint64_t &index,
                                      const QVariantList &val, void *param)>;
class sql : public QObject {
  Q_OBJECT
 public:
  explicit sql(QObject *parent = nullptr);
  virtual ~sql();
  static sql *ins();
  int init();
  bool query(QString sql, query_func func);
  bool query(const QString sql, QVariant &func);
  bool setKey(const QString &s, const QString &k, const QVariant &val,
              const QString &decs);
  bool getValue(const QString &s, const QString &k, QVariant &val,
                QString &decs);
  bool setCmd(const int &order, const QByteArray &cmd, const QString &name, const QString &sn);
  bool getCmd(QMap<QDateTime, cmd_info> &cmd, int limit, int offset);

  bool getTableCounts(const QString &tablename, const QString &where,
                      uint64_t &count);
  const QSqlDatabase &getDb();
  bool getSnList(QStringList &sn_list);
 signals:
 private:
  QSqlDatabase mDb;
  QString mUserName;
  QString mCompute;
  //QMap<uint64_t, uint64_t> mTimes;
  QMap<QString, QMap<uint64_t, uint64_t>> mTimes;
  int mFreq = 60;
  bool mNo = 0;
};
#define mSql sql::ins()
#endif  // SQL_H
