#ifndef STATEMENTWINDOW_H
#define STATEMENTWINDOW_H

#include "forms/basewindow.h"
#include "core/datamodule.h"
#include "utils/sparams.h"
#include "widgets/colormonitor.h"
#include <QDataWidgetMapper>
#include <QMenu>
#include "forms/speakerwindow.h"
#include "qlookupitemdelegate.h"
#include "core/tablehighlighter.h"
#include "core/textprocessor.h"

namespace Ui {
class StatementWindow;
}

class StatementWindow : public BaseWindow
{
  Q_OBJECT

public:
  explicit StatementWindow(QWidget *parent = nullptr);
    virtual ~StatementWindow() override;
  static StatementWindow* Instance(QWidget* parent);

  void updateMapper();
  void goToLastViewedStatement();
private slots:
  void on_btnFirst_clicked();
  void on_btnPrevious_clicked();
  void on_btnNext_clicked();
  void on_btnLast_clicked();
  void on_btnClose_clicked();
  void on_btnSpeakers_clicked();

  void on_btnAddDefect_clicked();
  void updateActions(int index);
  void on_btnSaveStatements_clicked();
  void on_chbHighlighter_clicked();
  void on_btnAccentuator_clicked();
  void on_btnHistory_clicked();

  void on_customActionClick();
  void on_addStatementMenuClicked();

  void on_btnDelete_clicked();

  void on_btnSearch_clicked();

  void on_btnRemoveAccent_clicked();

  void on_chbFilter_clicked();

  void updateFragmentNumber();

  void monitorCheck(MonitorState &state, QString &description);

public slots:
  void onProjectLoaded(int oldId, int newId);
private:
  static StatementWindow* singletonWindow;
  QDataModule* dm;
  Ui::StatementWindow *ui;
  SParams _customActions;
  QDataWidgetMapper* _mapperSpeakers;
  QLookupItemDelegate* _mapperDelegate;
  int tempSpeakerId;
  QString _defStatementDelimiter = "$";
  QTableHighlighter* _highlighter;
  void loadCustomActions();
  QString prepareActionResult(QString actionName, QString actionString);
  void createAddMenu();
  void submitMapperData();

  // QObject interface
public:
  virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // STATEMENTWINDOW_H
