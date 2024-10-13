#include "qstatementhistorydialog.h"
#include "ui_qstatementhistorydialog.h"
#include <QDebug>


QStatementHistoryDialog::QStatementHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QStatementHistoryDialog)
{
    ui->setupUi(this);
    _dm = QDataModule::dm();
    ui->lvStatements->setModel(_dm->mStatementHistory);
    ui->lvStatements->setModelColumn(_dm->mStatementHistory->fieldIndex("STATEMENT"));

    connect(ui->lvStatements->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

QStatementHistoryDialog::~QStatementHistoryDialog()
{
    delete ui;
}

void QStatementHistoryDialog::loadHistory(int statementId)
{
    QString filter = "statement_id=%1";
    _statementId = statementId;
    _dm->mStatementHistory->setFilter(filter.arg(QString::number(_statementId)));
    _dm->mStatementHistory->select();

    resultStatement.clear();
}

void QStatementHistoryDialog::selectionChanged(const QModelIndex &newIndex, const QModelIndex &oldIndex)
{
    std::ignore = oldIndex;
    resultStatement = _dm->mStatementHistory->data(newIndex).toString();
}

void QStatementHistoryDialog::on_btnDeleteHistory_clicked()
{
    QString deleteSql = "delete from STATEMENTS_HISTORY where statement_id=%1";
    _dm->mStatementHistory->execQuery(deleteSql.arg(QString::number(_statementId)));
    resultStatement.clear();
    _dm->mStatementHistory->select();
}
