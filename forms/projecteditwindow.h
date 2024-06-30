#ifndef PROJECTEDITWINDOW_H
#define PROJECTEDITWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include "forms/basewindow.h"
#include "core/datamodule.h"
#include "widgets/qcheckboxcombo.h"
#include <QDataWidgetMapper>
#include <QJSEngine>
#include "utils/sparams.h"

namespace Ui {
class ProjectEditWindow;
}

class ProjectEditWindow : public BaseWindow
{
  Q_OBJECT

public:
  explicit ProjectEditWindow(QWidget *parent = nullptr);
    ~ProjectEditWindow();
  static ProjectEditWindow* Instance(QWidget* parent = nullptr);

  QDataWidgetMapper* projectMapper;
protected:

private slots:
  void on_btnSave_clicked();
  void on_btnClose_clicked();
public slots:
  void onBeforePopup();
private:
  static ProjectEditWindow* singletonWindow;
  QDataModule* dm;
  Ui::ProjectEditWindow *ui;
  void createControls();
  QString listFileFromScript(QWidget* editor);
  void updateComboItems(QWidget* widget);
};

#endif // PROJECTEDITWINDOW_H
