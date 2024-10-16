#ifndef PROJECTLISTWINDOW_H
#define PROJECTLISTWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

#include "basewindow.h"
#include "core/datamodule.h"


const QString SMsgProjectImportSuccess = "Проект \"%1\" успешно импортирован.";
const QString SMsgProjectImportError = "Не удалось импортировать проект \"%1\".";
const QString SMsgProjectExportSuccess = "Проект \"%1\" успешно экспортирован.";
const QString SMsgProjectExportError = "Не удалось экспортировать проект \"%1\".";

namespace Ui {
class ProjectListWindow;
}

class ProjectListWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit ProjectListWindow(QWidget *parent = nullptr);
    ~ProjectListWindow();

    static ProjectListWindow* Instance(QWidget *parent = nullptr);
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
    static ProjectListWindow* singletonWindow;
    DataModule* _dm;
    Ui::ProjectListWindow *ui;
    void createImportMenu();
};

#endif // PROJECTLISTWINDOW_H
