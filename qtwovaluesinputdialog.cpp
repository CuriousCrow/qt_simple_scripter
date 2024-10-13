#include "qtwovaluesinputdialog.h"
#include "ui_qtwovaluesinputdialog.h"

QTwoValuesInputDialog::QTwoValuesInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTwoValuesInputDialog)
{
    ui->setupUi(this);
}

QTwoValuesInputDialog::~QTwoValuesInputDialog()
{
    delete ui;
}

void QTwoValuesInputDialog::setParam1(QString title, QString value)
{
    ui->lblParam1->setText(title);
    ui->edtParam1->setText(value);
}

void QTwoValuesInputDialog::setParam2(QString title, QString value)
{
    ui->lblParam2->setText(title);
    ui->edtParam2->setText(value);
}

QString QTwoValuesInputDialog::param1()
{
    return ui->edtParam1->text();
}

QString QTwoValuesInputDialog::param2()
{
    return ui->edtParam2->text();
}

void QTwoValuesInputDialog::on_btnFlipValues_clicked()
{
    QString value1 = ui->edtParam1->text();
    QString value2 = ui->edtParam2->text();
    ui->edtParam1->setText(value2);
    ui->edtParam2->setText(value1);
}
