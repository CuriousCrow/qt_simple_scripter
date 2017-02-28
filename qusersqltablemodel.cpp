#include "qusersqltablemodel.h"
#include <QSqlRecord>
#include <QDebug>

QUserSqlTableModel::QUserSqlTableModel(QObject *parent, QSqlDatabase db) :
    LSqlTableModel(parent, db)
{
}

QVariant QUserSqlTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole){
        int value = record(index.row()).value("ID").toInt();
        return value;
    }
    return LSqlTableModel::data(index, role);
}
