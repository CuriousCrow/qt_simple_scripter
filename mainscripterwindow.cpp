#include "mainscripterwindow.h"
#include "ui_mainscripterwindow.h"

#include <QWhatsThis>
#include <QFileDialog>

#include "widgets/qsmartdialog.h"
#include "models/lsqltablemodel.h"
#include "utils/slogger.h"
#include "datamodule.h"
#include "projectlistwindow.h"
#include "projecteditwindow.h"
#include "speakerwindow.h"
#include "statementwindow.h"
#include "highlightpatternwindow.h"
#include "patternschemewindow.h"
#include "replacepatternwindow.h"
#include "regexptestwindow.h"
#include "textprocessor.h"

MainScripterWindow::MainScripterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScripterWindow)
{
    ui->setupUi(this);
    baseWindowTitle = windowTitle();
    
    DataModule* dm = DataModule::dm(this);
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
    ProjectListWindow::Instance(this)->show();
}

void MainScripterWindow::on_aEditProject_triggered()
{
    ProjectEditWindow::Instance()->show();
}

void MainScripterWindow::on_aSpeakerList_triggered()
{
    SpeakerWindow::Instance(this)->show();
}

void MainScripterWindow::on_aStatementList_triggered()
{
    StatementWindow::Instance(this)->show();
}

void MainScripterWindow::on_aHighlightPatterns_triggered()
{
    HighlightPatternWindow::Instance(this)->show();
}

void MainScripterWindow::on_aPatternSchemes_triggered()
{
    PatternSchemeWindow::Instance(this)->show();
}

void MainScripterWindow::on_aReplacePatterns_triggered()
{
    ReplacePatternWindow::Instance(this)->show();
}

void MainScripterWindow::on_aTestPattern_triggered()
{
    RegexpTestWindow* regExpWindow = RegexpTestWindow::Instance(this);
    moveWidgetToCenter(regExpWindow);
    regExpWindow->show();
}

void MainScripterWindow::closeEvent(QCloseEvent *)
{
    DataModule::dm(this)->checkForUnsavedProject(true);
}

void MainScripterWindow::on_aAutosaveSetting_triggered()
{
    DataModule::dm(this)->editSetting(SETTING_AUTOSAVE, SPrmAutosavePeriod);
}

void MainScripterWindow::on_aNewProject_triggered()
{
    ProjectListWindow::Instance(this)->show();
    DataModule::dm(this)->mProjects->insertRow(DataModule::dm(this)->mProjects->rowCount());
}

void MainScripterWindow::onProjectLoaded()
{
    QString projectTitle = DataModule::dm()->projectTitle;
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
                                                      QString script = TextProcessor::fileToString(scriptFile);
                                                      INFO << "Executing script from " << scriptFile;
                                                      bool scriptResult = DataModule::dm()->execSqlScript(script);
                                                      QString resultMessage = scriptResult ? "Скрипт успешно выполнен" : "Ошибка при выполнении скрипта";
                                                      QSmartDialog::infoDialog(resultMessage);
}
