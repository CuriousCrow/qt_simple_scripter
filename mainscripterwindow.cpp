#include "mainscripterwindow.h"
#include "ui_mainscripterwindow.h"
#include <QWhatsThis>
#include "qdatamodule.h"
#include <QFileDialog>
#include <QDebug>
#include "qsmartdialog.h"

MainScripterWindow::MainScripterWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainScripterWindow)
{
  ui->setupUi(this);
  baseWindowTitle = windowTitle();

  QDataModule* dm = QDataModule::dm(this);
  connect(dm, SIGNAL(projectLoaded(int,int)), this, SLOT(onProjectLoaded()));
  on_aProjectList_triggered();
}

MainScripterWindow::~MainScripterWindow()
{
  delete ui;
}

void MainScripterWindow::on_btnExit_clicked()
{
  on_aExit_triggered();
}

void MainScripterWindow::on_aExit_triggered()
{
  close();
}

void MainScripterWindow::on_aProjectList_triggered()
{
  QProjectListWindow::Instance(this)->show();
}

void MainScripterWindow::on_aEditProject_triggered()
{
  QProjectEditWindow::Instance()->show();
}

void MainScripterWindow::on_aSpeakerList_triggered()
{
  QSpeakerWindow::Instance(this)->show();
}

void MainScripterWindow::on_aStatementList_triggered()
{
  QStatementWindow::Instance(this)->show();
}

void MainScripterWindow::on_aHighlightPatterns_triggered()
{
  QHighlightPatternWindow::Instance(this)->show();
}

void MainScripterWindow::on_aPatternSchemes_triggered()
{
  QPatternSchemeWindow::Instance(this)->show();
}

void MainScripterWindow::on_aReplacePatterns_triggered()
{
  QReplacePatternWindow::Instance(this)->show();
}

void MainScripterWindow::on_aTestPattern_triggered()
{
  QRegexpTestWindow* regExpWindow = QRegexpTestWindow::Instance(this);
  moveWidgetToCenter(regExpWindow);
  regExpWindow->show();
}

void MainScripterWindow::closeEvent(QCloseEvent *)
{
  QDataModule::dm(this)->checkForUnsavedProject();
}

void MainScripterWindow::on_aAutosaveSetting_triggered()
{
  QDataModule::dm(this)->editSetting(SETTING_AUTOSAVE, SPrmAutosavePeriod);
}

void MainScripterWindow::on_aNewProject_triggered()
{
  QProjectListWindow::Instance(this)->show();
  QDataModule::dm(this)->mProjects->insertRow(QDataModule::dm(this)->mProjects->rowCount());
}

void MainScripterWindow::onProjectLoaded()
{
  QString projectTitle = QDataModule::dm()->projectTitle;
  setWindowTitle(baseWindowTitle + " - " + projectTitle);
}

void MainScripterWindow::on_aWhatsThis_triggered()
{
  QWhatsThis::inWhatsThisMode() ?
        QWhatsThis::leaveWhatsThisMode() : QWhatsThis::enterWhatsThisMode();
}

void MainScripterWindow::on_aExecuteScript_triggered()
{
  QString scriptFile = QFileDialog::getOpenFileName(this, "Запуск SQL скрипта", "", "*.sql");
  QString script = QTextProcessor::fileToString(scriptFile);
  qDebug() << "Executing script from " << scriptFile;
  bool scriptResult = QDataModule::dm()->execSqlScript(script);
  QString resultMessage = scriptResult ? "Скрипт успешно выполнен" : "Ошибка при выполнении скрипта";
  QSmartDialog::infoDialog(resultMessage);
}
