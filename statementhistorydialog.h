#ifndef STATEMENTHISTORYDIALOG_H
#define STATEMENTHISTORYDIALOG_H

#include <QDialog>

#include "datamodule.h"

namespace Ui {
class StatementHistoryDialog;
}

class StatementHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatementHistoryDialog(QWidget *parent = nullptr);
    ~StatementHistoryDialog();

    void loadHistory(int statementId);
    QString resultStatement;
private slots:
    void selectionChanged(const QModelIndex &newIndex, const QModelIndex &oldIndex);
    void on_btnDeleteHistory_clicked();

private:
    Ui::StatementHistoryDialog *ui;
    DataModule* _dm;
    int _statementId = 0;
};

#endif // STATEMENTHISTORYDIALOG_H
