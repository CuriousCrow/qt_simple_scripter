#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>

#define SECTION_DATABASE "database"
#define PRM_TYPE "type"
#define PRM_HOST "host"
#define PRM_PORT "port"
#define PRM_DATABASE_NAME "name"
#define PRM_USERNAME "username"
#define PRM_PASSWORD "password"
#define PRM_BASE_IMPORT_PATH "baseImportPath"

#define SECTION_IMPORT "import"
#define PRM_DELIMITER "delimiter"
#define PRM_EOL "\r\n"

#define SECTION_GENERAL "common"
#define PRM_GRAMMAR_SUPPORT "grammarSupport"
#define PRM_BASE_MEDIA_PATH "baseMediaPath"

#define SECTION_LOGGER "logger"
#define PRM_LOG_FILE "logFile"
#define PRM_LOG_CRITICAL "logCritical"
#define PRM_LOG_DEBUG "logDebug"
#define PRM_LOG_FATAL "logFatal"
#define PRM_LOG_WARNING "logWarning"
#define PRM_LOG_FILTER "filter"

class AppSettings
{
public:
    AppSettings();
    static bool contains(QString section, QString name);
    static QVariant val(QString name, QVariant defValue = QVariant());    
    static QString strVal(QString section, QString name, QVariant defValue = QVariant());
    static int intVal(QString section, QString name, QVariant defValue = QVariant());
    static bool boolVal(QString section, QString name, QVariant defValue = QVariant());
    static QString applicationPath();
    static void sync();
private:
    static QSettings* _settings;
};

#endif // APPSETTINGS_H
