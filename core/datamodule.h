#ifndef DATAMODULE_H
#define DATAMODULE_H

#include <QObject>
#include <QDataWidgetMapper>
#include <QTimer>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QNetworkAccessManager>
#include "models/baseresttablemodel.h"
#include "models/basejsonaction.h"
//#include "utils/sparams.h"

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

//TEMP
#define URL_PROJECTS "http://localhost/api/v1/projects"
#define URL_SPEAKERS "http://localhost/api/v1/speakers"
#define URL_STATEMENTS "http://localhost/api/v1/statements"

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

const QString SNewLine = "\r\n";
const QString SDelimiter = "\t";
const QString SSpace = " ";

const QString SColHeader = "Header";
const QString SColID = "ID";
const QString SColAuthor = "Author";
const QString SColSchemeId = "scheme_id";
const QString SColPatternId = "pattern_id";
const QString SColActive = "Active";
const QString SColCreated = "Create year";
const QString SColPath = "Path";
const QString SColStatement = "Statement";
const QString SColProjectId = "project_id";
const QString SColSphere = "Sphere";
const QString SColSex = "Sex";
const QString SColProfession = "Profession";
const QString SColSpeachRole = "Speech role";
const QString SColActor = "Actor";
const QString SColSpeakerId = "speaker_id";
const QString SColSortId = "sort_id";
const QString SColColor = "Color";
const QString SColHexColor = "Hex color";
const QString SColDT = "dt";
const QString SColBirthYear = "Birth year";
const QString SColName = "Name";
const QString SColType = "Type";
const QString SColStyle = "Style";
const QString SColAudienceAge = "Audience age";
const QString SColAudienceSize = "Audience size";
const QString SColAudienceLevel = "AudienceLevel";
const QString SColTextType = "Text type";
const QString SColTopic = "Topic";
const QString SColPattern = "Pattern";
const QString SColRegexp = "Regexp";
const QString SColUsageType = "Usage type";
const QString SColSpeakerType = "speakerTypeId";
const QString SColRoleTitle = "Role title";
const QString SColProfessionTitle = "Profession title";
const QString SColActorTitle = "Actor title";

#define IDX_ROLE 2
#define IDX_PROFESSION 3
#define IDX_SEX 4
#define IDX_BIRTH_YEAR 5
#define IDX_ACTOR 6

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

  QNetworkAccessManager* _nam;

  BaseRestTableModel* mProjects;
  BaseRestTableModel* mSpeakers;
  BaseRestTableModel* mStatements;
  QSortFilterProxyModel* mStatementsNavigation;
//  QStatementFilterModel* mStatementsSmartFiltered;
  BaseRestTableModel* mPatterns;
  BaseRestTableModel* mSchemes;
  BaseRestTableModel* mSchemePatterns;
  BaseRestTableModel* mReplacePatterns;
  BaseRestTableModel* mStatementHistory;
  BaseRestTableModel* mSpeakerTypes;

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
//  bool execSql(const QString &sql);
//  bool execSqlScript(QString script);

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
  bool exportSqlTableModel(BaseRestTableModel* model, QString outPath);

  bool editSetting(QString settingName, QString caption);

  bool deleteStatementHistory(int statementId);
  QDataWidgetMapper* _mapperStatements;  
protected:

private:
  QString _appPath;
  QTimer* timer;

  BaseJsonAction *actDeleteStatementHistory;

  bool initDatabase();
  bool loadModels();
  int getLastRecordId(BaseRestTableModel* model);
  int checkStatementLengthExceeded();
  //Применение шаблонов замены к строке
  QString processByReplacePatterns(QString statement, int patternType, bool logging);
signals:
  void projectLoaded(int oldProjecId, int newProjectId);
private slots:
  void on_autosave_timeout();
//  void initNewSpeaker(QSqlRecord &record);
public slots:
//  void onBeforeStatementInsert(QSqlRecord& rec);
};

//Перемещение виджета в центр экрана
void moveWidgetToCenter(QWidget* widget);

#endif // DATAMODULE_H
