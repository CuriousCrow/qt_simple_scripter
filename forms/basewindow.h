#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLayoutItem>
#include "core/textprocessor.h"

namespace Ui {
class BaseWindow;
}

class BaseWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit BaseWindow(QWidget *parent = nullptr);
    ~BaseWindow();
  void setTitle(QString title);
protected:
  void loadSettings();
  void saveSettings();
  //Сделать неактивными все виджеты внутри компановщика
  void setLayoutEditable(QLayout* layout, bool on);
private:
  Ui::BaseWindow *ui;
  void moveToScreenCenter();
  QString _baseTitle;
  // QWidget interface
protected:
  virtual void closeEvent(QCloseEvent *);
  virtual void showEvent(QShowEvent *);
};

#endif // BASEWINDOW_H
