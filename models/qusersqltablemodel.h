#ifndef QUSERSQLTABLEMODEL_H
#define QUSERSQLTABLEMODEL_H

#include "../../../Qt/LSqlTableModel/trunk/lsqltablemodel.h"

class QUserSqlTableModel : public LSqlTableModel
{
    Q_OBJECT
public:
    QUserSqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
signals:

public slots:

};

#endif // QUSERSQLTABLEMODEL_H
