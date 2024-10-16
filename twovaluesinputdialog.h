#ifndef TWOVALUESINPUTDIALOG_H
#define TWOVALUESINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class TwoValuesInputDialog;
}

class TwoValuesInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TwoValuesInputDialog(QWidget *parent = nullptr);
    ~TwoValuesInputDialog();

    void setParam1(QString title, QString value = "");
    void setParam2(QString title, QString value = "");

    QString param1();
    QString param2();
private slots:
    void on_btnFlipValues_clicked();

private:
    Ui::TwoValuesInputDialog *ui;
};

#endif // TWOVALUESINPUTDIALOG_H
