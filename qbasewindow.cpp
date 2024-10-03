#include "qbasewindow.h"
#include "ui_qbasewindow.h"
// #include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include "utils/appsettings.h"

QBaseWindow::QBaseWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QBaseWindow)
{
  ui->setupUi(this);
  ui->statusbar->setVisible(false);
}

QBaseWindow::~QBaseWindow()
{
  delete ui;
}

void QBaseWindow::setTitle(QString title)
{
  if (_baseTitle.isEmpty())
    _baseTitle = windowTitle();
  if (title.isEmpty())
    setWindowTitle(_baseTitle);
  else
    setWindowTitle(_baseTitle + " - " + title);
}

void QBaseWindow::loadSettings()
{
  if (AppSettings::contains(SECTION_FORMS, objectName())){
    setGeometry(AppSettings::val(SECTION_FORMS, objectName()).toRect());
  }
  else {
    moveToScreenCenter();
  }
}

void QBaseWindow::saveSettings()
{
  AppSettings::setVal(SECTION_FORMS, objectName(), geometry());
}

void QBaseWindow::moveToScreenCenter()
{
    auto screens = QApplication::screens();
    QRect desktopRect = screens.first()->availableGeometry();
    this->move(desktopRect.width()/2 - this->width()/2,
               desktopRect.height()/2 - this->height()/2);
}

void QBaseWindow::setLayoutEditable(QLayout *layout, bool on)
{
  for(int idx = 0; idx < layout->count(); idx++){
    QLayoutItem* item = layout->itemAt(idx);
    QWidget* widget = item->widget();
    if (widget != 0){
      widget->setEnabled(on);
      continue;
    }
    QLayout* lo = item->layout();
    if (lo != 0){
      setLayoutEditable(lo, on);
    }
  }
}


void QBaseWindow::closeEvent(QCloseEvent *)
{
  saveSettings();
}

void QBaseWindow::showEvent(QShowEvent *)
{
  loadSettings();
}
