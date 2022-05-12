#include "qstatementwindow.h"
#include "ui_qstatementwindow.h"
#include "utils/slogger.h"
#include <QTextCursor>
#include <QMessageBox>
#include <QTextBrowser>
#include "qstatementhistorydialog.h"
#include "widgets/qsmartdialog.h"
#include "qstatementnavigationwindow.h"
#include "qtwovaluesinputdialog.h"
#include "utils/appsettings.h"
#include "utils/appconst.h"

#define ACT_BTN_PREFIX "ActionButton_"
#define FILE_CUSTOM_ACTIONS "Actions.lst"

#define S_DEFECTIVE_FORM "Дефектная форма"
#define S_DICTIONARY_FORM "Словарная форма"
#define PATTERN_DEFECTIVE_FORM "<distinct form=\"%1\">%2</distinct>"

QStatementWindow* QStatementWindow::singletonWindow = nullptr;

QStatementWindow::QStatementWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QStatementWindow)
{     
  ui->setupUi(this);
  setObjectName("QStatementWindow");

  dm = QDataModule::dm();
  //Связь сигнала о загрузке проекта
  connect(dm, SIGNAL(projectLoaded(int,int)), this, SLOT(onProjectLoaded(int,int)));

  _mapperDelegate = new QLookupItemDelegate(this);

  _mapperSpeakers = new QDataWidgetMapper(this);
  _mapperSpeakers->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

  _highlighter = new QTableHighlighter(this);
  _highlighter->setDocument(ui->memStatement->document());

  //Связываем комбобокс выбора говорящего с моделью
  ui->cmbSpeaker->setModel(dm->mSpeakers);
  ui->cmbSpeaker->setModelColumn(dm->mSpeakers->fieldIndex("SPEACH_ROLE"));

  //Мэппер для навигации по репликам
  dm->_mapperStatements->setModel(dm->mStatementsSmartFiltered);
  _mapperDelegate->setListModel(dm->mSpeakers);
  dm->_mapperStatements->setItemDelegate(_mapperDelegate);
  dm->_mapperStatements->addMapping(ui->cmbSpeaker, dm->mStatements->fieldIndex("SPEAKER_ID"));
  dm->_mapperStatements->addMapping(ui->memStatement, dm->mStatements->fieldIndex("STATEMENT"), "plainText");
  connect(dm->_mapperStatements, SIGNAL(currentIndexChanged(int)),
          this, SLOT(updateActions(int)));
  if (AppSettings::boolVal("", PRM_SHOW_FRAGMENT_NUMBER, false))
    connect(dm->_mapperStatements, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateFragmentNumber()));

  //Мэппер для отображения свойств говорящего
  _mapperSpeakers->setModel(dm->mSpeakers);
  _mapperSpeakers->addMapping(ui->edtBirthYear, dm->mSpeakers->fieldIndex("BIRTH_YEAR"));
  _mapperSpeakers->addMapping(ui->edtProfession, dm->mSpeakers->fieldIndex("PROFESSION"));
  _mapperSpeakers->addMapping(ui->edtSex, dm->mSpeakers->fieldIndex("SEX"));
  connect(ui->cmbSpeaker, SIGNAL(currentIndexChanged(int)),
          _mapperSpeakers, SLOT(setCurrentIndex(int)));

  //Связываем комбобокс выбора схемы с моделью
  ui->cmbScheme->setModel(dm->mSchemes);
  ui->cmbScheme->setModelColumn(dm->mSchemes->fieldIndex("NAME"));

  //Highlighter
  _highlighter->setModel(dm->mSchemePatterns);
  //При изменении шаблона или схемы текст текущей реплики переподсвечивается
  connect(dm->mSchemePatterns, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
          _highlighter, SLOT(rehighlight()));
  connect(dm->mSchemePatterns, SIGNAL(rowsInserted(QModelIndex,int,int)),
          _highlighter, SLOT(rehighlight()));
  connect(dm->mSchemePatterns, SIGNAL(rowsRemoved(QModelIndex,int,int)),
          _highlighter, SLOT(rehighlight()));

  loadCustomActions();

  createAddMenu();

  connect(ui->wtMonitor, SIGNAL(checkMethod(MonitorState&,QString&)),
          this, SLOT(monitorCheck(MonitorState&,QString&)));
  connect(ui->memStatement, SIGNAL(textChanged()),
          ui->wtMonitor, SLOT(update()));

  //Перехват комбинаций клавиш от редактора
  ui->memStatement->installEventFilter(this);
  //Проверка загруженности проекта
  onProjectLoaded(0, dm->projectId);
}

QStatementWindow::~QStatementWindow()
{
  delete ui;
}

QStatementWindow *QStatementWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QStatementWindow(parent);
    singletonWindow->goToLastViewedStatement();
  }
  return singletonWindow;
}

void QStatementWindow::updateMapper()
{
  dm->_mapperStatements->toLast();
}

void QStatementWindow::goToLastViewedStatement()
{
  dm->loadLastStatement();
}

void QStatementWindow::on_btnFirst_clicked()
{
  submitMapperData();
  dm->_mapperStatements->toFirst();
}

void QStatementWindow::on_btnPrevious_clicked()
{
  submitMapperData();
  dm->_mapperStatements->toPrevious();
}

void QStatementWindow::on_btnNext_clicked()
{
  submitMapperData();
  dm->_mapperStatements->toNext();
}

void QStatementWindow::on_btnLast_clicked()
{
  submitMapperData();
  dm->_mapperStatements->toLast();
}

void QStatementWindow::on_btnClose_clicked()
{
  submitMapperData();
  hide();
}

void QStatementWindow::on_btnSpeakers_clicked()
{
  QSpeakerWindow::Instance(parentWidget())->show();
}

void QStatementWindow::on_btnAddDefect_clicked()
{
  QTwoValuesInputDialog twoValDialog(this);
  twoValDialog.setModal(true);
  twoValDialog.setWindowTitle(S_DEFECTIVE_FORM);
  twoValDialog.setParam1(S_DEFECTIVE_FORM, ui->memStatement->textCursor().selectedText());
  twoValDialog.setParam2(S_DICTIONARY_FORM, "");

  if (twoValDialog.exec() != QDialog::Accepted)
    return;

  QString pattern = PATTERN_DEFECTIVE_FORM;
  ui->memStatement->insertPlainText(pattern.arg(twoValDialog.param1(), twoValDialog.param2()));
}

void QStatementWindow::updateActions(int index)
{
  ui->btnNext->setEnabled(index + 1 < dm->mStatementsSmartFiltered->rowCount());
  ui->btnLast->setEnabled(index + 1 < dm->mStatementsSmartFiltered->rowCount());

  ui->btnPrevious->setEnabled(index > 0);
  ui->btnFirst->setEnabled(index > 0);

  ui->btnDelete->setEnabled(index >= 0);
  ui->btnHistory->setEnabled(index >= 0);
  ui->btnAccentuator->setEnabled(index >= 0);

  ui->memStatement->setEnabled(index >= 0);
  ui->cmbSpeaker->setEnabled(index >= 0);
}

void QStatementWindow::on_btnSaveStatements_clicked()
{
  submitMapperData();
  dm->checkForUnsavedProject(false);
}

void QStatementWindow::on_chbHighlighter_clicked()
{
  _highlighter->setEnabled(ui->chbHighlighter->isChecked());
}

void QStatementWindow::on_btnAccentuator_clicked()
{
  QString statementText = ui->memStatement->toPlainText();
  if (statementText.contains("'") && !QSmartDialog::confirmationDialog(SConfirmAccentuatorRun))
    return;
  QString resText = QTextProcessor::processByAccentuator(ui->memStatement->toPlainText());
  ui->memStatement->setPlainText(resText);
}

void QStatementWindow::on_btnHistory_clicked()
{
  QStatementHistoryDialog historyDialog(this);

  int curStatementId = dm->mStatements->record(dm->_mapperStatements->currentIndex()).value("ID").toInt();
  historyDialog.loadHistory(curStatementId);

  //Если была нажата кнопка "ОК" и была выбрана реплика
  if ((historyDialog.exec() == QDialog::Accepted)
      && !historyDialog.resultStatement.isEmpty()){
    ui->memStatement->setPlainText(historyDialog.resultStatement);
  }
}

void QStatementWindow::on_customActionClick()
{
  QPushButton* actBtn = qobject_cast<QPushButton*>(sender());
  int actIndex = actBtn->objectName().replace(ACT_BTN_PREFIX, "", Qt::CaseInsensitive).toInt();
  QString insText = prepareActionResult(actBtn->text(), _customActions.paramValueAt(actIndex).toString());
  if (!insText.isEmpty())
    ui->memStatement->insertPlainText(insText);
}

void QStatementWindow::on_addStatementMenuClicked()
{
  QAction* act = qobject_cast<QAction*>(sender());
  dm->addOper = static_cast<AddOperation>(act->property("Action").toInt());
  QTextCursor cur;
  int currentRow = dm->_mapperStatements->currentIndex();

  if (currentRow < 0 && dm->addOper == Insert)
    dm->addOper = Add;

  switch(dm->addOper){
  case Split:
    //Нет активной реплики
    if (currentRow < 0)
      break;

    INFO << "Action: Split statement";
    cur = ui->memStatement->textCursor();
    if (cur.anchor() == cur.position()){
      QString sourceStr = ui->memStatement->toPlainText();
      QString destStr1 = sourceStr.left(cur.position()).trimmed();
      QString destStr2 = sourceStr.remove(0, cur.position()).trimmed();
      INFO << destStr1 << destStr2 << cur.anchor() << cur.position();
      ui->memStatement->setPlainText(destStr2);
      submitMapperData();

      qlonglong curSpeakerId = dm->mSpeakers->index(ui->cmbSpeaker->currentIndex(), 0).data(Qt::UserRole).toLongLong();

      dm->newStatementSpeaker = 0;
      dm->newStatementText = "";
      dm->mStatementsSmartFiltered->insertRow(currentRow);

      dm->newStatementSpeaker = curSpeakerId;
      dm->newStatementText = destStr1;
      dm->mStatementsSmartFiltered->insertRow(currentRow);

      dm->_mapperStatements->setCurrentIndex(currentRow + 1);
      break;
    }
    else {
      QSmartDialog::warningDialog(SErrCanNotSplitIfSelected, this);
    }
    break;
  case Insert:
    INFO << "Action: Insert statement";
    submitMapperData();
    dm->newStatementSpeaker = 0;
    dm->newStatementText = "";
    dm->mStatementsSmartFiltered->insertRow(currentRow);
    dm->_mapperStatements->setCurrentIndex(currentRow);
    break;
  case Add:
    INFO << "Action: Add statement";
    submitMapperData();
    dm->newStatementSpeaker = 0;
    dm->newStatementText = "";
    dm->mStatementsSmartFiltered->insertRow(dm->mStatementsSmartFiltered->rowCount());
    dm->_mapperStatements->toLast();
    break;
  }
}

void QStatementWindow::loadCustomActions()
{
  //Загрузка пользовательских действий
  QString customActStr =
      QTextProcessor::fileToString(QTextProcessor::configDir() + FILE_CUSTOM_ACTIONS);
  _customActions.add(customActStr, "\r\n", false);

  for(int i=0; i<_customActions.count(); i++){
    QPushButton* actBtn = new QPushButton(this);
    actBtn->setText(_customActions.paramNameAt(i));
    actBtn->setObjectName(ACT_BTN_PREFIX + QString::number(i));
    connect(actBtn, SIGNAL(clicked()), this, SLOT(on_customActionClick()));
    ui->loCustomActions->addWidget(actBtn);
  }
}

QString QStatementWindow::prepareActionResult(QString actionName, QString actionString)
{
  int paramCount = 0;
  QSmartDialog inputDialog(this);
  inputDialog.setWindowTitle("Действие \"" + actionName + "\"");

  QString selectedText = ui->memStatement->textCursor().selectedText();

  //Подсчет кол-ва параметров в шаблоне пользовательского действия
  for(int i=1; i<=5; i++){
    if(actionString.contains("%"+QString::number(i))){
      paramCount++;
      //Добавление редактора параметра
      inputDialog.addParam("Параметр " + QString::number(i), i==1 ? selectedText : "");
    }
    else {
      break;
    }
  }

  //Если в шаблоне нет параметров, возвращаем шаблон без изменений
  if (paramCount == 0){
    return actionString;
  }

  //Запуск диалогового окна. Если "Отмена", возвращаем пустую строку
  if (inputDialog.exec() != QDialog::Accepted){
    return "";
  }

  //Заполнение шаблона значениями параметров
  for (int i=1; i<=paramCount; i++){
    actionString = actionString.arg(inputDialog.valueAt(i-1).toString());
  }
  return actionString;
}

void QStatementWindow::createAddMenu()
{
  QMenu* addMenu = new QMenu(this);
  addMenu->addAction(SActionAddStatement, this, SLOT(on_addStatementMenuClicked()))->setProperty("Action", (int)Add);
  addMenu->addAction(SActionInsertStatement, this, SLOT(on_addStatementMenuClicked()), QKeySequence(Qt::CTRL + Qt::Key_D))->setProperty("Action", (int)Insert);
  addMenu->addAction(SActionSplitStatement, this, SLOT(on_addStatementMenuClicked()), QKeySequence(Qt::CTRL + Qt::Key_S))->setProperty("Action", (int)Split);

  ui->btnAdd->setMenu(addMenu);
}

void QStatementWindow::submitMapperData()
{
  dm->_mapperStatements->submit();
}

void QStatementWindow::updateFragmentNumber()
{
  int curRow = dm->_mapperStatements->currentIndex();
  int srcRow = dm->mStatementsSmartFiltered->mapToSource(dm->mStatementsSmartFiltered->index(curRow, 0)).row();
  int fragments = 1;
  int col = dm->mStatements->fieldIndex("STATEMENT");
  for(int row = 0; row < srcRow; row++) {
    if (dm->mStatements->index(row, col).data().toString().trimmed() == _defStatementDelimiter)
      fragments++;
  }
  statusBar()->showMessage("Эпизод: " + QString::number(fragments));
}

void QStatementWindow::monitorCheck(MonitorState &state, QString &description)
{
  if (ui->memStatement->toPlainText().length() > MAX_STATEMENT_SIZE) {
    state = MonitorState::Error;
    description = "Превышен максимальный размер реплики";
  }
  else {
    state = MonitorState::Ok;
    description = "Всё хорошо";
  }
}

void QStatementWindow::on_btnDelete_clicked()
{
  if (!QSmartDialog::confirmationDialog(SConfirmDeleteStatement))
    return;
  int indexToDelete = dm->_mapperStatements->currentIndex();
  dm->mStatementsSmartFiltered->removeRow(indexToDelete);
  if (indexToDelete > 0){
    dm->_mapperStatements->setCurrentIndex(indexToDelete - 1);
  }
  else {
    if (dm->_mapperStatements->model()->rowCount() > 0){
      dm->_mapperStatements->toFirst();
    }
    else {
      updateActions(-1);
    }
  }

}

void QStatementWindow::onProjectLoaded(int oldId, int newId)
{  
  Q_UNUSED(oldId)

  bool editable = newId > 0;
  setTitle(editable ? dm->projectTitle : "");
  ui->cmbSpeaker->setModelColumn(QDataModule::dm()->speakerTitleCol);

  ui->memStatement->setEnabled(editable);
  setLayoutEditable(ui->loAdvancedActions, editable);
  setLayoutEditable(ui->loBasicActions, editable);
  setLayoutEditable(ui->loCustomActions, editable);
  setLayoutEditable(ui->loTop, editable);
  setLayoutEditable(ui->loNavigation, editable);

  if (editable){
    updateActions(dm->_mapperStatements->currentIndex());
  }
}

void QStatementWindow::on_btnSearch_clicked()
{
  QStatementNavigationWindow::Instance(parentWidget())->show();
}

void QStatementWindow::on_btnRemoveAccent_clicked()
{
  ui->memStatement->setPlainText(ui->memStatement->toPlainText().remove("'"));
}

void QStatementWindow::on_chbFilter_clicked()
{
  dm->mStatementsSmartFiltered->setFiltered(ui->chbFilter->isChecked());
}


bool QStatementWindow::eventFilter(QObject *watched, QEvent *event)
{
  if ((watched == ui->memStatement) && (event->type() == QEvent::KeyPress)) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

    if (keyEvent->key() == Qt::Key_A && keyEvent->modifiers() == Qt::ControlModifier) {
      on_btnAccentuator_clicked();
      return true;
    }
    else if (keyEvent->key() == Qt::Key_Right && keyEvent->modifiers() == Qt::ControlModifier) {
      on_btnNext_clicked();
      return true;
    }
    else if (keyEvent->key() == Qt::Key_Left && keyEvent->modifiers() == Qt::ControlModifier) {
      on_btnPrevious_clicked();
      return true;
    }
  }
  return false;
}
