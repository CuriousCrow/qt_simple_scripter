#ifndef QSQLQUERYHELPER_H
#define QSQLQUERYHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QSqlError>
#include <QDebug>

#define DEBUG_SQL

class QSqlQueryHelper
{
public:
  QSqlQueryHelper();
  ~QSqlQueryHelper();
  static QSqlQuery execSql(QString sql, QString connectionName = QSqlDatabase::defaultConnection);
  static qlonglong getCurrentSequenceValue(QString sequenceName, QString connectionName = QSqlDatabase::defaultConnection);
  static QVariant getSingleValue(QString sql, QString connectionName = QSqlDatabase::defaultConnection);
  static QString databaseName(QString connection);
  static QString driverName(QString connection);
  static QSqlRecord tableRowInfo(QString table, QString connectionName);
  static QString fillSqlPattern(QString pattern, QObject* object);
  static void fillObjectFromRecord(QObject* object, QSqlRecord& rec);
};

#endif // QSQLQUERYHELPER_H