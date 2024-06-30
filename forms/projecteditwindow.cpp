#include "projecteditwindow.h"
#include "ui_projecteditwindow.h"
#include <QDebug>
#include "core/textprocessor.h"
#include "widgets/dictcombobox.h"
#include <QLabel>
#include <QVariantMap>

#define EDITOR_COMBOBOX "COMBOBOX"
#define EDITOR_CHECKLIST "CHECKLIST"
#define EDITOR_MODELCOMBO "MODELCOMBO"

ProjectEditWindow* ProjectEditWindow::singletonWindow = nullptr;

ProjectEditWindow::ProjectEditWindow(QWidget *parent) :
    BaseWindow(parent),
  ui(new Ui::ProjectEditWindow)
{
  ui->setupUi(this);
  setObjectName("QProjectEditWindow");

  dm = QDataModule::dm();

  projectMapper = new QDataWidgetMapper(this);
  projectMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
  projectMapper->setModel(dm->mProjects);
  createControls();
}

ProjectEditWindow::~ProjectEditWindow()
{
  delete ui;
}

ProjectEditWindow *ProjectEditWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
      singletonWindow = new ProjectEditWindow(parent);
  }
  return singletonWindow;
}


void ProjectEditWindow::createControls()
{
  SParams editorTypes;
  editorTypes.add(QTextProcessor::loadEditorTypes().toUpper(), SNewLine);

  auto model = dm->mProjects;

  for(int i=1; i < model->columnCount(); i++) {
    QString colName = model->columnNameByIdx(i).toUpper();

    QLabel* label = new QLabel(dm->mProjects->headerData(i, Qt::Horizontal).toString(), ui->scrollArea);
    ui->loControls->addWidget(label);

    QWidget* editor;
    QString editorType = editorTypes.value(colName).toString();
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
    editor->setObjectName(colName);

    ui->loControls->addWidget(editor);
    if (editorType == EDITOR_MODELCOMBO) {
        projectMapper->addMapping(editor, i, "data");
    }
    else {
        projectMapper->addMapping(editor, i);
    }
  }

  for(int i=1; i<model->columnCount(); i++){
    auto colName = model->columnNameByIdx(i).toUpper();
    QString editorType = editorTypes.value(colName).toString();
    if (editorType == EDITOR_CHECKLIST || editorType == EDITOR_COMBOBOX){
      QWidget* widget = projectMapper->mappedWidgetAt(i);
      updateComboItems(widget);
    }
  }
}

QString ProjectEditWindow::listFileFromScript(QWidget* editor)
{
  //TODO
//  QScriptEngine engine;
//  QVariantMap values;
//  for (int i=0; i<projectMapper->model()->columnCount(); i++){
//    QWidget* widget = projectMapper->mappedWidgetAt(i);
//    if (widget)
//      values.insert(widget->objectName(), widget->property("text"));
//  }
//  engine.globalObject().setProperty("colName", editor->objectName());
//  QScriptValue sValues = engine.newVariant(values);
//  engine.globalObject().setProperty("values", sValues);
//  QString script = QTextProcessor::editorScript();
//  QScriptValue resValue = engine.evaluate(script);
//  return resValue.toString();

  QString valuesFilename = editor->objectName();
  return valuesFilename + ".lst";
}

void ProjectEditWindow::on_btnSave_clicked()
{    
  projectMapper->submit();
  dm->mProjects->tryToSubmitAll();
}

void ProjectEditWindow::on_btnClose_clicked()
{
  close();
}

void ProjectEditWindow::onBeforePopup()
{
  updateComboItems(qobject_cast<QWidget*>(sender()));
}

void ProjectEditWindow::updateComboItems(QWidget* widget)
{
  QComboBox* comboEditor = qobject_cast<QComboBox*>(widget);

  QString filePath = QTextProcessor::configDir() + listFileFromScript(widget);
  QStringList values = QTextProcessor::fileToString(filePath).split(SNewLine);
  comboEditor->clear();
  comboEditor->addItems(values);

}
