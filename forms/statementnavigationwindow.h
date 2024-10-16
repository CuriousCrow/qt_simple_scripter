#ifndef STATEMENTNAVIGATIONWINDOW_H
#define STATEMENTNAVIGATIONWINDOW_H

#include "basewindow.h"
#include "core/datamodule.h"

namespace Ui {
class StatementNavigationWindow;
}

class StatementNavigationWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit StatementNavigationWindow(QWidget *parent = nullptr);
    ~StatementNavigationWindow();

    static StatementNavigationWindow* Instance(QWidget *parent);

private slots:
    void on_btnClose_clicked();

    void on_btnClear_clicked();

    void on_lvStatements_doubleClicked(const QModelIndex &index);

private:
    static StatementNavigationWindow* singletonWindow;
    Ui::StatementNavigationWindow *ui;
    DataModule* dm;
};

#endif // STATEMENTNAVIGATIONWINDOW_H
