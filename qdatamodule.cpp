#include "qdatamodule.h"
#include "utils/slogger.h"
#include <QSqlQuery>
#include <QSqlDriver>
#include <QApplication>
#include <QScreen>
// #include <QDesktopWidget>
#include <QFileInfo>
#include <QDir>
#include "qtextprocessor.h"
#include <QTextCodec>
#include <QDateTime>
#include "utils/qdatabaseupdater.h"
#include "widgets/qsmartdialog.h"
#include "utils/appsettings.h"
#include "utils/qsqlqueryhelper.h"
#include "utils/slogger.h"
#include "utils/strutils.h"
#include "utils/qfileutils.h"
#include "utils/appconst.h"

QDataModule* QDataModule::_dm = nullptr;

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
  checkForUnsavedProject(true);

  saveLastStatement();

  QString filter = "project_id=0 or project_id=%1";
  mSpeakers->setFilter(filter.arg(QString::number(id)));
  mSpeakers->select();

  mStatements->setFilter(filter.arg(QString::number(id)));
  mStatements->select();

  int oldProject = projectId;
  projectId = id;
  //Загрузка названия проекта
  if (projectId > 0) {
    QSqlRecord* rec = mProjects->recordById(id);
    projectTitle = rec->value(SColHeader).toString();
    int speakerType = rec->value(SColSpeakerType).toInt();
    QSqlRecord* speakerTypeRec = mSpeakerTypes->recordById(speakerType);
    if (speakerTypeRec) {
      LOG << "Speaker type found" << speakerType;
      roleTitle = speakerTypeRec->value(SColRoleTitle).toString();
      actorTitle = speakerTypeRec->value(SColActorTitle).toString();
      professionTitle = speakerTypeRec->value(SColProfessionTitle).toString();
    }
  }
  else
    projectTitle.clear();
  speakerTitleCol = roleTitle.isEmpty() ? IDX_ACTOR : IDX_ROLE;

  loadLastStatement();
  emit projectLoaded(oldProject, projectId);
}

bool QDataModule::saveProjectData(bool verbose)
{
  QStringList sqlErrors;
  bool saveResult = mSpeakers->submitAll();
  sqlErrors.append(mSpeakers->sqlErrors());
  saveResult = saveResult && mStatements->submitAll();
  sqlErrors.append(mStatements->sqlErrors());
  if (!sqlErrors.isEmpty()) {
    QString msg = "Произошло %1 ошибок при сохранении проекта!\r\n№1: %2";
    if (verbose)
      QSmartDialog::errorDialog(msg.arg(sqlErrors.count()).arg(sqlErrors.first()));
    QFileUtils::stringToFile(sqlErrors.join("\r\n"), "sql_errors.txt");
  }

  return saveResult;
}

bool QDataModule::backupLocalProject()
{
    if (AppSettings::boolVal("", PRM_LOCAL_PROJECT_BACKUP, false)) {
      QString backupFile = QFileUtils::legalFilename(projectTitle + "_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh_mm") + ".backup");
      INFO << "Backup project" << projectTitle << "to" << backupFile;
      exportSqlTableModel(mStatements, backupFile);
    }
    return true;
}

void QDataModule::checkForUnsavedProject(bool showDialog)
{
  if (!mStatements->isDirty() && !mSpeakers->isDirty())
    return;

  if (!showDialog || QSmartDialog::confirmationDialog(SConfirmSaveModifiedProject)) {
    backupLocalProject();
    int errorRow = QDataModule::dm()->checkStatementLengthExceeded();
    if (errorRow >= 0) {
      int targetRow = mStatementsSmartFiltered->mapFromSource(mStatements->index(errorRow, 0)).row();
      _mapperStatements->setCurrentIndex(targetRow);
      QSmartDialog::errorDialog("Проект не может быть сохранен. Есть реплики с превышением максимального размера реплики");
    }
    else {
      saveProjectData(true);
    }
  }
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

int QDataModule::nextId(QString sequenceName)
{
    QString sql = "select GEN_ID(%1, 1) from rdb$database";
    QSqlQuery query = QSqlQuery(db);
    if (!query.exec(sql.arg(sequenceName))){
        CRITICAL << SErrSequenceFailed.arg(query.lastError().text());
        throw query.lastError().text();
    }
    query.next();
    return query.value(0).toInt();
}

bool QDataModule::execSql(const QString &sql)
{
  LOG << "SQL:" << sql;
  QSqlError error = db.exec(sql).lastError();
  if (error.isValid()) {
    WARNING << "Sql error:" << error.databaseText();
    return false;
  }
  LOG << "Succesfully executed!";
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
  int newRow = mProjects->rowCount() - 1;
  mProjects->setData(mProjects->index(newRow,
                     mProjects->fieldIndex(SColHeader)),
                     fileInfo.baseName());
  mProjects->setData(mProjects->index(newRow,
                     mProjects->fieldIndex(SColPath)),
                     importPath);
  mProjects->setData(mProjects->index(newRow,
                     mProjects->fieldIndex(SColSpeakerType)),
                     0);
  //Если проект не сохранился в БД
  if (!mProjects->submitAll()){
    return false;
  }
  int newProjectId = getLastRecordId(mProjects);
  projectId = newProjectId;

  QString text = QTextProcessor::fileToString(importPath);
  QStringList textLines = text.split(SNewLine, Qt::SkipEmptyParts);

  bool submitted = true;

  QString headerDelimiter = AppSettings::strVal("", SETTING_HEADER_DELIMITER, SETTING_HEADER_DELIMITER_DEF_VAL);

  foreach(QString rawLine, textLines){
    QStringList statementList
        = QTextProcessor::splitStringBySize(rawLine, STATEMENT_SPLIT_SIZE, headerDelimiter);
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

bool QDataModule::importFromXml(QString importPath)
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
  QStringList textLines = text.split(SNewLine, Qt::SkipEmptyParts);

  bool submitted = true;

  QHash<QString, int> newSpeakers;

  foreach(QString xmlStatement, textLines){
    int speakerId = 0;
    QHash<QString, QString> strValHash = StrUtils::tagToHash(xmlStatement, "speach");
    if (strValHash.contains(COL_ROLE)) {
      if (!newSpeakers.contains(strValHash.value(COL_ROLE))) {
        newSpeakerRole = strValHash.value(COL_ROLE);
        newSpeakerProfession = strValHash.value(COL_PROFESSION);
        newSpeakerSex = strValHash.value(COL_SEX);
        mSpeakers->insertRow(mSpeakers->rowCount());
        mSpeakers->submitAll();
        speakerId = getLastRecordId(mSpeakers);
        newSpeakers.insert(strValHash.value(COL_ROLE), speakerId);
      }
      else {
        speakerId = newSpeakers.value(strValHash.value(COL_ROLE));
      }
    }
    else {
      speakerId = 0;
    }
    newStatementSpeaker = speakerId;
    newStatementText = processByReplacePatterns(strValHash.value("text"), PT_IMPORT, false);
    submitted &= mStatements->insertRow(mStatements->rowCount());
    if (!submitted){
      return false;
    }
  }
  if (submitted){
    emit projectLoaded(qlonglong(0), projectId);
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
    WARNING << SErrNoProjectLoaded;
    return false;
  }

  int digitsCount = AppSettings::intVal("", PRM_EXPORT_DIGIT_COUNT, 2);

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
    LOG << i << ":" << currSpeakerId;
    QString statementText = statementRec.value(SColStatement).toString() + SSpace;
    if (!multifileDelimiter.isEmpty() && statementText.trimmed() == multifileDelimiter){
      exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(true), resStatement.trimmed());
      resStatement.clear();
      speakerAttrs.clear();
      QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + PATH_DELIMITER + projectDir + PATH_DELIMITER + projectTitle + "_" + StrUtils::intToStr(fileIndex++, digitsCount) + ".xml");
      exportText.clear();
    }
    else {
      if (currSpeakerId != lastSpeakerId) {
        if (!resStatement.isEmpty())
          exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(true), resStatement.trimmed());
        resStatement.clear();
        speakerAttrs.clear();
        if (currSpeakerId != 0){
          speakerRec = mSpeakers->recordById(currSpeakerId);
          speakerAttrs.setValue(COL_ACTOR, speakerRec->value(SColActor).toString());
          speakerAttrs.setValue(COL_PROFESSION, speakerRec->value(SColProfession).toString());
          speakerAttrs.setValue(COL_ROLE, speakerRec->value(SColSpeachRole).toString());
          speakerAttrs.setValue(COL_SEX, speakerRec->value(SColSex).toString());
        }
      }
      resStatement += processByReplacePatterns(statementText, PT_EXPORT, detailedLogging);
    }
    lastSpeakerId = currSpeakerId;
  }
  exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(true), resStatement.trimmed());
  if (multifileDelimiter.isEmpty()){
    QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + PATH_DELIMITER + projectDir + PATH_DELIMITER + projectTitle + ".xml");
  }
  else {
    result = QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + PATH_DELIMITER + projectDir + PATH_DELIMITER + projectTitle + "_" + StrUtils::intToStr(fileIndex++, digitsCount) + ".xml");
  }

  return result;
}

bool QDataModule::exportSpeakerList(QString outDir)
{
    auto extendedSpeakerExport = AppSettings::boolVal("", PRM_EXTENDED_SPEAKER_EXPORT, false);

  QStringList exportList;
  QSqlRecord rec;
  QString firstActorCol = speakerTitleCol == IDX_ACTOR ? SColActor : SColSpeachRole;
  QString secondActorCol = speakerTitleCol == IDX_ACTOR ? SColSpeachRole : SColActor;
  for(int i=0; i<mSpeakers->rowCount(); i++){
    rec = mSpeakers->record(i);
    QString line = rec.value(firstActorCol).toString();
    if (extendedSpeakerExport) {
        line.append(SDelimiter).append(rec.value(secondActorCol).toString());
    }
    line.append(SDelimiter).append(rec.value(SColBirthYear).toString());
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
  QString newValue = QSmartDialog::inputStringDialog(SSettingEditing, caption, Q_NULLPTR, oldValue);
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
            S_AUDIENCE_AGE << S_AUDIENCE_EDUCATION << S_AUDIENCE_SIZE <<
            S_SPEAKER_TYPE;

  mProjects->setHeaders(titles);
  mProjects->setFilter("id>0");
  mProjects->setSort(S_ID, Qt::AscendingOrder);
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

  mSpeakers = new LSqlTableModel(this, db);
  mSpeakers->setFilter("project_id=0");
  connect(mSpeakers, SIGNAL(beforeInsert(QSqlRecord&)),
          this, SLOT(initNewSpeaker(QSqlRecord&)));
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

  mSpeakerTypes = new LSqlTableModel(this, db);
  mSpeakerTypes->setTable(TABLE_SPEAKER_TYPES);
  mSpeakerTypes->select();

  return true;
}

bool QDataModule::startTransaction()
{
  bool result = db.transaction();
  LOG << "START TRANSACTION" << result;
  return result;
}

bool QDataModule::commitTransaction()
{
  bool result = db.commit();
  LOG << "COMMIT TRANSACTION" << result;
  return result;
}

bool QDataModule::rollbackTransaction()
{
  bool result = db.rollback();
  LOG << "ROLLBACK TRANSACTION" << result;
  return result;
}

bool QDataModule::loadModel(LSqlTableModel* model, QString table, QString sequence)
{
  bool result = false;
  model->setTable(table);
  if (!sequence.isEmpty())
    model->setSequenceName(sequence);
  result = model->select();
  INFO << (result ?
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

int QDataModule::checkStatementLengthExceeded()
{
  int idx = -1;
  for (int row = 0; row < mStatements->rowCount(); row++) {
      int stSize = mStatements->data(row, COL_STATEMENT).toString().size();
      if (stSize > MAX_STATEMENT_SIZE) {
        WARNING << "Statement size exceeded";
        idx = row;
        break;
      }
  }
  return idx;
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
    LOG << "Initial statement:" << statement;
  QSqlRecord patternRec;
  QString result = statement;
  QRegExp rx;
  for(int row = 0; row < mReplacePatterns->rowCount(); row++){
    patternRec = mReplacePatterns->record(row);
    if (patternRec.value(SColUsageType).toInt() == patternType){
      rx.setPattern(patternRec.value(SColRegexp).toString());
      QString pattern = patternRec.value(SColPattern).toString();
      result = rx.replaceIn(result, pattern);
      if (logging)
        LOG << "Processing step" << row + 1 << ":" << result;
    }
  }
  if (logging)
    LOG << "Result statement:" << result;
  return result;
}

void QDataModule::on_autosave_timeout()
{
  QString debugMsg;

  if (projectId == 0){
    debugMsg = SErrNoProjectLoaded;
  }
  else {
    int exceedLengthRow = checkStatementLengthExceeded();
    if (exceedLengthRow >= 0) {
      debugMsg = RESULT_FAILED;
    }
    else if (saveProjectData(false)){
      debugMsg = RESULT_SUCCESS;
    }
    else {
      debugMsg = RESULT_FAILED;
    }
  }  
  INFO << SMsgAutosaving.arg(debugMsg);
}

void QDataModule::initNewSpeaker(QSqlRecord &record)
{
  record.setValue(SColProjectId, _dm->projectId);
  if (!newSpeakerRole.isEmpty())
    record.setValue(SColSpeachRole, newSpeakerRole);
  if (!newSpeakerActor.isEmpty())
    record.setValue(SColActor, newSpeakerActor);
  if (!newSpeakerProfession.isEmpty())
    record.setValue(SColProfession, newSpeakerProfession);
  if (!newSpeakerSex.isEmpty())
    record.setValue(SColSex, newSpeakerSex);
  record.setValue(SColBirthYear, QVariant());

  newSpeakerRole = "";
  newSpeakerActor = "";
  newSpeakerProfession = "";
  newSpeakerSex = "";
}

void QDataModule::onBeforeStatementInsert(QSqlRecord &rec)
{
  rec.setValue(SColProjectId, projectId);
  if (newStatementSpeaker >= 0)
    rec.setValue(SColSpeakerId, newStatementSpeaker);
  rec.setValue(SColStatement, newStatementText);

  newStatementText = "";
  newStatementSpeaker = 0;
}


void moveWidgetToCenter(QWidget *widget)
{
  QList<QScreen*> screens = QGuiApplication::screens();
  QRect desktopRect = screens.first()->availableGeometry();
  widget->move(desktopRect.width()/2 - widget->width()/2,
             desktopRect.height()/2 - widget->height()/2);
}
