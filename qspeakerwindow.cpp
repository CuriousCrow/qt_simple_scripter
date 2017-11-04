#include "qspeakerwindow.h"
#include "ui_qspeakerwindow.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QRegExp>
#include <QSqlQuery>
#include <QInputDialog>
#include <QMessageBox>
#include "widgets/qsmartdialog.h"

#define WARN_CHOOSE_SPEAKER "Для автосвязывания необходимо сначала выбрать говорящего"
#define INPUT_SPEAKER_PATTERN "Задайте регулярное выражение соответвующее говорящему \"%1\""
#define SPEAKER_BINDING_WIZARD "Мастер настройки говорящих"
#define INVALID_REGEXP "Некорректное регулярное выражение. Действие отменено."
#define SPEAKER_BINDING_SUCCESS "Операция прошла успешно. Идентифицировано %1 реплик"
#define ACTION_COMPLETED "Действие завершено"
#define NEW_SPEAKER "Новый говорящий"


QSpeakerWindow* QSpeakerWindow::singletonWindow = 0;

QSpeakerWindow::QSpeakerWindow(QWidget *parent) :
    QBaseWindow(parent),
    ui(new Ui::QSpeakerWindow)
{
    ui->setupUi(this);
    setObjectName("QSpeakerWindow");

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    _dm = QDataModule::dm();
    setModel(_dm->mSpeakers);

    //Связываем сигнал о загрузке проекта к слоту
    connect(_dm, SIGNAL(projectLoaded(int,int)), this, SLOT(onProjectLoaded(int,int)));
    onProjectLoaded(0, _dm->projectId);
}

QSpeakerWindow::~QSpeakerWindow()
{
  delete ui;
}

QSpeakerWindow *QSpeakerWindow::Instance(QWidget* parent)
{
  if (!singletonWindow){
    singletonWindow = new QSpeakerWindow(parent);    
  }
  return singletonWindow;
}

void QSpeakerWindow::setEditable(bool on)
{
  setTitle(on ? _dm->projectTitle : "");

  setLayoutEditable(ui->loBasicActions, on);
  setLayoutEditable(ui->loAdvancedActions, on);
  setLayoutEditable(ui->loFieldWidgets, on);
}

void QSpeakerWindow::onProjectLoaded(int oldId, int newId)
{
  std::ignore = oldId;
  setEditable(newId > 0);
}

void QSpeakerWindow::setModel(LSqlTableModel *model)
{
    _model = model;
    ui->listView->setModel(_model);
    ui->listView->setModelColumn(2);
    mapper->setModel(_model);
    mapper->addMapping(ui->edtSpeachRole, 2);
    mapper->addMapping(ui->edtActor, 6);
    mapper->addMapping(ui->edtProfession, 3);
    mapper->addMapping(ui->edtSex, 4);
    mapper->addMapping(ui->edtBirthYear, 5);
    connect(ui->listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));
}

void QSpeakerWindow::on_btnClose_clicked()
{
    _model->revertAll();
    hide();
}

void QSpeakerWindow::on_btnSave_clicked()
{
    _model->submitAll();
}

void QSpeakerWindow::on_btnAddSpeaker_clicked()
{
  QDataModule::dm()->newSpeakerRole = NEW_SPEAKER;
  _model->insertRow(_model->rowCount());
}

void QSpeakerWindow::on_btnDeleteSpeaker_clicked()
{
    qDebug() << "Deleting row " << ui->listView->currentIndex().row();
    _model->removeRow(ui->listView->currentIndex().row());
}

void QSpeakerWindow::on_btnAutoAssign_clicked()
{
  int speakerIndex = ui->listView->currentIndex().row();
  if (speakerIndex < 0){
    QSmartDialog::warningDialog(WARN_CHOOSE_SPEAKER, this);
    return;
  }
  int speakerID = _model->data(speakerIndex, SColID).toInt();

  QString msg = INPUT_SPEAKER_PATTERN;
  QString pattern =
      QSmartDialog::inputStringDialog(SPEAKER_BINDING_WIZARD,
                                      msg.arg(ui->edtSpeachRole->text()),
                                      this);
  //Пользователь отменил действие
  if (pattern.isEmpty())
    return;

  QRegExp rx(pattern);
  //Проверка регулярного выражения на корректность
  if (!rx.isValid() || rx.isEmpty() || rx.exactMatch("")){
    QSmartDialog::warningDialog(INVALID_REGEXP);
    return;
  }

  int statementAffected = 0;
  for(int i = 0; i < _dm->mStatements->rowCount(); i++){
    QSqlRecord rec = _dm->mStatements->record(i);
    QString statement = rec.value(SColStatement).toString();
    if (rx.indexIn(statement) > -1){
      _dm->mStatements->setData(i, SColSpeakerId, speakerID);
      _dm->mStatements->setData(i, SColStatement, statement.replace(rx, "").trimmed());
      statementAffected++;
    }
  }
  msg = SPEAKER_BINDING_SUCCESS;
  QMessageBox::information(this, ACTION_COMPLETED,
                           msg.arg(QString::number(statementAffected)));
}
