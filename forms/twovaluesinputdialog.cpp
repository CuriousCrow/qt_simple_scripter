#include "twovaluesinputdialog.h"
#include "ui_twovaluesinputdialog.h"

TwoValuesInputDialog::TwoValuesInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TwoValuesInputDialog)
{
    ui->setupUi(this);
}

TwoValuesInputDialog::~TwoValuesInputDialog()
{
    delete ui;
}

void TwoValuesInputDialog::setParam1(QString title, QString value)
{
    ui->lblParam1->setText(title);
    ui->edtParam1->setText(value);
}

void TwoValuesInputDialog::setParam2(QString title, QString value)
{
    ui->lblParam2->setText(title);
    ui->edtParam2->setText(value);
}

QString TwoValuesInputDialog::param1()
{
    return ui->edtParam1->text();
}

QString TwoValuesInputDialog::param2()
{
    return ui->edtParam2->text();
}

void TwoValuesInputDialog::on_btnFlipValues_clicked()
{
    QString value1 = ui->edtParam1->text();
    QString value2 = ui->edtParam2->text();
    ui->edtParam1->setText(value2);
    ui->edtParam2->setText(value1);
}
