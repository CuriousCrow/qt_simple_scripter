#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QLayoutItem>

namespace Ui {
class QBaseWindow;
}

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseWindow(QWidget *parent = nullptr);
    ~BaseWindow();
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

#endif // BASEWINDOW_H
