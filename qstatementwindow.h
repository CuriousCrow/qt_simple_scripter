#ifndef QSTATEMENTWINDOW_H
#define QSTATEMENTWINDOW_H

#include "qbasewindow.h"
#include "qdatamodule.h"
#include "utils/sparams.h"
#include <QDataWidgetMapper>
#include <QMenu>
#include "qspeakerwindow.h"
#include "qdatamodule.h"
#include "qlookupitemdelegate.h"
#include "qtablehighlighter.h"
#include "qtextprocessor.h"

namespace Ui {
class QStatementWindow;
}

class QStatementWindow : public QBaseWindow
{
  Q_OBJECT

public:
  explicit QStatementWindow(QWidget *parent = 0);
  ~QStatementWindow();
  static QStatementWindow* Instance(QWidget* parent);

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
public slots:
  void onProjectLoaded(int oldId, int newId);
private:
  static QStatementWindow* singletonWindow;
  QDataModule* dm;
  Ui::QStatementWindow *ui;
  SParams _customActions;
  QDataWidgetMapper* _mapperSpeakers;
  QLookupItemDelegate* _mapperDelegate;
  int tempSpeakerId;
  QTableHighlighter* _highlighter;
  void loadCustomActions();
  QString prepareActionResult(QString actionName, QString actionString);
  void createAddMenu();
  void submitMapperData();

  // QObject interface
public:
  virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // QSTATEMENTWINDOW_H
