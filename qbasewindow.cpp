#include "qbasewindow.h"
#include "ui_qbasewindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

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
  QSettings* settings = QTextProcessor::settings();
  settings->beginGroup("Forms");
  if (settings->contains(objectName())){
    setGeometry(settings->value(objectName()).toRect());    
  }
  else {
    moveToScreenCenter();
  }
  settings->endGroup();
}

void QBaseWindow::saveSettings()
{
  QSettings* settings = QTextProcessor::settings();
  settings->beginGroup("Forms");
  settings->setValue(objectName(), geometry());  
  settings->endGroup();
}

void QBaseWindow::moveToScreenCenter()
{
  QRect desktopRect = QApplication::desktop()->availableGeometry();
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
