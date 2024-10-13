#ifndef QPROJECTEDITWINDOW_H
#define QPROJECTEDITWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include "models/lsqltablemodel.h"
#include <QSqlRecord>
#include "qbasewindow.h"
#include "qdatamodule.h"
#include "widgets/qcheckboxcombo.h"
#include <QDataWidgetMapper>
#include "utils/sparams.h"

namespace Ui {
class QProjectEditWindow;
}

class QProjectEditWindow : public QBaseWindow
{
    Q_OBJECT

public:
    explicit QProjectEditWindow(QWidget *parent = nullptr);
    ~QProjectEditWindow();
    static QProjectEditWindow* Instance(QWidget* parent = nullptr);

    QDataWidgetMapper* projectMapper;
protected:

private slots:
    void on_btnSave_clicked();
    void on_btnClose_clicked();
public slots:
    void onBeforePopup();
private:
    static QProjectEditWindow* singletonWindow;
    QDataModule* dm;
    Ui::QProjectEditWindow *ui;
    QSqlRecord* _record;
    void createControls();
    QString listFileFromScript(QWidget* editor);
    void updateComboItems(QWidget* widget);
};

#endif // QPROJECTEDITWINDOW_H
