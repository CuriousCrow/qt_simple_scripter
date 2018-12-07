#ifndef QPROJECTLISTWINDOW_H
#define QPROJECTLISTWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "../../../Qt/LSqlTableModel/trunk/lsqltablemodel.h"
#include "qbasewindow.h"
#include "qdatamodule.h"
#include "qprojecteditwindow.h"

const QString SMsgProjectImportSuccess = "Проект \"%1\" успешно импортирован.";
const QString SMsgProjectImportError = "Не удалось импортировать проект \"%1\".";
const QString SMsgProjectExportSuccess = "Проект \"%1\" успешно экспортирован.";
const QString SMsgProjectExportError = "Не удалось экспортировать проект \"%1\".";

namespace Ui {
class QProjectListWindow;
}

class QProjectListWindow : public QBaseWindow
{
  Q_OBJECT

public:
  explicit QProjectListWindow(QWidget *parent = 0);
  ~QProjectListWindow();

  static QProjectListWindow* Instance(QWidget *parent = 0);
private slots:
  void on_btnEditProject_clicked();

  void on_btnClose_clicked();

  void on_projectsView_doubleClicked(const QModelIndex &index);

  void importProjectFromTxt();

  void on_btnExportProject_clicked();

  void on_btnNewProject_clicked();

  void on_btnDeleteProject_clicked();

  void updateColumnSize(const QModelIndex &index);

  void importProjectFromXml();
private:
  static QProjectListWindow* singletonWindow;
  QDataModule* _dm;
  Ui::QProjectListWindow *ui;
  void createImportMenu();
};

#endif // QPROJECTLISTWINDOW_H
