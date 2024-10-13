#ifndef QDATAMODULE_H
#define QDATAMODULE_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include "models/lsqltablemodel.h"
#include <QDataWidgetMapper>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTimer>
#include <QSettings>
#include <QSortFilterProxyModel>
#include "models/qstatementmodel.h"
#include "models/qstatementfiltermodel.h"

#define DB_VERSION_TABLE "SYS_PARAMS"
#define DB_VERSION_FIELD "DATABASE_VERSION"

#define ERR_CODE_DB 1

#define S_WARNING "Предупреждение"
#define S_ERROR "Ошибка"

#define RESULT_SUCCESS "success"
#define RESULT_FAILED "failed"

#define SETTING_PROJECT_GROUP "projects/project"
#define SETTING_LAST_STATEMENT "lastStatement"
#define SETTING_TITLE "title"
#define SETTING_AUTOSAVE "AUTOSAVE_INTERVAL"
#define SETTING_AUTOSAVE_DEF_VAL 5
#define SETTING_DATABASE "DATABASE"
#define SETTING_HEADER_DELIMITER "HEADER_DELIMITER"
#define SETTING_HEADER_DELIMITER_DEF_VAL "@"

#define DIR_PROJECTS_ROOT "Projects/"

#define TABLE_PROJECTS "PROJECTS"
#define TABLE_SPEAKERS "SPEAKERS"
#define TABLE_STATEMENTS "STATEMENTS"
#define TABLE_SCHEMES "HIGHLIGHT_SCHEMES"
#define TABLE_SCHEME_PATTERNS "SCHEME_PATTERNS"
#define TABLE_PATTERNS "PATTERNS"
#define TABLE_REPLACE_PATTERNS "REPLACE_PATTERNS"
#define TABLE_STATEMENTS_HISTORY "STATEMENTS_HISTORY"
#define TABLE_SPEAKER_TYPES "SPEAKER_TYPES"

#define TAG_SPEECH "speech"
#define ATTR_ROLE "role"
#define ATTR_PROFESSION "profession"
#define ATTR_SEX "sex"
#define ATTR_ACTOR "actor"
const QString SMainWinTitle = "";
const QString SErrDatabase = "Ошибка БД: ";
const QString SErrWhileDbConnection = "Не удалось подключиться к базе данных";
const QString SErrWhileDbUpfate = "Не удалось обновить базу данных";
const QString SErrCanNotSplitIfSelected = "Нельзя разбить реплику при выделенном тексте";
const QString SConfirmDeleteProject = "Вы действительно хотите удалить проект \"%1\"?";
const QString SConfirmLoadProject = "Вы действительно хотите загрузить проект \"%1\"?";
const QString SConfirmSaveModifiedProject = "Данные реплик были изменены. Сохранить изменения?";
const QString SErrWhileProjectCreation = "Ошибка при создании нового проекта";
const QString SErrNoProjectLoaded = "No project loaded!";
const QString SErrSequenceFailed = "Error handling sequence: %1";
const QString SDbConnectionSuccess = "Database successfully connected";
const QString SMsgAutosaving = "Autosaving... %1";
const QString SLoadTableResultOK = "Loading table \"%1\"... Success";
const QString SLoadTableResultFailed = "Loading table \"%1\"... Failed";
const QString SErrPatternAlreadyAdded = "Шаблон уже присутствует в схеме.";
const QString SPrmAutosavePeriod = "Частота автосохранения в мин.";
const QString SChangesAppliesAfterRestart = "Изменения вступят в силу при следующем запуске программы";
const QString SEnterFileSeparationDelimiter = "Введите разделитель между файлами";
const QString SSettingEditing = "Редактирование настройки";
const QString SExportSetting = "Настройка экспорта";
const QString SConfirmDeleteStatement = "Вы действительно хотите удалить текущую реплику?";
const QString SConfirmAccentuatorRun = "В реплике обнаружены знаки ударения. Вы действительно хотите запустить акцентуатор?";

const QString SExportFilePattern = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<html><head></head><body>\r\n%1</body></html>";
const QString SExportStatementPattern = "<speech %1>%2</speech>\r\n";

const QString SActionAddStatement = "Добавить реплику";
const QString SActionInsertStatement = "Вставить реплику";
const QString SActionSplitStatement = "Разделить реплику";

#define SNewLine "\r\n"
#define SDelimiter "\t"
#define SSpace " "

#define SColHeader "HEADER"
#define SColID "ID"
#define SColAuthor "AUTHOR"
#define SColCreated "CREATED"
#define SColPath "PATH"
#define SColStatement "STATEMENT"
#define SColProjectId "PROJECT_ID"
#define SColSex "SEX"
#define SColProfession "PROFESSION"
#define SColSpeechRole "SPEACH_ROLE"
#define SColActor "ACTOR"
#define SColSpeakerId "SPEAKER_ID"
#define SColBirthYear "BIRTH_YEAR"
#define SColName "NAME"
#define SColPattern "PATTERN"
#define SColRegexp "REGEXP"
#define SColUsageType "USAGE_TYPE"
#define SColSpeakerType "SPEAKER_TYPE_ID"
#define SColRoleTitle "ROLE_TITLE"
#define SColProfessionTitle "PROFESSION_TITLE"
#define SColActorTitle "ACTOR_TITLE"

#define IDX_ROLE 2
#define IDX_ACTOR 6
#define IDX_PROFESSION 3
#define IDX_SEX 4
#define IDX_BIRTH_YEAR 5

enum AddOperation {
    Add = 0x1,
    Insert = 0x2,
    Split = 0x3
};

class QDataModule : public QObject
{
  Q_OBJECT
public:
  explicit QDataModule(QObject *parent = nullptr);
  ~QDataModule();

  static QDataModule* _dm;
  static QDataModule* dm(QObject* parent = nullptr);

  static const int PT_IMPORT = 1;
  static const int PT_EXPORT = 2;

  QSqlDatabase db;
  LSqlTableModel* mProjects;
  LSqlTableModel* mSpeakers;
  QStatementModel* mStatements;
  QSortFilterProxyModel* mStatementsNavigation;
  QStatementFilterModel* mStatementsSmartFiltered;
  LSqlTableModel* mPatterns;
  LSqlTableModel* mSchemes;
  LSqlTableModel* mSchemePatterns;
  LSqlTableModel* mReplacePatterns;
  LSqlTableModel* mStatementHistory;
  LSqlTableModel* mSpeakerTypes;

  int projectId = 0;

  int speakerTitleCol = 2;
  QString roleTitle = "Роль";
  QString actorTitle = "Актер";
  QString professionTitle = "Профессия";

  AddOperation addOper;
  int newStatementSpeaker = 0;
  QString newStatementText;

  QString newSpeakerRole;
  QString newSpeakerActor;
  QString newSpeakerSex;
  QString newSpeakerProfession;

  QString projectTitle;

  QString appPath();

  void loadProjectData(int id);
  bool saveProjectData(bool verbose = false);
  bool backupLocalProject();
  void checkForUnsavedProject(bool showDialog = false);

  void saveLastStatement();
  void loadLastStatement();

  int nextId(QString sequenceName);
  bool execSql(const QString &sql);
  bool execSqlScript(QString script);

  //Импорт проекта из файла
  bool importProject(QString importPath);

  bool importFromXml(QString importPath);
  //Удаление проекта
  bool deleteProject(int row);
  //Экспорт загруженного проекта
  bool exportProject();
  //Экспорт списка говорящих
  bool exportSpeakerList(QString outDir);
  //Экспорт списка проектов
  bool exportSqlTableModel(LSqlTableModel* model, QString outPath);

  bool editSetting(QString settingName, QString caption);
  QDataWidgetMapper* _mapperStatements;  
protected:

private:
  QString _appPath;
  QTimer* timer;  
  bool initDatabase();
  bool loadModels();
  bool startTransaction();
  bool commitTransaction();
  bool rollbackTransaction();
  bool loadModel(LSqlTableModel* model, QString table, QString sequence = "");
  int getLastRecordId(LSqlTableModel* model);
  int checkStatementLengthExceeded();
  void setTableHeaders(QSqlTableModel* table, QStringList headers);
  //Применение шаблонов замены к строке
  QString processByReplacePatterns(QString statement, int patternType, bool logging);
signals:
  void projectLoaded(int oldProjecId, int newProjectId);
private slots:
  void on_autosave_timeout();
  void initNewSpeaker(QSqlRecord &record);
public slots:
  void onBeforeStatementInsert(QSqlRecord& rec);
};

//Перемещение виджета в центр экрана
void moveWidgetToCenter(QWidget* widget);

#endif // QDATAMODULE_H
