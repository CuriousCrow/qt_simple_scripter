#ifndef QSTATEMENTMODEL_H
#define QSTATEMENTMODEL_H

#include "models/lsqllinkedrecordsmodel.h"

class PlainStatementField : public LCalcField
{
public:
    PlainStatementField(QString name);

    // LCalcField interface
public:
    virtual QVariant data(int row, int role) override;
};

class QStatementModel : public LSqlLinkedRecordsModel
{
    Q_OBJECT
public:
    explicit QStatementModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
};

#endif // QSTATEMENTMODEL_H
