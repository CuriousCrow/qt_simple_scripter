#include "qdatamodule.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QDir>
#include "qtextprocessor.h"
#include <QTextCodec>
#include "utils/qdatabaseupdater.h"
#include "widgets/qsmartdialog.h"
#include "utils/appsettings.h"
#include "utils/appconst.h"
#include "utils/qsqlqueryhelper.h"
#include "utils/slogger.h"

QDataModule* QDataModule::_dm = 0;

QDataModule::QDataModule(QObject *parent) :
  QObject(parent)
{
  _appPath = QApplication::applicationDirPath();

  //Создание таймера автосохранения
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(on_autosave_timeout()));
  //Запуск таймера автосохранения
  timer->start(1000 * 60 * AppSettings::intVal("", SETTING_AUTOSAVE, SETTING_AUTOSAVE_DEF_VAL));

  //Инициализация подключения к БД
  if (!initDatabase()){
    QSmartDialog::errorDialog(SErrWhileDbConnection);
    exit(ERR_CODE_DB);
  }

  //Пытаемся обновить БД
  if (!QDatabaseUpdater::updateDatabase(db, DB_VERSION_TABLE, DB_VERSION_FIELD)){
    QSmartDialog::errorDialog(SErrWhileDbUpfate);
    exit(ERR_CODE_DB);
  }

  //Загружаем данные в модели
  loadModels();
}

QDataModule::~QDataModule()
{
  saveLastStatement();
  //Записывает настройки в файл
  AppSettings::sync();
}

QDataModule *QDataModule::dm(QObject* parent)
{
  if (!_dm){
    _dm = new QDataModule(parent);
  }
  return _dm;
}

QString QDataModule::appPath()
{
  return AppSettings::applicationPath();
}

void QDataModule::loadProjectData(int id)
{      
  checkForUnsavedProject();

  saveLastStatement();

  QString filter = "project_id>0 and project_id=%1";
  mSpeakers->setFilter(filter.arg(QString::number(id)));
  mSpeakers->select();

  mStatements->setFilter(filter.arg(QString::number(id)));
  mStatements->select();

  int oldProject = projectId;
  projectId = id;
  //Загрузка названия проекта
  if (projectId > 0)
    projectTitle = mProjects->recordById(id)->value(SColHeader).toString();
  else
    projectTitle.clear();

  loadLastStatement();
  emit projectLoaded(oldProject, projectId);
}

bool QDataModule::saveProjectData()
{
  return mSpeakers->submitAll()
      && mStatements->submitAll();
}

void QDataModule::checkForUnsavedProject()
{
  if (!mStatements->isDirty() && !mSpeakers->isDirty())
    return;

  if (QSmartDialog::confirmationDialog(SConfirmSaveModifiedProject))
    saveProjectData();
}

void QDataModule::saveLastStatement()
{
  //Сохранение номера последней записи
  if (projectId > 0){
    QString section = SETTING_PROJECT_GROUP + QString::number(projectId);
    AppSettings::setVal(section, SETTING_LAST_STATEMENT, _mapperStatements->currentIndex());
    AppSettings::setVal(section, SETTING_TITLE, projectTitle);
  }
}

void QDataModule::loadLastStatement()
{
  if (projectId > 0){
    QString section = SETTING_PROJECT_GROUP + QString::number(projectId);

    int lastStatement = AppSettings::intVal(section, SETTING_LAST_STATEMENT, 0);
    if (lastStatement >= 0){
      _mapperStatements->setCurrentIndex(lastStatement);
    }
    else {
      _mapperStatements->toLast();
    }
  }
}

qlonglong QDataModule::nextId(QString sequenceName)
{
    QString sql = "select GEN_ID(%1, 1) from rdb$database";
    QSqlQuery query = QSqlQuery(db);
    if (!query.exec(sql.arg(sequenceName))){
        qDebug() << SErrSequenceFailed.arg(query.lastError().text());
        throw query.lastError().text();
    }
    query.next();
    return query.value(0).toLongLong();
}

bool QDataModule::execSql(const QString &sql)
{
  qDebug() << "SQL:" << sql;
  QSqlError error = db.exec(sql).lastError();
  if (error.isValid()) {
    qDebug() << "Sql error:" << error.databaseText();
    return false;
  }
  qDebug() << "Succesfully executed!";
  return true;
}

bool QDataModule::execSqlScript(QString script)
{
  if (!startTransaction()) {
    qWarning() << "Transaction begin fails";
    return false;
  }
  QStringList queries = QTextProcessor::splitSqlScript(script);
  foreach (QString query, queries) {
    query.replace("[semicolon]", ";");
    if (!execSql(query)) {
      rollbackTransaction();
      return false;
    }
  }
  commitTransaction();
  return true;
}

bool QDataModule::importProject(QString importPath)
{    
  //Грузим пустой проект, чтобы очистить модели
  loadProjectData(0);

  QFileInfo fileInfo(importPath);
  projectTitle = fileInfo.baseName();

  //Если файл не существует
  if (!fileInfo.exists()){
    return false;
  }

  //Если проект не вставился
  bool insertProjectResult = mProjects->insertRow(mProjects->rowCount());
  if (!insertProjectResult){
    return false;
  }
  mProjects->setData(mProjects->index(mProjects->rowCount() - 1,
                     mProjects->fieldIndex(SColHeader)),
                     fileInfo.baseName());
  mProjects->setData(mProjects->index(mProjects->rowCount() - 1,
                     mProjects->fieldIndex(SColPath)),
                     importPath);
  //Если проект не сохранился в БД
  if (!mProjects->submitAll()){
    return false;
  }
  int newProjectId = getLastRecordId(mProjects);
  projectId = newProjectId;

  QString text = QTextProcessor::fileToString(importPath);
  QStringList textLines = text.split(SNewLine, QString::SkipEmptyParts);

  bool submitted = true;

  QString headerDelimiter = AppSettings::strVal("", SETTING_HEADER_DELIMITER, SETTING_HEADER_DELIMITER_DEF_VAL);

  foreach(QString rawLine, textLines){
    QStringList statementList
        = QTextProcessor::splitStringBySize(rawLine, MAX_STATEMENT_LENGTH, ".", headerDelimiter);
    foreach(QString statement, statementList){
      newStatementText = processByReplacePatterns(statement.trimmed(), PT_IMPORT, false);
      submitted &= mStatements->insertRow(mStatements->rowCount());
      if (!submitted){
        return false;
      }      
    }
  }
  if (submitted){
    emit projectLoaded(0, projectId);
  }
  return submitted;
}

bool QDataModule::deleteProject(int row)
{
  int idColIdx = mProjects->fieldIndex(SColID);
  int projectToBeDeleted = mProjects->data(mProjects->index(row, idColIdx)).toInt();
  bool result = mProjects->removeRow(row);
  if (result){
    //Удаление информации о проекте из настроек
    QString section = "projects/project" + QString::number(projectToBeDeleted);
    AppSettings::remove(section);
  }
  if (result && (projectToBeDeleted == projectId))
    loadProjectData(0);
  return result;
}

bool QDataModule::exportProject()
{
  if (projectId == 0){
    qDebug() << SErrNoProjectLoaded;
    return false;
  }

  QDir dir(appPath());
  QString projectDir = DIR_PROJECTS_ROOT + projectTitle;
  if (!dir.mkpath(projectDir))
    return false;

  QString multifileDelimiter
      = QSmartDialog::inputStringDialog(SExportSetting,
                                        SEnterFileSeparationDelimiter);

  //Экспорт данных по проектам
  exportSqlTableModel(mProjects, appPath() + DIR_PROJECTS_ROOT + "Projects.txt");
  //Экспорт списка говорящих
  exportSpeakerList(appPath() + projectDir);

  QSqlRecord statementRec;
  QSqlRecord* speakerRec;

  bool result = true;
  bool detailedLogging = AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_EXPORT_DETAILS, false);

  int fileIndex = 1;
  int lastSpeakerId = -1;
  int currSpeakerId = -1;
  QString resStatement = "";
  QString exportText = "";  
  SParams speakerAttrs = SParams();
  for(int i=0; i<mStatements->rowCount(); i++){
    statementRec = mStatements->record(i);
    currSpeakerId = statementRec.value(SColSpeakerId).toInt();
    qDebug() << i << ":" << currSpeakerId;
    QString statementText = statementRec.value(SColStatement).toString() + SSpace;
    if (!multifileDelimiter.isEmpty() && statementText.trimmed() == multifileDelimiter){
      exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(), resStatement.trimmed());
      resStatement.clear();
      speakerAttrs.clear();
      QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + "/" + projectDir + "/" + projectTitle + "_" + QString::number(fileIndex++) + ".xml");
      exportText.clear();
    }
    else {
      if (currSpeakerId != lastSpeakerId) {
        if (!resStatement.isEmpty())
          exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(), resStatement.trimmed());
        resStatement.clear();
        speakerAttrs.clear();
        if (currSpeakerId != 0){
          speakerRec = mSpeakers->recordById(currSpeakerId);
          speakerAttrs.setValue(COL_ROLE, speakerRec->value(SColSpeachRole).toString());
          speakerAttrs.setValue(COL_ACTOR, speakerRec->value(SColActor).toString());
          speakerAttrs.setValue(COL_SEX, speakerRec->value(SColSex).toString());
          speakerAttrs.setValue(COL_PROFESSION, speakerRec->value(SColProfession).toString());
        }
      }
      resStatement += processByReplacePatterns(statementText, PT_EXPORT, detailedLogging);
    }
    lastSpeakerId = currSpeakerId;
  }
  exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(), resStatement.trimmed());
  if (multifileDelimiter.isEmpty()){
    QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + "/" + projectDir + "/" + projectTitle + ".xml");
  }
  else {
    result = QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + "/" + projectDir + "/" + projectTitle + "_" + QString::number(fileIndex++) + ".xml");
  }

  return result;
}

bool QDataModule::exportSpeakerList(QString outDir)
{
  QStringList exportList;
  QSqlRecord rec;
  for(int i=0; i<mSpeakers->rowCount(); i++){
    rec = mSpeakers->record(i);
    QString line = rec.value(SColSpeachRole).toString() + SDelimiter + rec.value(SColBirthYear).toString();
    exportList.append(line);
  }
  return QTextProcessor::stringToFile(exportList.join(SNewLine), outDir.append("/Speakers.txt"));
}

bool QDataModule::exportSqlTableModel(LSqlTableModel* model, QString outPath)
{
  QStringList exportList;
  QSqlRecord rec;
  QString headerLine;
  for(int i=0; i<model->columnCount(); i++){
    headerLine.append(model->headerData(i, Qt::Horizontal).toString()).append(SDelimiter);
  }
  exportList.append(headerLine);
  for(int i=0; i<model->rowCount(); i++){
    rec = model->record(i);
    QString line;
    for(int j=0; j<rec.count(); j++){
      line.append(rec.value(j).toString()).append(SDelimiter);
    }
    exportList.append(line);
  }
  return QTextProcessor::stringToFile(exportList.join(SNewLine), outPath);
}

bool QDataModule::editSetting(QString settingName, QString caption)
{
  QString oldValue = AppSettings::val(settingName).toString();
  QString newValue = QSmartDialog::inputStringDialog(SSettingEditing, caption, 0, oldValue);
  if (!newValue.isEmpty()){
    AppSettings::setVal(settingName, newValue);
    QSmartDialog::infoDialog(SChangesAppliesAfterRestart);
  }
  return !newValue.isEmpty();
}

bool QDataModule::initDatabase()
{
  QString dbPath = AppSettings::strVal("", SETTING_DATABASE, "Scripter.fdb");
  db = QSqlDatabase::addDatabase("QIBASE");
  db.setDatabaseName(dbPath);
  db.setUserName("SYSDBA");
  db.setPassword("masterkey");

  bool ok = db.open();
  if (ok) {
    INFO << SDbConnectionSuccess;
    LSqlTableModel::enableLogging(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_SQL, false));
    QSqlQueryHelper::setLogging(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_SQL, false));
  }
  else
    CRITICAL << SErrDatabase << db.lastError().databaseText();
  return ok;
}

bool QDataModule::loadModels()
{
  bool result = true;
  QStringList titles;

  mProjects = new LSqlTableModel(this, db);
  titles << S_ID << S_HEADER << S_PATH << S_AUTHOR << S_SEX << S_BIRTH_YEAR <<
            S_CREATE_YEAR << S_SPHERE << S_TEXT_TYPE << S_TOPIC << S_STYLE <<
            S_AUDIENCE_AGE << S_AUDIENCE_EDUCATION << S_AUDIENCE_SIZE;

  mProjects->setHeaders(titles);
  mProjects->setFilter("id>0");
  result = result && loadModel(mProjects, TABLE_PROJECTS, GEN_PROJECTS);

  mPatterns = new LSqlTableModel(this, db);
  result = result && loadModel(mPatterns, TABLE_PATTERNS, GEN_PATTERNS);

  mSchemes = new LSqlTableModel(this, db);
  result = result && loadModel(mSchemes, TABLE_SCHEMES, GEN_HIGHLIGHT_SCHEMES);

  mSchemePatterns = new LSqlTableModel(this, db);
  mSchemePatterns->addCalcField(new LLookupField(COL_NAME, mPatterns, COL_PATTERN_ID, COL_NAME));
  mSchemePatterns->addCalcField(new LLookupField(COL_PATTERN, mPatterns, COL_PATTERN_ID, COL_PATTERN));
  mSchemePatterns->addCalcField(new LLookupField(COL_HEXCOLOR, mPatterns, COL_PATTERN_ID, COL_HEXCOLOR));
  result = result && loadModel(mSchemePatterns, TABLE_SCHEME_PATTERNS, GEN_SCHEME_PATTERNS);

  mReplacePatterns = new LSqlTableModel(this, db);
  mReplacePatterns->setSort(COL_PRIORITY, Qt::AscendingOrder);
  result = result && loadModel(mReplacePatterns, TABLE_REPLACE_PATTERNS, GEN_REPLACE_PATTERNS);

  mSpeakers = new QUserSqlTableModel(this, db);
  mSpeakers->setFilter("project_id=0");
  result = result && loadModel(mSpeakers, TABLE_SPEAKERS, GEN_SPEAKERS);

  mStatements = new QStatementModel(this, db);
  mStatements->setFilter("project_id=0");
  mStatements->setLinkField(COL_PARENT_ID);

  //Прокси-модель реплик для навигации
  mStatementsNavigation = new QSortFilterProxyModel(this);
  mStatementsNavigation->setSourceModel(mStatements);
  connect(mStatements, SIGNAL(beforeInsert(QSqlRecord&)),
          this, SLOT(onBeforeStatementInsert(QSqlRecord&)));
  result = loadModel(mStatements, TABLE_STATEMENTS, GEN_STATEMENTS);
  mStatementsNavigation->setFilterKeyColumn(mStatements->columnCount(QModelIndex()) - 1);
  mStatementsNavigation->setFilterCaseSensitivity(Qt::CaseInsensitive);

  //Фильтрованная прокси-модель реплик
  mStatementsSmartFiltered = new QStatementFilterModel(this);
  mStatementsSmartFiltered->setSourceModel(mStatements);
  mStatementsSmartFiltered->setModel(mSchemePatterns);
  mStatementsSmartFiltered->setFilterKeyColumn(mStatements->fieldIndex(COL_STATEMENT));

  _mapperStatements = new QDataWidgetMapper(this);
  _mapperStatements->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

  mStatementHistory = new LSqlTableModel(this, db);
  mStatementHistory->setTable(TABLE_STATEMENTS_HISTORY);

  return true;
}

bool QDataModule::startTransaction()
{
  bool result = db.transaction();
  qDebug() << "START TRANSACTION" << result;
  return result;
}

bool QDataModule::commitTransaction()
{
  bool result = db.commit();
  qDebug() << "COMMIT TRANSACTION" << result;
  return result;
}

bool QDataModule::rollbackTransaction()
{
  bool result = db.rollback();
  qDebug() << "ROLLBACK TRANSACTION" << result;
  return result;
}

bool QDataModule::loadModel(LSqlTableModel* model, QString table, QString sequence)
{
  bool result = false;
  model->setTable(table);
  if (!sequence.isEmpty())
    model->setSequenceName(sequence);
  result = model->select();
  qDebug() << (result ?
    SLoadTableResultOK.arg(model->tableName()) : SLoadTableResultFailed.arg(model->tableName()));
  return result;
}

int QDataModule::getLastRecordId(LSqlTableModel *model)
{
  int rowIndex = model->rowCount() - 1;
  if (rowIndex < 0){
    return -1;
  }

  return model->data(model->index(rowIndex, model->fieldIndex(SColID))).toInt();
}

void QDataModule::setTableHeaders(QSqlTableModel *table, QStringList headers)
{
    for (int i=0; i < headers.size(); i++){
        table->setHeaderData(i, Qt::Horizontal, headers.at(i));
    }
}

QString QDataModule::processByReplacePatterns(QString statement, int patternType, bool logging)
{
  if (logging)
    qDebug() << "Initial statement:" << statement;
  QSqlRecord patternRec;
  QString result = statement;
  QRegExp rx;
  for(int row = 0; row < mReplacePatterns->rowCount(); row++){
    patternRec = mReplacePatterns->record(row);
    if (patternRec.value(SColUsageType).toInt() == patternType){
      rx.setPattern(patternRec.value(SColRegexp).toString());
      QString pattern = patternRec.value(SColPattern).toString();
      result = result.replace(rx, pattern);
      if (logging)
        qDebug() << "Processing step" << row + 1 << ":" << result;
    }
  }
  if (logging)
    qDebug() << "Result statement:" << result;
  return result;
}

void QDataModule::on_autosave_timeout()
{
  QString debugMsg;

  if (projectId == 0){
    debugMsg = SErrNoProjectLoaded;
  }
  else {
    if (saveProjectData()){
      debugMsg = RESULT_SUCCESS;
    }
    else {
      debugMsg = RESULT_FAILED;
    }
  }  
  qDebug() << SMsgAutosaving.arg(debugMsg);
}

void QDataModule::onBeforeStatementInsert(QSqlRecord &rec)
{
  rec.setValue(SColProjectId, projectId);
  rec.setValue(SColStatement, newStatementText);
}


void moveWidgetToCenter(QWidget *widget)
{
  QRect desktopRect = QApplication::desktop()->availableGeometry();
  widget->move(desktopRect.width()/2 - widget->width()/2,
             desktopRect.height()/2 - widget->height()/2);
}
