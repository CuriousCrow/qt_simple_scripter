#include "qstatementnavigationwindow.h"
#include "ui_qstatementnavigationwindow.h"

QStatementNavigationWindow* QStatementNavigationWindow::singletonWindow = 0;

QStatementNavigationWindow::QStatementNavigationWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QStatementNavigationWindow)
{
  ui->setupUi(this);
  dm = QDataModule::dm();
  ui->lvStatements->setModel(dm->mStatementsNavigation);
  ui->lvStatements->setModelColumn(dm->mStatementsNavigation->columnCount() - 1);
  // Редактирование поля фильтра связываем с фильтрацией модели
  connect(ui->edtFilter, SIGNAL(textChanged(QString)),
          dm->mStatementsNavigation, SLOT(setFilterRegExp(QString)));
}

QStatementNavigationWindow::~QStatementNavigationWindow()
{
  delete ui;
}

QStatementNavigationWindow *QStatementNavigationWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QStatementNavigationWindow(parent);
  }
  return singletonWindow;
}

void QStatementNavigationWindow::on_btnClose_clicked()
{
  close();
}

void QStatementNavigationWindow::on_btnClear_clicked()
{
  ui->edtFilter->clear();
}

void QStatementNavigationWindow::on_lvStatements_doubleClicked(const QModelIndex &index)
{
  dm->_mapperStatements->setCurrentModelIndex(dm->mStatementsNavigation->mapToSource(index));
}
