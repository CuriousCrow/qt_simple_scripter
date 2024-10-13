#include "qsqlqueryhelper.h"

#include <QMetaProperty>
#include <QRegExp>
#include <QSqlRecord>

#include "utils/slogger.h"

bool QSqlQueryHelper::_logging = false;

QSqlQuery QSqlQueryHelper::execSql(QString sql, QString connectionName)
{
  if (_logging)
    LOG << QString("SQL(%1): %2").arg(connectionName, sql);

  QSqlQuery sqlResult = QSqlDatabase::database(connectionName).exec(sql);  
  if (sqlResult.lastError().isValid() && _logging){
    WARNING << "Error" << sqlResult.lastError().text();
  }
  return sqlResult;
}

qlonglong QSqlQueryHelper::getCurrentSequenceValue(QString sequenceName, QString connectionName)
{
    QString sql = "SELECT gen_id(%1, 0) FROM rdb$database";
    return getSingleValue(sql.arg(sequenceName), connectionName).toLongLong();
}

QVariant QSqlQueryHelper::getSingleValue(QString sql, QString connectionName)
{
    QSqlQuery query = execSql(sql, connectionName);
    return query.next() ? query.value(0) : QVariant();
}

QString QSqlQueryHelper::databaseName(QString connection)
{
  QSqlDatabase dbCon = QSqlDatabase::database(connection, false);
  Q_ASSERT(dbCon.isValid());
  return dbCon.databaseName();
}

QString QSqlQueryHelper::driverName(QString connection)
{
  QSqlDatabase dbCon = QSqlDatabase::database(connection, false);
  Q_ASSERT(dbCon.isValid());
  return dbCon.driverName();
}

QSqlRecord QSqlQueryHelper::tableRowInfo(QString table, QString connectionName)
{
  QSqlRecord rec = QSqlDatabase::database(connectionName).record(table);
  LOG << "Table info:" << rec;
  return rec;
}


QString QSqlQueryHelper::fillSqlPattern(QString pattern, QObject *object)
{
  QRegExp rx = QRegExp("#([A-Za-z]+)#");
  QStringList fields;
  int index = 0;
  while (index >= 0){
    index = rx.indexIn(pattern, index);
    if (index >= 0){
      fields.append(rx.cap(1));
      index += rx.cap().length();
    }
  }
  fields.removeDuplicates();
  QString result(pattern);
  foreach(QString field, fields){
    result = result.replace("#"+field+"#", object->property(qPrintable(field)).toString());
  }
  return result;
}

void QSqlQueryHelper::fillObjectFromRecord(QObject *object, QSqlRecord &rec)
{  
  for (int i=0; i<rec.count(); i++){

    object->setProperty(qPrintable(rec.fieldName(i)), rec.value(i));
  }
}

void QSqlQueryHelper::setLogging(bool enable)
{
  _logging = enable;
}

