#ifndef QSTATEMENTHISTORYDIALOG_H
#define QSTATEMENTHISTORYDIALOG_H

#include <QDialog>
#include "qdatamodule.h"

namespace Ui {
class QStatementHistoryDialog;
}

class QStatementHistoryDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QStatementHistoryDialog(QWidget *parent = 0);
  ~QStatementHistoryDialog();

  void loadHistory(int statementId);
  QString resultStatement;
private slots:
  void selectionChanged(const QModelIndex &newIndex, const QModelIndex &oldIndex);
  void on_btnDeleteHistory_clicked();

private:
  Ui::QStatementHistoryDialog *ui;
  QDataModule* _dm;
  int _statementId = 0;
};

#endif // QSTATEMENTHISTORYDIALOG_H
