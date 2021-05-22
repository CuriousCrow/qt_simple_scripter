#ifndef MAINSCRIPTERWINDOW_H
#define MAINSCRIPTERWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "../LSqlTableModel/lsqltablemodel.h"
#include "qdatamodule.h"
#include "qprojectlistwindow.h"
#include "qspeakerwindow.h"
#include "qstatementwindow.h"
#include "qhighlightpatternwindow.h"
#include "qpatternschemewindow.h"
#include "qreplacepatternwindow.h"
#include "qregexptestwindow.h"

namespace Ui {
class MainScripterWindow;
}

class MainScripterWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainScripterWindow(QWidget *parent = nullptr);
  ~MainScripterWindow();

private slots:
  void on_btnExit_clicked();

  void on_aExit_triggered();  

  void on_aProjectList_triggered();

  void on_aEditProject_triggered();

  void on_aSpeakerList_triggered();

  void on_aStatementList_triggered();

  void on_aHighlightPatterns_triggered();

  void on_aPatternSchemes_triggered();

  void on_aReplacePatterns_triggered();

  void on_aTestPattern_triggered();

  void on_aAutosaveSetting_triggered();

  void on_aNewProject_triggered();

  void onProjectLoaded();
  void on_aWhatsThis_triggered();

  void on_aExecuteScript_triggered();

private:
  Ui::MainScripterWindow *ui;
  QString baseWindowTitle;

protected:
  virtual void closeEvent(QCloseEvent *);
};

#endif // MAINSCRIPTERWINDOW_H
