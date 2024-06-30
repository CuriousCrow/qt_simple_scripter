#include "datamodule.h"
#include "utils/slogger.h"
#include <QApplication>
#include <QScreen>
#include <QFileInfo>
#include <QDir>
#include "textprocessor.h"
#include <QTextCodec>
#include <QDateTime>
#include "widgets/qsmartdialog.h"
#include "utils/appsettings.h"
#include "utils/slogger.h"
#include "utils/strutils.h"
#include "utils/fileutils.h"
#include "core/appconst.h"

QDataModule* QDataModule::_dm = nullptr;

QDataModule::QDataModule(QObject *parent) :
  QObject(parent)
{
  _appPath = QApplication::applicationDirPath();

  //TODO
//  //Создание таймера автосохранения
//  QTimer* timer = new QTimer(this);
//  connect(timer, SIGNAL(timeout()), this, SLOT(on_autosave_timeout()));
//  //Запуск таймера автосохранения
//  timer->start(1000 * 60 * AppSettings::intVal("", SETTING_AUTOSAVE, SETTING_AUTOSAVE_DEF_VAL));


  //Загружаем данные в модели
  loadModels();

  actDeleteStatementHistory = new BaseJsonAction(_nam, "http://localhost/api/v1/statementHistory");
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
//  checkForUnsavedProject(true);

//  saveLastStatement();

  mSpeakers->setParam("project_id", id);
  mSpeakers->refresh();

  mStatements->setParam("project_id", id);
  mStatements->refresh();

  int oldProject = projectId;
  projectId = id;
  //Загрузка названия проекта
//  if (projectId > 0) {
//    QSqlRecord* rec = mProjects->recordById(id);
//    projectTitle = rec->value(SColHeader).toString();
//    int speakerType = rec->value(SColSpeakerType).toInt();
//    QSqlRecord* speakerTypeRec = mSpeakerTypes->recordById(speakerType);
//    if (speakerTypeRec) {
//      LOG << "Speaker type found" << speakerType;
//      roleTitle = speakerTypeRec->value(SColRoleTitle).toString();
//      actorTitle = speakerTypeRec->value(SColActorTitle).toString();
//      professionTitle = speakerTypeRec->value(SColProfessionTitle).toString();
//    }
//  }
//  else
//    projectTitle.clear();
//  speakerTitleCol = roleTitle.isEmpty() ? IDX_ACTOR : IDX_ROLE;

//  loadLastStatement();
  emit projectLoaded(oldProject, projectId);
}

bool QDataModule::saveProjectData(bool verbose)
{
  Q_UNUSED(verbose)
  bool saveResult = mSpeakers->submit();
  saveResult = saveResult && mStatements->submit();

  return saveResult;
//  QStringList sqlErrors;
//  bool saveResult = mSpeakers->submitAll();
//  sqlErrors.append(mSpeakers->sqlErrors());
//  saveResult = saveResult && mStatements->submitAll();
//  sqlErrors.append(mStatements->sqlErrors());
//  if (!sqlErrors.isEmpty()) {
//    QString msg = "Произошло %1 ошибок при сохранении проекта!\r\n№1: %2";
//    if (verbose)
//      QSmartDialog::errorDialog(msg.arg(sqlErrors.count()).arg(sqlErrors.first()));
//    QFileUtils::stringToFile(sqlErrors.join("\r\n"), "sql_errors.txt");
//  }

//  return saveResult;
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
    Q_UNUSED(showDialog)
//  if (!mStatements->isDirty() && !mSpeakers->isDirty())
//    return;

  //TODO:
//  if (!showDialog || QSmartDialog::confirmationDialog(SConfirmSaveModifiedProject)) {
//    backupLocalProject();
//    int errorRow = QDataModule::dm()->checkStatementLengthExceeded();
//    if (errorRow >= 0) {
//      int targetRow = mStatementsSmartFiltered->mapFromSource(mStatements->index(errorRow, 0)).row();
//      _mapperStatements->setCurrentIndex(targetRow);
//      QSmartDialog::errorDialog("Проект не может быть сохранен. Есть реплики с превышением максимального размера реплики");
//    }
//    else {
//      saveProjectData(true);
//    }
//  }
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
  Q_UNUSED(sequenceName)
  return 1;
  //TODO
//    QString sql = "select GEN_ID(%1, 1) from rdb$database";
//    QSqlQuery query = QSqlQuery(db);
//    if (!query.exec(sql.arg(sequenceName))){
//        CRITICAL << SErrSequenceFailed.arg(query.lastError().text());
//        throw query.lastError().text();
//    }
//    query.next();
//    return query.value(0).toInt();
}

//bool QDataModule::execSql(const QString &sql)
//{
//  LOG << "SQL:" << sql;
//  QSqlError error = db.exec(sql).lastError();
//  if (error.isValid()) {
//    WARNING << "Sql error:" << error.databaseText();
//    return false;
//  }
//  LOG << "Succesfully executed!";
//  return true;
//}

//bool QDataModule::execSqlScript(QString script)
//{
//  if (!startTransaction()) {
//    qWarning() << "Transaction begin fails";
//    return false;
//  }
//  QStringList queries = QTextProcessor::splitSqlScript(script);
//  foreach (QString query, queries) {
//    query.replace("[semicolon]", ";");
//    if (!execSql(query)) {
//      rollbackTransaction();
//      return false;
//    }
//  }
//  commitTransaction();
//  return true;
//}

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
                     mProjects->colIdxByName(SColHeader)),
                     fileInfo.baseName());
  mProjects->setData(mProjects->index(newRow,
                     mProjects->colIdxByName(SColPath)),
                     importPath);
  mProjects->setData(mProjects->index(newRow,
                     mProjects->colIdxByName(SColSpeakerType)),
                     0);
  //Если проект не сохранился в БД
  if (!mProjects->tryToSubmitAll()){
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
                     mProjects->colIdxByName(SColHeader)),
                     fileInfo.baseName());
  mProjects->setData(mProjects->index(mProjects->rowCount() - 1,
                     mProjects->colIdxByName(SColPath)),
                     importPath);
  //Если проект не сохранился в БД
  if (!mProjects->tryToSubmitAll()){
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
    if (strValHash.contains(ATTR_ROLE)) {
      if (!newSpeakers.contains(strValHash.value(ATTR_ROLE))) {
        newSpeakerRole = strValHash.value(ATTR_ROLE);
        newSpeakerProfession = strValHash.value(COL_PROFESSION);
        newSpeakerSex = strValHash.value(COL_SEX);
        mSpeakers->insertRow(mSpeakers->rowCount());
        mSpeakers->tryToSubmitAll();
        speakerId = getLastRecordId(mSpeakers);
        newSpeakers.insert(strValHash.value(ATTR_ROLE), speakerId);
      }
      else {
        speakerId = newSpeakers.value(strValHash.value(ATTR_ROLE));
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
  int projectToBeDeleted = mProjects->cellData(row, SColID).toInt();
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

//  int digitsCount = AppSettings::intVal("", PRM_EXPORT_DIGIT_COUNT, 2);

  QDir dir(appPath());
  QString projectDir = DIR_PROJECTS_ROOT + projectTitle;
  if (!dir.mkpath(projectDir))
    return false;

//  QString multifileDelimiter
//      = QSmartDialog::inputStringDialog(SExportSetting,
//                                        SEnterFileSeparationDelimiter);

  bool result = true;

//  //Экспорт данных по проектам
//  exportSqlTableModel(mProjects, appPath() + DIR_PROJECTS_ROOT + "Projects.txt");
//  //Экспорт списка говорящих
//  exportSpeakerList(appPath() + projectDir);

//  QSqlRecord statementRec;
//  QSqlRecord* speakerRec;

//  bool result = true;
//  bool detailedLogging = AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_EXPORT_DETAILS, false);

//  int fileIndex = 1;
//  int lastSpeakerId = -1;
//  int currSpeakerId = -1;
//  QString resStatement = "";
//  QString exportText = "";
//  SParams speakerAttrs = SParams();
//  for(int i=0; i<mStatements->rowCount(); i++){
//    statementRec = mStatements->record(i);
//    currSpeakerId = statementRec.value(SColSpeakerId).toInt();
//    LOG << i << ":" << currSpeakerId;
//    QString statementText = statementRec.value(SColStatement).toString() + SSpace;
//    if (!multifileDelimiter.isEmpty() && statementText.trimmed() == multifileDelimiter){
//      exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(true), resStatement.trimmed());
//      resStatement.clear();
//      speakerAttrs.clear();
//      QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + PATH_DELIMITER + projectDir + PATH_DELIMITER + projectTitle + "_" + StrUtils::intToStr(fileIndex++, digitsCount) + ".xml");
//      exportText.clear();
//    }
//    else {
//      if (currSpeakerId != lastSpeakerId) {
//        if (!resStatement.isEmpty())
//          exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(true), resStatement.trimmed());
//        resStatement.clear();
//        speakerAttrs.clear();
//        if (currSpeakerId != 0){
//          speakerRec = mSpeakers->recordById(currSpeakerId);
//          speakerAttrs.setValue(COL_ACTOR, speakerRec->value(SColActor).toString());
//          speakerAttrs.setValue(COL_PROFESSION, speakerRec->value(SColProfession).toString());
//          speakerAttrs.setValue(COL_ROLE, speakerRec->value(SColSpeachRole).toString());
//          speakerAttrs.setValue(COL_SEX, speakerRec->value(SColSex).toString());
//        }
//      }
//      resStatement += processByReplacePatterns(statementText, PT_EXPORT, detailedLogging);
//    }
//    lastSpeakerId = currSpeakerId;
//  }
//  exportText += SExportStatementPattern.arg(speakerAttrs.toXmlAttrs(true), resStatement.trimmed());
//  if (multifileDelimiter.isEmpty()){
//    QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + PATH_DELIMITER + projectDir + PATH_DELIMITER + projectTitle + ".xml");
//  }
//  else {
//    result = QTextProcessor::stringToFile(SExportFilePattern.arg(exportText), appPath() + PATH_DELIMITER + projectDir + PATH_DELIMITER + projectTitle + "_" + StrUtils::intToStr(fileIndex++, digitsCount) + ".xml");
//  }

  return result;
}

bool QDataModule::exportSpeakerList(QString outDir)
{
  //TODO
//  QStringList exportList;
//  QSqlRecord rec;
//  QString speakerColName = speakerTitleCol == IDX_ACTOR ? SColActor : SColSpeachRole;
//  for(int i=0; i<mSpeakers->rowCount(); i++){
//    rec = mSpeakers->record(i);

//    QString line = rec.value(speakerColName).toString() + SDelimiter + rec.value(SColBirthYear).toString();
//    exportList.append(line);
//  }
//  return QTextProcessor::stringToFile(exportList.join(SNewLine), outDir.append("/Speakers.txt"));
  return true;
}

bool QDataModule::exportSqlTableModel(BaseRestTableModel* model, QString outPath)
{
  Q_UNUSED(model)
  Q_UNUSED(outPath)
  return true;
  //TODO
//  QStringList exportList;
//  QSqlRecord rec;
//  QString headerLine;
//  for(int i=0; i<model->columnCount(); i++){
//    headerLine.append(model->headerData(i, Qt::Horizontal).toString()).append(SDelimiter);
//  }
//  exportList.append(headerLine);
//  for(int i=0; i<model->rowCount(); i++){
//    rec = model->record(i);
//    QString line;
//    for(int j=0; j<rec.count(); j++){
//      line.append(rec.value(j).toString()).append(SDelimiter);
//    }
//    exportList.append(line);
//  }
//  return QTextProcessor::stringToFile(exportList.join(SNewLine), outPath);
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

bool QDataModule::deleteStatementHistory(int statementId)
{
  QJsonDocument jsonEmptyDoc;
  actDeleteStatementHistory->setParam("statement_id", statementId);
  actDeleteStatementHistory->sendAction("deleteHistory", jsonEmptyDoc);
  return true;
}

bool QDataModule::initDatabase()
{
  //TODO
//  QString dbPath = AppSettings::strVal("", SETTING_DATABASE, "Scripter.fdb");
//  db = QSqlDatabase::addDatabase("QIBASE");
//  db.setDatabaseName(dbPath);
//  db.setUserName("SYSDBA");
//  db.setPassword("masterkey");

//  bool ok = db.open();
//  if (ok) {
//    INFO << SDbConnectionSuccess;
//    LSqlTableModel::enableLogging(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_SQL, false));
//    QSqlQueryHelper::setLogging(AppSettings::boolVal(SECTION_LOGGER, PRM_LOG_SQL, false));
//  }
//  else
//    CRITICAL << SErrDatabase << db.lastError().databaseText();
  bool ok = true;
  return ok;
}

bool QDataModule::loadModels()
{
  bool result = true;

  _nam = new QNetworkAccessManager(this);
  mProjects = new BaseRestTableModel(_nam, this);
  mProjects->registerColumn(COL_ID, SColID);
  mProjects->registerColumn(COL_HEADER, SColHeader);
  mProjects->registerColumn(COL_PATH, SColPath);
  mProjects->registerColumn(COL_AUTHOR, SColAuthor);
  mProjects->registerColumn(COL_SEX, SColSex);
  mProjects->registerColumn(COL_BIRTH_YEAR, SColBirthYear);
  mProjects->registerColumn(COL_CREATE_YEAR, SColCreated);
  mProjects->registerColumn(COL_SPHERE, SColSphere);
  mProjects->registerColumn(COL_TEXT_TYPE, SColTextType);
  mProjects->registerColumn(COL_TOPIC, SColTopic);
  mProjects->registerColumn(COL_STYLE, SColStyle);
  mProjects->registerColumn(COL_AUDIENCE_AGE, SColAudienceAge);
  mProjects->registerColumn(COL_AUDIENCE_LEVEL, SColAudienceLevel);
  mProjects->registerColumn(COL_AUDIENCE_SIZE, SColAudienceSize);
  mProjects->registerColumn(COL_SPEAKER_TYPE, SColSpeakerType);
  mProjects->setServiceUrl(QUrl(URL_PROJECTS), false);
  mProjects->refresh();

  mPatterns = new BaseRestTableModel(_nam, this);
  mPatterns->registerColumn(SColID);
  mPatterns->registerColumn(SColName);
  mPatterns->registerColumn(SColType);
  mPatterns->registerColumn(SColPattern);
  mPatterns->registerColumn(SColColor);
  mPatterns->registerColumn(SColHexColor);

  mPatterns->setServiceUrl(QUrl(""), false);
//  mPatterns->refresh();

  mSchemes = new BaseRestTableModel(_nam, this);
  mSchemes->registerColumn(SColID);
  mSchemes->registerColumn(SColName);

  mSchemes->setServiceUrl(QUrl(""), false);
//  mSchemes->refresh();

  mSchemePatterns = new BaseRestTableModel(_nam, this);
  mSchemePatterns->registerColumn(SColID);
  mSchemePatterns->registerColumn(SColSchemeId);
  mSchemePatterns->registerColumn(SColPatternId);
  mSchemePatterns->registerColumn(SColActive);

  mSchemePatterns->registerCalcColumn(new LookupJsonColumn(COL_NAME, mPatterns, COL_PATTERN_ID, COL_NAME));
  mSchemePatterns->registerCalcColumn(new LookupJsonColumn(COL_PATTERN, mPatterns, COL_PATTERN_ID, COL_PATTERN));
  mSchemePatterns->registerCalcColumn(new LookupJsonColumn(COL_HEXCOLOR, mPatterns, COL_PATTERN_ID, COL_HEXCOLOR));

  mSchemePatterns->setServiceUrl(QUrl(""), false);
//  mSchemePatterns->refresh();

  mReplacePatterns = new BaseRestTableModel(_nam, this);
  mReplacePatterns->registerColumn(COL_ID);
  mReplacePatterns->registerColumn(COL_NAME);
  mReplacePatterns->registerColumn(COL_PATTERN);
  mReplacePatterns->registerColumn(COL_REGEXP);
  mReplacePatterns->registerColumn(COL_USAGE_TYPE);

  mReplacePatterns->setServiceUrl(QUrl(""), false);
//  mReplacePatterns->refresh();

  mSpeakers = new BaseRestTableModel(_nam, this);
  mSpeakers->registerColumn(COL_ID);
  mSpeakers->registerColumn(COL_PROJECT_ID);
  mSpeakers->registerColumn(COL_SPEECH_ROLE);
  mSpeakers->registerColumn(COL_PROFESSION);
  mSpeakers->registerColumn(COL_SEX);
  mSpeakers->registerColumn(COL_BIRTH_YEAR);
  mSpeakers->registerColumn(COL_ACTOR);

  mSpeakers->setParam("project_id", 0);
  mSpeakers->setServiceUrl(QUrl(URL_SPEAKERS), false);
  mSpeakers->refresh();

//  connect(mSpeakers, SIGNAL(beforeInsert(QSqlRecord&)),
//          this, SLOT(initNewSpeaker(QSqlRecord&)));
//  result = result && loadModel(mSpeakers, TABLE_SPEAKERS, GEN_SPEAKERS);

  mStatements = new BaseRestTableModel(_nam, this);
  mStatements->registerColumn(COL_ID);
  mStatements->registerColumn(COL_PROJECT_ID);
  mStatements->registerColumn(COL_SPEAKER_ID);
  mStatements->registerColumn(COL_STATEMENT);
  mStatements->registerColumn(COL_SORT_ID);
  mStatements->registerColumn(COL_DATE_TIME);
  mStatements->setServiceUrl(QUrl(URL_STATEMENTS), false);

  mStatements->setParam("project_id", 0);
//  mStatements->setLinkField(COL_PARENT_ID);
//  mStatements->refresh();

//  connect(mStatements, SIGNAL(beforeInsert(QSqlRecord&)),
//          this, SLOT(onBeforeStatementInsert(QSqlRecord&)));

  //Прокси-модель реплик для навигации
  mStatementsNavigation = new QSortFilterProxyModel(this);
  mStatementsNavigation->setSourceModel(mStatements);
  mStatementsNavigation->setFilterKeyColumn(mStatements->colIdxByName(COL_STATEMENT));
  mStatementsNavigation->setFilterCaseSensitivity(Qt::CaseInsensitive);

//  //Фильтрованная прокси-модель реплик
//  mStatementsSmartFiltered = new QStatementFilterModel(this);
//  mStatementsSmartFiltered->setSourceModel(mStatements);
//  mStatementsSmartFiltered->setModel(mSchemePatterns);
//  mStatementsSmartFiltered->setFilterKeyColumn(mStatements->fieldIndex(COL_STATEMENT));

  _mapperStatements = new QDataWidgetMapper(this);
  _mapperStatements->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

  mStatementHistory = new BaseRestTableModel(_nam, this);
  mStatementHistory->setServiceUrl(QUrl(TABLE_STATEMENTS_HISTORY), false);
//  mStatementHistory->refresh();

  mSpeakerTypes = new BaseRestTableModel(_nam, this);
  mSpeakerTypes->registerColumn(SColID);
  mSpeakerTypes->registerColumn(SColName);
  mSpeakerTypes->registerColumn(SColRoleTitle);
  mSpeakerTypes->registerColumn(SColActorTitle);
  mSpeakerTypes->registerColumn(SColProfessionTitle);

  mSpeakerTypes->setServiceUrl(QUrl(TABLE_SPEAKER_TYPES), false);
//  mSpeakerTypes->refresh();

  return result;
}

int QDataModule::getLastRecordId(BaseRestTableModel *model)
{
  int rowIndex = model->rowCount() - 1;
  if (rowIndex < 0){
    return -1;
  }

  return model->data(model->index(rowIndex, model->colIdxByName(SColID))).toInt();
}

int QDataModule::checkStatementLengthExceeded()
{
  int idx = -1;
  for (int row = 0; row < mStatements->rowCount(); row++) {
    int stSize = mStatements->cellData(row, COL_STATEMENT).toString().size();
      if (stSize > MAX_STATEMENT_SIZE) {
        WARNING << "Statement size exceeded";
        idx = row;
        break;
      }
  }
  return idx;
}

QString QDataModule::processByReplacePatterns(QString statement, int patternType, bool logging)
{
  if (logging)
    LOG << "Initial statement:" << statement;

  //TODO
//  QSqlRecord patternRec;
//  QString result = statement;
//  QRegularExpression rx;
//  for(int row = 0; row < mReplacePatterns->rowCount(); row++){
//    patternRec = mReplacePatterns->record(row);
//    if (patternRec.value(SColUsageType).toInt() == patternType){
//      rx.setPattern(patternRec.value(SColRegexp).toString());
//      QString pattern = patternRec.value(SColPattern).toString();
//      result = result.replace(rx, pattern);
//      if (logging)
//        LOG << "Processing step" << row + 1 << ":" << result;
//    }
//  }

  QString result = statement;
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

//void QDataModule::initNewSpeaker(QSqlRecord &record)
//{
//  record.setValue(SColProjectId, _dm->projectId);
//  if (!newSpeakerRole.isEmpty())
//    record.setValue(SColSpeachRole, newSpeakerRole);
//  if (!newSpeakerActor.isEmpty())
//    record.setValue(SColActor, newSpeakerActor);
//  if (!newSpeakerProfession.isEmpty())
//    record.setValue(SColProfession, newSpeakerProfession);
//  if (!newSpeakerSex.isEmpty())
//    record.setValue(SColSex, newSpeakerSex);
//  record.setValue(SColBirthYear, QVariant());

//  newSpeakerRole = "";
//  newSpeakerActor = "";
//  newSpeakerProfession = "";
//  newSpeakerSex = "";
//}

//void QDataModule::onBeforeStatementInsert(QSqlRecord &rec)
//{
//  rec.setValue(SColProjectId, projectId);
//  if (newStatementSpeaker >= 0)
//    rec.setValue(SColSpeakerId, newStatementSpeaker);
//  rec.setValue(SColStatement, newStatementText);

//  newStatementText = "";
//  newStatementSpeaker = 0;
//}


void moveWidgetToCenter(QWidget *widget)
{
  QList<QScreen*> screens = QGuiApplication::screens();
  QRect desktopRect = screens.first()->availableGeometry();
  widget->move(desktopRect.width()/2 - widget->width()/2,
             desktopRect.height()/2 - widget->height()/2);
}
