#include "qstatementmodel.h"

#include "core/appconst.h"

QStatementModel::QStatementModel(QObject *parent, QSqlDatabase db) :
    LSqlLinkedRecordsModel(parent, db)
{
    addCalcField(new PlainStatementField(COL_STATEMENT_PLAIN));
}

PlainStatementField::PlainStatementField(QString name) : LCalcField(name)
{
}

QVariant PlainStatementField::data(int row, int role)
{
    if (role == Qt::DisplayRole){
        QString rawText = modelData(row, COL_STATEMENT, role).toString();
        return rawText.replace("'", "").replace("\u0301", "");
    }
    return QVariant();
}
