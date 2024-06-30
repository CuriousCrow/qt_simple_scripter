#include "projectlistwindow.h"
#include "ui_projectlistwindow.h"
#include "utils/slogger.h"
#include <QFileDialog>
#include <QMenu>
#include "widgets/qsmartdialog.h"

ProjectListWindow* ProjectListWindow::singletonWindow = nullptr;

ProjectListWindow::ProjectListWindow(QWidget *parent) :
    BaseWindow(parent),
  ui(new Ui::ProjectListWindow)
{
  ui->setupUi(this);
  setObjectName("QProjectListWindow");

  _dm = QDataModule::dm();

  auto model = _dm->mProjects;

  ui->projectsView->setModel(model);

  //Hide all unwanted columns
  QStringList visibleColumns;
  visibleColumns << SColHeader << SColAuthor << SColCreated;
  for(int i=0; i<model->columnCount(); i++) {
      if (!visibleColumns.contains(model->columnNameByIdx(i), Qt::CaseInsensitive)) {
          ui->projectsView->hideColumn(i);
      }
  }

  // При редактировании свойства проекта, ширина колонки подстраивается под содержимое
  connect(_dm->mProjects, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
          this, SLOT(updateColumnSize(QModelIndex)));


//  QProjectEditWindow* editWindow = QProjectEditWindow::Instance(parent);
//  connect(ui->projectsView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//          editWindow->projectMapper, SLOT(setCurrentModelIndex(QModelIndex)));

//  createImportMenu();
}

ProjectListWindow::~ProjectListWindow()
{
  delete ui;
}

ProjectListWindow *ProjectListWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new ProjectListWindow(parent);
    singletonWindow->ui->projectsView->resizeColumnsToContents();
  }
  return singletonWindow;
}

void ProjectListWindow::on_btnEditProject_clicked()
{
  ProjectEditWindow::Instance()->show();
}

void ProjectListWindow::on_btnClose_clicked()
{
  close();
}

void ProjectListWindow::on_projectsView_doubleClicked(const QModelIndex &index)
{
  QString projectTitle = _dm->mProjects->cellData(index.row(), SColHeader).toString();
  if (!QSmartDialog::confirmationDialog(SConfirmLoadProject.arg(projectTitle)))
    return;

  int projectId = _dm->mProjects->cellData(index.row(), SColID).toInt();
  _dm->loadProjectData(projectId);
}

void ProjectListWindow::importProjectFromTxt()
{
  QString importFilePath = QFileDialog::getOpenFileName(this, "Выберете файл проекта в текстовом формате");
  //Если файл не выбран, выходим
  if (importFilePath.isNull()){
    return;
  }
  if (_dm->importProject(importFilePath))
    QSmartDialog::infoDialog(SMsgProjectImportSuccess.arg(_dm->projectTitle), this);
  else
    QSmartDialog::errorDialog(SMsgProjectImportError.arg(_dm->projectTitle), this);
}

void ProjectListWindow::on_btnExportProject_clicked()
{
  if (_dm->exportProject())
    QSmartDialog::infoDialog(SMsgProjectExportSuccess.arg(_dm->projectTitle), this);
  else
    QSmartDialog::errorDialog(SMsgProjectExportError.arg(_dm->projectTitle), this);
}

void ProjectListWindow::on_btnNewProject_clicked()
{
  //Создание нового проекта
  bool insertProjectResult = _dm->mProjects->insertRow(_dm->mProjects->rowCount());
  if (!insertProjectResult){
    CRITICAL << SErrWhileProjectCreation;
  }
}

void ProjectListWindow::on_btnDeleteProject_clicked()
{
  //Удаление проекта
  int row = ui->projectsView->currentIndex().row();
  QString project = _dm->mProjects->cellData(row, SColHeader).toString();
  if (QSmartDialog::confirmationDialog(SConfirmDeleteProject.arg(project)))
    _dm->deleteProject(row);
}

void ProjectListWindow::updateColumnSize(const QModelIndex &index)
{
  int colIdx = index.column();
  if (!ui->projectsView->isColumnHidden(colIdx)){
    ui->projectsView->resizeColumnToContents(index.column());
  }
}

void ProjectListWindow::importProjectFromXml()
{
  QString importFilePath = QFileDialog::getOpenFileName(this, "Выбрать файл проекта в формате XML", "", "XML project (*.xml)");
  //Если файл не выбран, выходим
  if (importFilePath.isNull()){
    return;
  }
  if (_dm->importFromXml(importFilePath))
    QSmartDialog::infoDialog(SMsgProjectImportSuccess.arg(_dm->projectTitle), this);
  else
    QSmartDialog::errorDialog(SMsgProjectImportError.arg(_dm->projectTitle), this);
}

void ProjectListWindow::createImportMenu()
{
  QMenu* importMenu = new QMenu(ui->btnImportProject);
  importMenu->addAction("Импорт из текстового файла", this, SLOT(importProjectFromTxt()));
  importMenu->addAction("Импорт из XML-файла", this, SLOT(importProjectFromXml()));
  ui->btnImportProject->setMenu(importMenu);
}
