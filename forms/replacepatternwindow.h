#ifndef REPLACEPATTERNWINDOW_H
#define REPLACEPATTERNWINDOW_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include "forms/basewindow.h"
#include "core/datamodule.h"

namespace Ui {
class ReplacePatternWindow;
}

class ReplacePatternWindow : public BaseWindow
{
  Q_OBJECT

public:
  explicit ReplacePatternWindow(QWidget *parent = nullptr);
    ~ReplacePatternWindow();

  static ReplacePatternWindow* Instance(QWidget* parent);
private slots:
  void on_btnClose_clicked();

  void on_btnSubmit_clicked();

  void on_btnAddPattern_clicked();

  void on_btnDeletePattern_clicked();

private:
  static ReplacePatternWindow* singletonWindow;
  QDataModule* dm;
  QDataWidgetMapper* widgetMapper;
  Ui::ReplacePatternWindow *ui;
};

#endif // REPLACEPATTERNWINDOW_H
