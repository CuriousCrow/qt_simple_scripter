#ifndef QHIGHLIGHTPATTERNWINDOW_H
#define QHIGHLIGHTPATTERNWINDOW_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include "qbasewindow.h"
#include "qdatamodule.h"

namespace Ui {
class QHighlightPatternWindow;
}

class QHighlightPatternWindow : public QBaseWindow
{
  Q_OBJECT

public:
  explicit QHighlightPatternWindow(QWidget *parent = 0);
  ~QHighlightPatternWindow();

  static QHighlightPatternWindow* Instance(QWidget* parent = 0);
private slots:
  void on_btnApply_clicked();
  void on_btnCancel_clicked();
  void on_btnAdd_clicked();
  void on_btnDelete_clicked();

  void initRecord(QSqlRecord &record);

private:
  static QHighlightPatternWindow* singletonWindow;
  QDataModule* dm;
  Ui::QHighlightPatternWindow *ui;
  void initColorCombo();
  QDataWidgetMapper* patternMapper;
};

#endif // QHIGHLIGHTPATTERNWINDOW_H
