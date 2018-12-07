#include "qprojectlistwindow.h"
#include "ui_qprojectlistwindow.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QFileDialog>
#include <QMenu>
#include "widgets/qsmartdialog.h"

QProjectListWindow* QProjectListWindow::singletonWindow = 0;

QProjectListWindow::QProjectListWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QProjectListWindow)
{
  ui->setupUi(this);
  setObjectName("QProjectListWindow");

  _dm = QDataModule::dm();

  ui->projectsView->setModel(_dm->mProjects);
  LSqlRecord rec = _dm->mProjects->patternRecord();
  for(int i=0; i<rec.count(); i++){
    QStringList visibleColumns;
    visibleColumns << SColHeader << SColAuthor << SColCreated;
    if (!visibleColumns.contains(rec.fieldName(i), Qt::CaseInsensitive)){
      ui->projectsView->hideColumn(i);
    }
  }
  // При редактировании свойства проекта, ширина колонки подстраивается под содержимое
  connect(_dm->mProjects, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
          this, SLOT(updateColumnSize(QModelIndex)));

  QProjectEditWindow* editWindow = QProjectEditWindow::Instance(parent);
  connect(ui->projectsView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          editWindow->projectMapper, SLOT(setCurrentModelIndex(QModelIndex)));

  createImportMenu();
}

QProjectListWindow::~QProjectListWindow()
{
  delete ui;
}

QProjectListWindow *QProjectListWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QProjectListWindow(parent);
    singletonWindow->ui->projectsView->resizeColumnsToContents();
  }
  return singletonWindow;
}

void QProjectListWindow::on_btnEditProject_clicked()
{
  QProjectEditWindow::Instance()->show();
}

void QProjectListWindow::on_btnClose_clicked()
{
  close();
}

void QProjectListWindow::on_projectsView_doubleClicked(const QModelIndex &index)
{
  QString projectTitle = _dm->mProjects->data(index.row(), SColHeader).toString();
  if (!QSmartDialog::confirmationDialog(SConfirmLoadProject.arg(projectTitle)))
    return;

  int projectId = _dm->mProjects->data(index.row(), SColID).toInt();
  _dm->loadProjectData(projectId);
}

void QProjectListWindow::importProjectFromTxt()
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

void QProjectListWindow::on_btnExportProject_clicked()
{
  if (_dm->exportProject())
    QSmartDialog::infoDialog(SMsgProjectExportSuccess.arg(_dm->projectTitle), this);
  else
    QSmartDialog::errorDialog(SMsgProjectExportError.arg(_dm->projectTitle), this);
}

void QProjectListWindow::on_btnNewProject_clicked()
{
  //Создание нового проекта
  bool insertProjectResult = _dm->mProjects->insertRow(_dm->mProjects->rowCount());
  if (!insertProjectResult){
    qDebug() << SErrWhileProjectCreation;
  }
}

void QProjectListWindow::on_btnDeleteProject_clicked()
{
  //Удаление проекта
  int row = ui->projectsView->currentIndex().row();
  QString project = _dm->mProjects->data(row, SColHeader).toString();
  if (QSmartDialog::confirmationDialog(SConfirmDeleteProject.arg(project)))
    _dm->deleteProject(row);
}

void QProjectListWindow::updateColumnSize(const QModelIndex &index)
{
  int colIdx = index.column();
  if (!ui->projectsView->isColumnHidden(colIdx)){
    qDebug() << "column " << index.column() << "resize";
    ui->projectsView->resizeColumnToContents(index.column());
  }
}

void QProjectListWindow::importProjectFromXml()
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

void QProjectListWindow::createImportMenu()
{
  QMenu* importMenu = new QMenu(ui->btnImportProject);
  importMenu->addAction("Импорт из текстового файла", this, SLOT(importProjectFromTxt()));
  importMenu->addAction("Импорт из XML-файла", this, SLOT(importProjectFromXml()));
  ui->btnImportProject->setMenu(importMenu);
}
