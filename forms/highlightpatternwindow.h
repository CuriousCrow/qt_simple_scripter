#ifndef HIGHLIGHTPATTERNWINDOW_H
#define HIGHLIGHTPATTERNWINDOW_H

#include <QMainWindow>
#include <QDataWidgetMapper>

#include "basewindow.h"
#include "core/datamodule.h"

namespace Ui {
class HighlightPatternWindow;
}

class HighlightPatternWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit HighlightPatternWindow(QWidget *parent = nullptr);
    ~HighlightPatternWindow();

    static HighlightPatternWindow* Instance(QWidget* parent = nullptr);
private slots:
    void on_btnApply_clicked();
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();

    void initRecord(QSqlRecord &record);

private:
    static HighlightPatternWindow* singletonWindow;
    DataModule* dm;
    Ui::HighlightPatternWindow *ui;
    void initColorCombo();
    QDataWidgetMapper* patternMapper;
};

#endif // HIGHLIGHTPATTERNWINDOW_H
