#ifndef SPEAKERWINDOW_H
#define SPEAKERWINDOW_H

#include <QDataWidgetMapper>
#include <QSqlRecord>

#include "core/datamodule.h"
#include "basewindow.h"

namespace Ui {
class SpeakerWindow;
}

class SpeakerWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit SpeakerWindow(QWidget *parent = nullptr);
    ~SpeakerWindow();

    static SpeakerWindow* Instance(QWidget* parent = nullptr);
    LSqlTableModel* model() { return _model; }
    void setEditable(bool on);
public slots:
    void onProjectLoaded(int oldId, int newId);
private slots:
    void on_btnClose_clicked();
    void on_btnSave_clicked();
    void on_btnAddSpeaker_clicked();
    void on_btnDeleteSpeaker_clicked();
    void on_btnAutoAssign_clicked();
    void onSpeakerSelected(QModelIndex idx);
private:
    static SpeakerWindow* singletonWindow;
    Ui::SpeakerWindow *ui;
    QDataWidgetMapper* mapper;
    LSqlTableModel* _model;
    DataModule* _dm;

    void setModel(LSqlTableModel* model);
    void updateSpeakerFields();
};

#endif // SPEAKERWINDOW_H
