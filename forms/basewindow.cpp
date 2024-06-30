#include "basewindow.h"
#include "ui_basewindow.h"
#include <QApplication>
#include <QDebug>
#include "utils/appsettings.h"

BaseWindow::BaseWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::BaseWindow)
{
  ui->setupUi(this);
  ui->statusbar->setVisible(false);
}

BaseWindow::~BaseWindow()
{
  delete ui;
}

void BaseWindow::setTitle(QString title)
{
  if (_baseTitle.isEmpty())
    _baseTitle = windowTitle();
  if (title.isEmpty())
    setWindowTitle(_baseTitle);
  else
    setWindowTitle(_baseTitle + " - " + title);
}

void BaseWindow::loadSettings()
{
  if (AppSettings::contains(SECTION_FORMS, objectName())){
    setGeometry(AppSettings::val(SECTION_FORMS, objectName()).toRect());
  }
  else {
    moveToScreenCenter();
  }
}

void BaseWindow::saveSettings()
{
  AppSettings::setVal(SECTION_FORMS, objectName(), geometry());
}

void BaseWindow::moveToScreenCenter()
{
  //TODO
//  QRect desktopRect = QApplication::desktop()->availableGeometry();
//  this->move(desktopRect.width()/2 - this->width()/2,
//             desktopRect.height()/2 - this->height()/2);
}

void BaseWindow::setLayoutEditable(QLayout *layout, bool on)
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


void BaseWindow::closeEvent(QCloseEvent *)
{
  saveSettings();
}

void BaseWindow::showEvent(QShowEvent *)
{
  loadSettings();
}
