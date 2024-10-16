#include "statementhistorydialog.h"
#include "ui_statementhistorydialog.h"

#include "core/appconst.h"


StatementHistoryDialog::StatementHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatementHistoryDialog)
{
    ui->setupUi(this);

    _dm = DataModule::dm();
    ui->lvStatements->setModel(_dm->mStatementHistory);
    ui->lvStatements->setModelColumn(_dm->mStatementHistory->fieldIndex(COL_STATEMENT));

    connect(ui->lvStatements->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

StatementHistoryDialog::~StatementHistoryDialog()
{
    delete ui;
}

void StatementHistoryDialog::loadHistory(int statementId)
{
    QString filter = "statement_id=%1";
    _statementId = statementId;
    _dm->mStatementHistory->setFilter(filter.arg(QString::number(_statementId)));
    _dm->mStatementHistory->select();

    resultStatement.clear();
}

void StatementHistoryDialog::selectionChanged(const QModelIndex &newIndex, const QModelIndex &oldIndex)
{
    Q_UNUSED(oldIndex)

    resultStatement = _dm->mStatementHistory->data(newIndex).toString();
}

void StatementHistoryDialog::on_btnDeleteHistory_clicked()
{
    QString deleteSql = "delete from STATEMENTS_HISTORY where statement_id=%1";
    _dm->mStatementHistory->execQuery(deleteSql.arg(QString::number(_statementId)));
    resultStatement.clear();
    _dm->mStatementHistory->select();
}
