#include "statementnavigationwindow.h"
#include "ui_statementnavigationwindow.h"


StatementNavigationWindow* StatementNavigationWindow::singletonWindow = nullptr;

StatementNavigationWindow::StatementNavigationWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::StatementNavigationWindow)
{
    ui->setupUi(this);
    
    dm = DataModule::dm();
    ui->lvStatements->setModel(dm->mStatementsNavigation);
    ui->lvStatements->setModelColumn(dm->mStatementsNavigation->columnCount() - 1);
    // Редактирование поля фильтра связываем с фильтрацией модели
    connect(ui->edtFilter, SIGNAL(textChanged(QString)),
            dm->mStatementsNavigation, SLOT(setFilterRegularExpression(QString)));
}

StatementNavigationWindow::~StatementNavigationWindow()
{
    delete ui;
}

StatementNavigationWindow *StatementNavigationWindow::Instance(QWidget *parent)
{
    if (!singletonWindow){
        singletonWindow = new StatementNavigationWindow(parent);
    }
    return singletonWindow;
}

void StatementNavigationWindow::on_btnClose_clicked()
{
    close();
}

void StatementNavigationWindow::on_btnClear_clicked()
{
    ui->edtFilter->clear();
}

void StatementNavigationWindow::on_lvStatements_doubleClicked(const QModelIndex &index)
{  
    dm->_mapperStatements->setCurrentIndex(dm->mStatementsNavigation->mapToSource(index).row());
}
