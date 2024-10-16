#include "regexptestwindow.h"
#include "ui_regexptestwindow.h"


RegexpTestWindow* RegexpTestWindow::singletonWindow = nullptr;

RegexpTestWindow::RegexpTestWindow(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::RegexpTestWindow)
{
    ui->setupUi(this);
    highlighter = new RegexpHighlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(ui->edtRegexp->text());
}

RegexpTestWindow::~RegexpTestWindow()
{  
    delete ui;
}

RegexpTestWindow *RegexpTestWindow::Instance(QWidget *parent)
{
    if (!singletonWindow){
        singletonWindow = new RegexpTestWindow(parent);
    }
    return singletonWindow;
}

void RegexpTestWindow::on_edtRegexp_textChanged(const QString &arg1)
{
    highlighter->setPattern(arg1);
}
