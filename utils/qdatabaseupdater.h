#ifndef QDATABASEUPDATER_H
#define QDATABASEUPDATER_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

#define SQL_SET_VERSION "update %1 set %2=%3"
#define SQL_GET_VERSION "select %1 from %2"
#define FILE_LOG_UPDATER "DbUpdate.log"

const QString SErrCantUpdateToVer = "Error while updating up to version %1";
const QString SMsgDbUpdatedToVersion = "Database successfully updated up to version %1";
const QString SMsgCurrentDbVersion = "Current DB version: %1";
const QString SMsgUpdateIsStarting = "Starting DB update task";
const QString SMsgUpdateCompleted = "Database is up to date";

/**
 * @brief Класс реализующий автоматическое обновление базы данных
 */
class QDatabaseUpdater : public QObject
{
    Q_OBJECT
public:  
        //Запуск обновления БД
    static bool updateDatabase(QSqlDatabase db, QString versionTable, QString versionField);
private:
    //Получает текущую версию БД
    static int getDbVersion(QSqlDatabase db, QString versionTable, QString versionField);
    //Обновляет значение версии в БД
    static void setDbVersion(QSqlDatabase db, QString versionTable, QString versionField, int version);
    //Формирование списка запросов обновления БД
    static QStringList initializeUpdateScript();
signals:

public slots:

};

#endif // QDATABASEUPDATER_H
