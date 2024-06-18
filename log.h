#ifndef LOG_H
#define LOG_H
#include "QsLog.h"
#define LOG_PATH "./log/log.txt"
using namespace QsLogging;
class log {
 public:
  static log* getInstance() {
    static log _log;
    return &_log;
  };
  static void setLoggingLevel(const Level& newLevel);
  static void setDestIsFile(const QString& path, const int& maxSize,
                            const int& logCount);
  static void setDestIsCmd();
  virtual ~log();

 private:
 private:
  log(){};
  log(const log&) {}
  log& operator=(const log&) { return *this; }
};

#endif  // LOG_H
