#ifndef QSTATEMENTMODEL_H
#define QSTATEMENTMODEL_H

#include "../../../Qt/LSqlTableModel/trunk/lsqllinkedrecordsmodel.h"

class QStatementModel : public LSqlLinkedRecordsModel
{
  Q_OBJECT
public:
  explicit QStatementModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
signals:

public slots:


  // QAbstractItemModel interface
public:
  virtual int columnCount(const QModelIndex &parent) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // QSTATEMENTMODEL_H
