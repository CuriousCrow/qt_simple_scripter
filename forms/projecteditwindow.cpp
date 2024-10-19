#include "projecteditwindow.h"
#include "ui_projecteditwindow.h"

#include <QVariantMap>
#include <QJSEngine>
#include <QJSValue>

#include "core/textprocessor.h"
#include "widgets/dictcombobox.h"
#include "models/lsqltablemodel.h"
#include "widgets/qcheckboxcombo.h"
#include "utils/sparams.h"
#include "utils/slogger.h"

#define EDITOR_COMBOBOX "COMBOBOX"
#define EDITOR_CHECKLIST "CHECKLIST"
#define EDITOR_MODELCOMBO "MODELCOMBO"

ProjectEditWindow* ProjectEditWindow::singletonWindow = nullptr;

ProjectEditWindow::ProjectEditWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::ProjectEditWindow)
{
    ui->setupUi(this);
    setObjectName("ProjectEditWindow");
    
    dm = DataModule::dm();

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
    editorTypes.add(TextProcessor::loadEditorTypes().toUpper(), SNewLine);

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
            modelCombo->setModel(DataModule::dm()->mSpeakerTypes);
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

QString ProjectEditWindow::listFileFromScript(QWidget* editor)
{
    QString script = TextProcessor::editorScript();
    QJSEngine engine;
    // engine.installExtensions(QJSEngine::ConsoleExtension);


    QVariantMap values;
    for (int i=0; i<projectMapper->model()->columnCount(); i++){
        QWidget* widget = projectMapper->mappedWidgetAt(i);
        if (widget) {
            values.insert(widget->objectName(), widget->property("currentText"));
        }
    }
    engine.globalObject().setProperty("colName", editor->objectName());
    QJSValue sValues = engine.toScriptValue(values);
    engine.globalObject().setProperty("values", sValues);
    QJSValue resValue = engine.evaluate(script);
    INFO << "Editor" << editor->objectName() << "filename" << resValue.toString();
    return resValue.toString();
}

void ProjectEditWindow::on_btnSave_clicked()
{    
    projectMapper->submit();
    dm->mProjects->submitAll();
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

    QString filePath = TextProcessor::configDir() + listFileFromScript(widget);
    QStringList values = TextProcessor::fileToString(filePath).split(SNewLine);
    comboEditor->clear();
    comboEditor->addItems(values);

}
