#ifndef STATEMENTMODEL_H
#define STATEMENTMODEL_H

#include "models/lsqllinkedrecordsmodel.h"

class PlainStatementField : public LCalcField
{
public:
    PlainStatementField(QString name);

    // LCalcField interface
public:
    virtual QVariant data(int row, int role) override;
};

class StatementModel : public LSqlLinkedRecordsModel
{
    Q_OBJECT
public:
    explicit StatementModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
};

#endif // STATEMENTMODEL_H
