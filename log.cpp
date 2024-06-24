#include "log.h"

void log::setLoggingLevel(const Level &newLevel) {
  Logger &logger = Logger::instance();
  logger.setLoggingLevel(newLevel);
}

void log::setDestIsFile(const QString &path, const int &maxSize,
                        const int &logCount) {
  Logger &logger = Logger::instance();
  DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                                   path, EnableLogRotation, MaxSizeBytes(maxSize),
                                   MaxOldLogCount(logCount)));
  logger.addDestination(fileDestination);
}

void log::setDestIsCmd() {
  Logger &logger = Logger::instance();
  DestinationPtr debugDestination(
    DestinationFactory::MakeDebugOutputDestination());
  logger.addDestination(debugDestination);
}

log::~log() { QsLogging::Logger::destroyInstance(); }
