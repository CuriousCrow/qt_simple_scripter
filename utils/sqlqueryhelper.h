#ifndef SQLQUERYHELPER_H
#define SQLQUERYHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include <QSqlError>

#define DEBUG_SQL

class SqlQueryHelper
{
public:
  static QSqlQuery execSql(QString sql, QString connectionName = QSqlDatabase::defaultConnection);
  static qlonglong getCurrentSequenceValue(QString sequenceName, QString connectionName = QSqlDatabase::defaultConnection);
  static QVariant getSingleValue(QString sql, QString connectionName = QSqlDatabase::defaultConnection);
  static QString databaseName(QString connection);
  static QString driverName(QString connection);
  static QSqlRecord tableRowInfo(QString table, QString connectionName);
  static QString fillSqlPattern(QString pattern, QObject* object);
  static void fillObjectFromRecord(QObject* object, QSqlRecord& rec);
  static void setLogging(bool enable);
private:
  static bool _logging;
};

#endif // SQLQUERYHELPER_H
