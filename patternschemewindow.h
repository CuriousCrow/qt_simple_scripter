#ifndef PATTERNSCHEMEWINDOW_H
#define PATTERNSCHEMEWINDOW_H

#include <QMainWindow>

#include "basewindow.h"
#include "datamodule.h"

namespace Ui {
class PatternSchemeWindow;
}

class PatternSchemeWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit PatternSchemeWindow(QWidget *parent = nullptr);
    ~PatternSchemeWindow();

    static PatternSchemeWindow* Instance(QWidget* parent = nullptr);
private slots:
    void on_btnClose_clicked();

    void on_btnAddItem_clicked();

    void on_btnRemoveItem_clicked();

    void on_btnAddScheme_clicked();

private:
    static PatternSchemeWindow* singletonWindow;
    DataModule* dm;
    Ui::PatternSchemeWindow *ui;
public slots:
    void onBeforePatternInsert(QSqlRecord& rec);
    void onBeforeSchemeInsert(QSqlRecord& rec);
};

#endif // PATTERNSCHEMEWINDOW_H
