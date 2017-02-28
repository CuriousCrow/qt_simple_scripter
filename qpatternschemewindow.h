#ifndef QPATTERNSCHEMEWINDOW_H
#define QPATTERNSCHEMEWINDOW_H

#include <QMainWindow>
#include "qbasewindow.h"
#include "qdatamodule.h"

namespace Ui {
class QPatternSchemeWindow;
}

class QPatternSchemeWindow : public QBaseWindow
{
  Q_OBJECT

public:
  explicit QPatternSchemeWindow(QWidget *parent = 0);
  ~QPatternSchemeWindow();

  static QPatternSchemeWindow* Instance(QWidget* parent = 0);
private slots:
  void on_btnClose_clicked();

  void on_btnAddItem_clicked();

  void on_btnRemoveItem_clicked();

  void on_btnAddScheme_clicked();

private:
  static QPatternSchemeWindow* singletonWindow;
  QDataModule* dm;
  Ui::QPatternSchemeWindow *ui;  
public slots:
  void onBeforePatternInsert(QSqlRecord& rec);
};

#endif // QPATTERNSCHEMEWINDOW_H
