#ifndef QSPEAKERWINDOW_H
#define QSPEAKERWINDOW_H

#include <QDataWidgetMapper>
#include <QSqlRecord>
#include "qsqltablemodel.h"
#include "qdatamodule.h"
#include "qbasewindow.h"

namespace Ui {
class QSpeakerWindow;
}

class QSpeakerWindow : public QBaseWindow
{
  Q_OBJECT

public:
  explicit QSpeakerWindow(QWidget *parent = 0);
  ~QSpeakerWindow();

  static QSpeakerWindow* Instance(QWidget* parent = 0);
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

  void initRecord(QSqlRecord &record);
private:
  static QSpeakerWindow* singletonWindow;
  Ui::QSpeakerWindow *ui;
  QDataWidgetMapper* mapper;
  LSqlTableModel* _model;
  QDataModule* _dm;

  void setModel(LSqlTableModel* model);
};

#endif // QSPEAKERWINDOW_H
