#include "speakerwindow.h"
#include "ui_speakerwindow.h"
#include "utils/slogger.h"
#include <QRegularExpression>
#include <QInputDialog>
#include <QMessageBox>
#include <QDate>
#include <QIntValidator>
#include "widgets/qsmartdialog.h"

#define WARN_CHOOSE_SPEAKER "Для автосвязывания необходимо сначала выбрать говорящего"
#define INPUT_SPEAKER_PATTERN "Задайте регулярное выражение соответвующее говорящему \"%1\""
#define SPEAKER_BINDING_WIZARD "Мастер настройки говорящих"
#define INVALID_REGEXP "Некорректное регулярное выражение. Действие отменено."
#define SPEAKER_BINDING_SUCCESS "Операция прошла успешно. Идентифицировано %1 реплик"
#define ACTION_COMPLETED "Действие завершено"
#define NEW_SPEAKER "Новый говорящий"


SpeakerWindow* SpeakerWindow::singletonWindow = nullptr;

SpeakerWindow::SpeakerWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::SpeakerWindow)
{
    ui->setupUi(this);
    setObjectName("QSpeakerWindow");

//    ui->edtBirthYear->setValidator(new QIntValidator(1000, QDate::currentDate().year(), this));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    _dm = QDataModule::dm();
    setModel(_dm->mSpeakers);

    //Связываем сигнал о загрузке проекта к слоту
    connect(_dm, SIGNAL(projectLoaded(int,int)), this, SLOT(onProjectLoaded(int,int)));
    onProjectLoaded(0, _dm->projectId);
}

SpeakerWindow::~SpeakerWindow()
{
  delete ui;
}

SpeakerWindow *SpeakerWindow::Instance(QWidget* parent)
{
  if (!singletonWindow){
      singletonWindow = new SpeakerWindow(parent);    
  }
  return singletonWindow;
}

void SpeakerWindow::setEditable(bool on)
{
  setTitle(on ? _dm->projectTitle : "");

  setLayoutEditable(ui->loBasicActions, on);
  setLayoutEditable(ui->loAdvancedActions, on);
  setLayoutEditable(ui->loFieldWidgets, on);
}

void SpeakerWindow::onProjectLoaded(int oldId, int newId)
{
  Q_UNUSED(oldId)
  setEditable(newId > 0);
  updateSpeakerFields();
}

void SpeakerWindow::setModel(BaseRestTableModel *model)
{
    _model = model;
    ui->listView->setModel(_model);
    ui->listView->setModelColumn(IDX_ROLE);
    mapper->setModel(_model);
    mapper->addMapping(ui->edtRole, IDX_ROLE);
    mapper->addMapping(ui->edtActor, IDX_ACTOR);
    mapper->addMapping(ui->edtProfession, IDX_PROFESSION);
    mapper->addMapping(ui->edtSex, IDX_SEX);
    mapper->addMapping(ui->edtBirthYear, IDX_BIRTH_YEAR);
    connect(ui->listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(onSpeakerSelected(QModelIndex)));
}

void SpeakerWindow::updateSpeakerFields()
{
    QString roleTitle = QDataModule::dm()->roleTitle;
    QString actorTitle = QDataModule::dm()->actorTitle;
    QString professionTitle = QDataModule::dm()->professionTitle;

    ui->lblRole->setVisible(!roleTitle.isEmpty());
    ui->lblRole->setText(roleTitle);
    ui->edtRole->setVisible(!roleTitle.isEmpty());

    ui->lblActor->setVisible(!actorTitle.isEmpty());
    ui->lblActor->setText(actorTitle);
    ui->edtActor->setVisible(!actorTitle.isEmpty());

    ui->lblProfession->setVisible(!professionTitle.isEmpty());
    ui->lblProfession->setText(professionTitle);
    ui->edtProfession->setVisible(!professionTitle.isEmpty());

    ui->listView->setModelColumn(QDataModule::dm()->speakerTitleCol);
}

void SpeakerWindow::on_btnClose_clicked()
{
    _model->revertAll();
    hide();
}

void SpeakerWindow::on_btnSave_clicked()
{
    ui->btnSave->setFocus();
    _model->tryToSubmitAll();
}

void SpeakerWindow::on_btnAddSpeaker_clicked()
{
  if (QDataModule::dm()->speakerTitleCol  == IDX_ROLE)
    QDataModule::dm()->newSpeakerRole = NEW_SPEAKER;
  else if (QDataModule::dm()->speakerTitleCol == IDX_ACTOR)
    QDataModule::dm()->newSpeakerActor = NEW_SPEAKER;
  _model->insertRow(_model->rowCount());
}

void SpeakerWindow::on_btnDeleteSpeaker_clicked()
{
    LOG << "Deleting row " << ui->listView->currentIndex().row();
    _model->removeRow(ui->listView->currentIndex().row());
}

void SpeakerWindow::on_btnAutoAssign_clicked()
{
  int speakerIndex = ui->listView->currentIndex().row();
  if (speakerIndex < 0){
    QSmartDialog::warningDialog(WARN_CHOOSE_SPEAKER, this);
    return;
  }
//  qlonglong speakerID = _model->cellData(speakerIndex, SColID).toLongLong();

  QString msg = INPUT_SPEAKER_PATTERN;
  QString pattern =
      QSmartDialog::inputStringDialog(SPEAKER_BINDING_WIZARD,
                                      msg.arg(ui->edtRole->isVisible() ? ui->edtRole->text() : ui->edtActor->text()),
                                      this);
  //Пользователь отменил действие
  if (pattern.isEmpty())
    return;

  //TODO
//  QRegularExpression rx(pattern);
//  //Проверка регулярного выражения на корректность
//  if (!rx.isValid() || rx.isEmpty() || rx.exactMatch("")){
//    QSmartDialog::warningDialog(INVALID_REGEXP);
//    return;
//  }

  int statementAffected = 0;
//  for(int i = 0; i < _dm->mStatements->rowCount(); i++){
//    QSqlRecord rec = _dm->mStatements->record(i);
//    QString statement = rec.value(SColStatement).toString();
//    if (rx.indexIn(statement) > -1){
//      _dm->mStatements->setData(i, SColSpeakerId, speakerID);
//      _dm->mStatements->setData(i, SColStatement, statement.replace(rx, "").trimmed());
//      statementAffected++;
//    }
//  }
  msg = SPEAKER_BINDING_SUCCESS;
  QMessageBox::information(this, ACTION_COMPLETED,
                           msg.arg(QString::number(statementAffected)));
}

void SpeakerWindow::onSpeakerSelected(QModelIndex idx)
{
  mapper->setCurrentModelIndex(idx);
}
