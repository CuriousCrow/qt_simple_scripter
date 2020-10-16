#include "mainscripterwindow.h"
#include <QApplication>
#include <QTranslator>
#include "utils/slogger.h"
#include "utils/appsettings.h"
#include "utils/qfileutils.h"
#include "utils/appconst.h"

int main(int argc, char *argv[])
{  
  QApplication a(argc, argv);

  SLogger log;
  QString logPath = AppSettings::applicationPath() + DEF_LOG_DIR + PATH_DELIMITER + AppSettings::applicationName() + ".log";
  if (AppSettings::contains(SECTION_LOGGER, PRM_LOG_FILE)) {
    logPath = AppSettings::strVal(SECTION_LOGGER, PRM_LOG_FILE);
  }
  QFileUtils::forcePath(QFileUtils::extractDirPath(logPath));
  log.setLogToFile(logPath);
  log.setLogCritical(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_CRITICAL, true));
  log.setLogDebug(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_DEBUG, true));
  log.setLogEventTime(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_EVENT_TIME, true));
  log.setLogFatal(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_FATAL, true));
  log.setLogWarning(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_WARNING, true));
  log.setFilter(AppSettings::strVal(SECTION_LOGGER, PRM_LOG_FILTER, ""));
  log.start();

  QTranslator translator2;
  bool trRes = translator2.load(QLocale::system(), "qtbase", "_");
  if (!trRes) {
    qDebug() << "Translator qtbase not loaded";
  }

  MainScripterWindow w;
  w.showMaximized();

  return a.exec();
}
