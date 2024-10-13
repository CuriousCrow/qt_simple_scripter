#ifndef QTWOVALUESINPUTDIALOG_H
#define QTWOVALUESINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class QTwoValuesInputDialog;
}

class QTwoValuesInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QTwoValuesInputDialog(QWidget *parent = nullptr);
    ~QTwoValuesInputDialog();

    void setParam1(QString title, QString value = "");
    void setParam2(QString title, QString value = "");

    QString param1();
    QString param2();
private slots:
    void on_btnFlipValues_clicked();

private:
    Ui::QTwoValuesInputDialog *ui;
};

#endif // QTWOVALUESINPUTDIALOG_H
