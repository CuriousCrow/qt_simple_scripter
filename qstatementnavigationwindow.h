#ifndef QSTATEMENTNAVIGATIONWINDOW_H
#define QSTATEMENTNAVIGATIONWINDOW_H

#include "qbasewindow.h"
#include "qdatamodule.h"

namespace Ui {
class QStatementNavigationWindow;
}

class QStatementNavigationWindow : public QBaseWindow
{
    Q_OBJECT

public:
    explicit QStatementNavigationWindow(QWidget *parent = nullptr);
    ~QStatementNavigationWindow();

    static QStatementNavigationWindow* Instance(QWidget *parent);

private slots:
    void on_btnClose_clicked();

    void on_btnClear_clicked();

    void on_lvStatements_doubleClicked(const QModelIndex &index);

private:
    static QStatementNavigationWindow* singletonWindow;
    Ui::QStatementNavigationWindow *ui;
    QDataModule* dm;
};

#endif // QSTATEMENTNAVIGATIONWINDOW_H
