#include "qdatabaseupdater.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>



bool QDatabaseUpdater::updateDatabase(QSqlDatabase db, QString versionTable, QString versionField)
{
  log(SMsgUpdateIsStarting);
  int currentVersion = getDbVersion(db, versionTable, versionField);
  log(SMsgCurrentDbVersion.arg(QString::number(currentVersion)));
  QStringList queryList = initializeUpdateScript();
  while (currentVersion < queryList.size()){
    QSqlQuery query = db.exec(queryList.at(currentVersion));
    if (query.lastError().type() != QSqlError::NoError){
      log(SErrCantUpdateToVer.arg(QString::number(currentVersion + 1)));
      return false;
    }
    setDbVersion(db, versionTable, versionField, ++currentVersion);
    log(SMsgDbUpdatedToVersion.arg(QString::number(currentVersion)));
  }
  log(SMsgUpdateCompleted);
  return true;
}

int QDatabaseUpdater::getDbVersion(QSqlDatabase db, QString versionTable, QString versionField)
{
  QString sql = SQL_GET_VERSION;
  QSqlQuery query = db.exec(sql.arg(versionField, versionTable));
  if (query.lastError().type() != QSqlError::NoError){
    qDebug() << query.lastError().databaseText();
    return -1;
  }
  if (!query.next()){
    log("No record in version table " + versionTable);
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
  return scriptList;
}

void QDatabaseUpdater::log(QString msg)
{
  qDebug() << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss: ") + msg;
}
