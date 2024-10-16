#include "qdatabaseupdater.h"

#include "utils/slogger.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>



bool QDatabaseUpdater::updateDatabase(QSqlDatabase db, QString versionTable, QString versionField)
{
    qWarning() << SMsgUpdateIsStarting;
    int currentVersion = getDbVersion(db, versionTable, versionField);
    qWarning() << SMsgCurrentDbVersion.arg(QString::number(currentVersion));
    QStringList queryList = initializeUpdateScript();
    while (currentVersion < queryList.size()){
        QSqlQuery query = db.exec(queryList.at(currentVersion));
        if (query.lastError().type() != QSqlError::NoError){
            CRITICAL << SErrCantUpdateToVer.arg(QString::number(currentVersion + 1));
            return false;
        }
        setDbVersion(db, versionTable, versionField, ++currentVersion);
        INFO << SMsgDbUpdatedToVersion.arg(QString::number(currentVersion));
    }
    INFO << SMsgUpdateCompleted;
    return true;
}

int QDatabaseUpdater::getDbVersion(QSqlDatabase db, QString versionTable, QString versionField)
{
    QString sql = SQL_GET_VERSION;
    QSqlQuery query = db.exec(sql.arg(versionField, versionTable));
    if (query.lastError().type() != QSqlError::NoError){
        CRITICAL << "Error while updating DB: " << query.lastError().databaseText();
        return -1;
    }
    if (!query.next()){
        CRITICAL << "No record in version table " << versionTable;
        return -1;
    }
    return query.value(versionField).toInt();
}

void QDatabaseUpdater::setDbVersion(QSqlDatabase db, QString versionTable, QString versionField, int version)
{
    QString sql = SQL_SET_VERSION;
    db.exec(sql.arg(versionTable, versionField, QString::number(version)));
}

QStringList QDatabaseUpdater::initializeUpdateScript()
{
    QStringList scriptList;
    //1
    scriptList.append("select * from sys_params");
    //2
    scriptList.append("select * from sys_params");
    //3
    scriptList.append("select * from sys_params");
    //4
    scriptList.append("select * from sys_params");
    //5
    scriptList.append("select * from sys_params");
    //6
    scriptList.append("select * from sys_params");
    //7
    scriptList.append("select * from sys_params");
    //8
    scriptList.append("select * from sys_params");
    //9 Максимальный размер реплики 5000 символов
    scriptList.append("ALTER TABLE statements ALTER statement TYPE varchar(5000)");
    //10
    scriptList.append("CREATE TABLE SPEAKER_TYPES ("
                      "ID                INTEGER NOT NULL,"
                      "NAME              VARCHAR(100) NOT NULL,"
                      "ROLE_TITLE        VARCHAR(30),"
                      "ACTOR_TITLE       VARCHAR(30),"
                      "PROFESSION_TITLE  VARCHAR(30))");
    //11
    scriptList.append("ALTER TABLE SPEAKER_TYPES ADD CONSTRAINT PK_SPEAKER_TYPES PRIMARY KEY (ID)");
    //12
    scriptList.append("INSERT INTO SPEAKER_TYPES (ID, NAME, ROLE_TITLE, ACTOR_TITLE, PROFESSION_TITLE) "
                      "VALUES (0, 'Все поля', 'Роль', 'Актер', 'Профессия')");
    //13
    scriptList.append("INSERT INTO SPEAKER_TYPES (ID, NAME, ROLE_TITLE, ACTOR_TITLE, PROFESSION_TITLE) "
                      "VALUES (2, 'По ролям', 'Роль', 'Актер', NULL)");
    //14
    scriptList.append("INSERT INTO SPEAKER_TYPES (ID, NAME, ROLE_TITLE, ACTOR_TITLE, PROFESSION_TITLE) "
                      "VALUES (1, 'Без ролей', NULL, 'Имя', 'Профессия')");
    //15
    scriptList.append("ALTER TABLE projects ADD SPEAKER_TYPE_ID INTEGER DEFAULT 0 NOT NULL;");
    //16
    scriptList.append("ALTER TABLE PROJECTS "
                      "ADD CONSTRAINT FK_PROJECTS_SPEAKER_TYPE "
                      "FOREIGN KEY (SPEAKER_TYPE_ID) "
                      "REFERENCES SPEAKER_TYPES(ID) "
                      "ON DELETE SET DEFAULT "
                      "ON UPDATE CASCADE");
    return scriptList;
}
