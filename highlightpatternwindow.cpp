#include "highlightpatternwindow.h"
#include "ui_highlightpatternwindow.h"

#include <QSqlRecord>
#include <QSqlField>

#include "utils/slogger.h"
#include "utils/appconst.h"

HighlightPatternWindow* HighlightPatternWindow::singletonWindow = nullptr;

HighlightPatternWindow::HighlightPatternWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::HighlightPatternWindow)
{
    ui->setupUi(this);
    setObjectName("QHighlightPatternWindow");
    
    dm = DataModule::dm();

    patternMapper = new QDataWidgetMapper(this);
    patternMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    ui->patternListView->setModel(dm->mPatterns);
    ui->patternListView->setModelColumn(dm->mPatterns->fieldIndex(COL_NAME));

    patternMapper->setModel(dm->mPatterns);
    patternMapper->addMapping(ui->edName, dm->mPatterns->fieldIndex(COL_NAME));
    patternMapper->addMapping(ui->cmbType, dm->mPatterns->fieldIndex(COL_TYPE));
    patternMapper->addMapping(ui->edPattern, dm->mPatterns->fieldIndex(COL_PATTERN));
    patternMapper->addMapping(ui->cmbColor, dm->mPatterns->fieldIndex(COL_HEXCOLOR), "hexcolor");

    connect(ui->patternListView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            patternMapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(dm->mPatterns, SIGNAL(beforeInsert(QSqlRecord&)),
            this, SLOT(initRecord(QSqlRecord&)));
}

HighlightPatternWindow::~HighlightPatternWindow()
{
    delete ui;
}

HighlightPatternWindow *HighlightPatternWindow::Instance(QWidget *parent)
{
    if (!singletonWindow){
        singletonWindow = new HighlightPatternWindow(parent);
    }
    return singletonWindow;
}

void HighlightPatternWindow::initRecord(QSqlRecord &record)
{
    record.setValue(COL_NAME, "Новый шаблон");
    record.setValue(COL_TYPE, 0);
}

void HighlightPatternWindow::on_btnApply_clicked()
{
    dm->mPatterns->submitAll();
}

void HighlightPatternWindow::on_btnCancel_clicked()
{
    dm->mPatterns->revertAll();
    close();
}


void HighlightPatternWindow::on_btnAdd_clicked()
{
    dm->mPatterns->insertRow(ui->patternListView->currentIndex().row());
}


void HighlightPatternWindow::on_btnDelete_clicked()
{
    LOG << "Deleting row " << ui->patternListView->currentIndex().row();
    dm->mPatterns->removeRow(ui->patternListView->currentIndex().row());
}
