#include "qstatementmodel.h"

QStatementModel::QStatementModel(QObject *parent, QSqlDatabase db) :
  LSqlLinkedRecordsModel(parent, db)
{
}

int QStatementModel::columnCount(const QModelIndex &parent) const
{
  return LSqlLinkedRecordsModel::columnCount(parent) + 1;
}

QVariant QStatementModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DisplayRole) {
    if (index.column() == columnCount(index) - 1){
      QString rawText = record(index.row()).value("STATEMENT").toString();
      return rawText.replace("'", "");
    }
  }
  return LSqlLinkedRecordsModel::data(index, role);
}

Qt::ItemFlags QStatementModel::flags(const QModelIndex &index) const
{
  if (index.column() == columnCount(index) - 1){
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }
  return LSqlLinkedRecordsModel::flags(index);
}
