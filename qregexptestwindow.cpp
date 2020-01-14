#include "qregexptestwindow.h"
#include "ui_qregexptestwindow.h"
#include <QDebug>

QRegexpTestWindow* QRegexpTestWindow::singletonWindow = nullptr;

QRegexpTestWindow::QRegexpTestWindow(QWidget *parent) :
  QWidget(parent, Qt::Window),
  ui(new Ui::QRegexpTestWindow)
{
  ui->setupUi(this);
  highlighter = new QRegexpHighlighter(this);
  highlighter->setDocument(ui->edtText->document());
  highlighter->setPattern(ui->edtRegexp->text());
}

QRegexpTestWindow::~QRegexpTestWindow()
{  
  delete ui;
}

QRegexpTestWindow *QRegexpTestWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QRegexpTestWindow(parent);    
  }  
  return singletonWindow;
}

void QRegexpTestWindow::on_edtRegexp_textChanged(const QString &arg1)
{
  highlighter->setPattern(arg1);
}
