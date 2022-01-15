#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>

#define SECTION_DATABASE "database"
#define PRM_HOST "host"
#define PRM_PORT "port"
#define PRM_DATABASE_NAME "name"
#define PRM_USERNAME "username"
#define PRM_PASSWORD "password"

#define SECTION_FORMS "forms"

#define SECTION_LOGGER "logger"
#define PRM_LOG_FILE "logFile"
#define PRM_LOG_CRITICAL "logCritical"
#define PRM_LOG_DEBUG "logDebug"
#define PRM_LOG_FATAL "logFatal"
#define PRM_LOG_WARNING "logWarning"
#define PRM_LOG_FILTER "filter"
#define PRM_LOG_EVENT_TIME "logEventTime"
#define PRM_LOG_SQL "logSql"
#define PRM_LOG_EXPORT_DETAILS "logExportDetails"

//Общие настройки
#define PRM_EXPORT_DIGIT_COUNT "exportDigitCount"
#define PRM_SHOW_FRAGMENT_NUMBER "showFragmentNumber"
#define PRM_LOCAL_PROJECT_BACKUP "localProjectBackup"
#define PRM_DEFAULT_STATEMENT_DELIMITER "defaultStatementDelimiter"


class AppSettings
{
public:
    AppSettings();
    static bool contains(QString section, QString name);
    static QVariant val(QString section, QString param, QVariant defValue = QVariant());
    static QVariant val(QString key, QVariant defValue = QVariant());
    static void setVal(QString key, QVariant val);
    static void setVal(QString section, QString param, QVariant val);
    static QString strVal(QString section, QString name, QVariant defValue = QVariant());
    static int intVal(QString section, QString name, QVariant defValue = QVariant());
    static bool boolVal(QString section, QString name, QVariant defValue = QVariant());
    static void remove(const QString &key);
    static QString applicationPath();
    static QString applicationName();
    static void sync();
private:
    static QSettings* _settings;
    static void init();
};

#endif // APPSETTINGS_H
