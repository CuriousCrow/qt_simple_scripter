#include "qprojecteditwindow.h"
#include "ui_qprojecteditwindow.h"
#include <QDebug>
#include "qtextprocessor.h"
#include "widgets/dictcombobox.h"
#include <QVariantMap>

#define EDITOR_COMBOBOX "COMBOBOX"
#define EDITOR_CHECKLIST "CHECKLIST"
#define EDITOR_MODELCOMBO "MODELCOMBO"

QProjectEditWindow* QProjectEditWindow::singletonWindow = nullptr;

QProjectEditWindow::QProjectEditWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QProjectEditWindow)
{
  ui->setupUi(this);
  setObjectName("QProjectEditWindow");

  dm = QDataModule::dm();

  projectMapper = new QDataWidgetMapper(this);
  projectMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
  projectMapper->setModel(dm->mProjects);
  createControls();
}

QProjectEditWindow::~QProjectEditWindow()
{
  delete ui;
}

QProjectEditWindow *QProjectEditWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QProjectEditWindow(parent);    
  }
  return singletonWindow;
}


void QProjectEditWindow::createControls()
{
  SParams editorTypes;
  editorTypes.add(QTextProcessor::loadEditorTypes().toUpper(), SNewLine);

  //Использование шаблонной записи для генерации контролов
  QSqlRecord rec = dm->mProjects->patternRecord();

  for(int i=1; i<rec.count(); i++){
    QLabel* label = new QLabel(dm->mProjects->headerData(i, Qt::Horizontal).toString(), ui->scrollArea);
    ui->loControls->addWidget(label);

    QWidget* editor;
    QString editorType = editorTypes.value(rec.fieldName(i)).toString();
    if (editorType == EDITOR_COMBOBOX){
      QComboBox* combo = new QComboBox(ui->scrollArea);
      combo->setEditable(true);
      editor = combo;
    }
    else if (editorType == EDITOR_CHECKLIST){
      QCheckboxCombo* combo = new QCheckboxCombo(ui->scrollArea);
      combo->setDelimiter("|");
      combo->setEditable(true);
      connect(combo, SIGNAL(beforeOpen()), this, SLOT(onBeforePopup()));
      editor = combo;
    }
    else if (editorType == EDITOR_MODELCOMBO){
      QComboBox* modelCombo = new DictCombobox(ui->scrollArea);
      modelCombo->setModel(QDataModule::dm()->mSpeakerTypes);
      modelCombo->setModelColumn(1);
      modelCombo->setEditable(false);
      editor = modelCombo;
    }
    else {
      editor = new QLineEdit(ui->scrollArea);
    }
    editor->setObjectName(rec.fieldName(i));

    ui->loControls->addWidget(editor);
    if (editorType == EDITOR_MODELCOMBO) {
      projectMapper->addMapping(editor, i, "data");
    }
    else {
      projectMapper->addMapping(editor, i);
    }
  }

  for(int i=1; i<rec.count(); i++){
    QString editorType = editorTypes.value(rec.fieldName(i)).toString();
    if (editorType == EDITOR_CHECKLIST || editorType == EDITOR_COMBOBOX){
      QWidget* widget = projectMapper->mappedWidgetAt(i);
      updateComboItems(widget);
    }
  }
}

QString QProjectEditWindow::listFileFromScript(QWidget* editor)
{
    //TODO: Restore script logic

  // QScriptEngine engine;
  // QVariantMap values;
  // for (int i=0; i<projectMapper->model()->columnCount(); i++){
  //   QWidget* widget = projectMapper->mappedWidgetAt(i);
  //   if (widget)
  //     values.insert(widget->objectName(), widget->property("text"));
  // }
  // engine.globalObject().setProperty("colName", editor->objectName());
  // QScriptValue sValues = engine.newVariant(values);
  // engine.globalObject().setProperty("values", sValues);
  // QString script = QTextProcessor::editorScript();
  // QScriptValue resValue = engine.evaluate(script);
    QString resValue = editor->objectName() + ".lst";
    return resValue; //.toString();
}

void QProjectEditWindow::on_btnSave_clicked()
{    
  projectMapper->submit();
  dm->mProjects->submitAll();
}

void QProjectEditWindow::on_btnClose_clicked()
{
  close();
}

void QProjectEditWindow::onBeforePopup()
{
  updateComboItems(qobject_cast<QWidget*>(sender()));
}

void QProjectEditWindow::updateComboItems(QWidget* widget)
{
  QComboBox* comboEditor = qobject_cast<QComboBox*>(widget);

  QString filePath = QTextProcessor::configDir() + listFileFromScript(widget);
  QStringList values = QTextProcessor::fileToString(filePath).split(SNewLine);
  comboEditor->clear();
  comboEditor->addItems(values);

}
