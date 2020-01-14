#ifndef QBASEWINDOW_H
#define QBASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLayoutItem>
#include "qtextprocessor.h"

namespace Ui {
class QBaseWindow;
}

class QBaseWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QBaseWindow(QWidget *parent = nullptr);
  ~QBaseWindow();
  void setTitle(QString title);
protected:
  void loadSettings();
  void saveSettings();
  //Сделать неактивными все виджеты внутри компановщика
  void setLayoutEditable(QLayout* layout, bool on);
private:
  Ui::QBaseWindow *ui;
  void moveToScreenCenter();
  QString _baseTitle;
  // QWidget interface
protected:
  virtual void closeEvent(QCloseEvent *);
  virtual void showEvent(QShowEvent *);
};

#endif // QBASEWINDOW_H
