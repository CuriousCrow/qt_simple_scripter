#include "statementhistorydialog.h"
#include "ui_statementhistorydialog.h"
#include <QDebug>


StatementHistoryDialog::StatementHistoryDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::StatementHistoryDialog)
{
  ui->setupUi(this);
  _dm = QDataModule::dm();
  ui->lvStatements->setModel(_dm->mStatementHistory);
  ui->lvStatements->setModelColumn(_dm->mStatementHistory->colIdxByName(SColStatement));

  connect(ui->lvStatements->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
          this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

StatementHistoryDialog::~StatementHistoryDialog()
{
  delete ui;
}

void StatementHistoryDialog::loadHistory(int statementId)
{
  _statementId = statementId;
  _dm->mStatementHistory->setParam("statement_id", _statementId);
  _dm->mStatementHistory->refresh();

  resultStatement.clear();
}

void StatementHistoryDialog::selectionChanged(const QModelIndex &newIndex, const QModelIndex &oldIndex)
{
  std::ignore = oldIndex;
  resultStatement = _dm->mStatementHistory->data(newIndex).toString();
}

void StatementHistoryDialog::on_btnDeleteHistory_clicked()
{
  _dm->deleteStatementHistory(_statementId);
  resultStatement.clear();
  _dm->mStatementHistory->refresh();
}
