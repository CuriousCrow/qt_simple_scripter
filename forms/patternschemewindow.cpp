#include "patternschemewindow.h"
#include "ui_patternschemewindow.h"

#include <QSqlRelationalDelegate>
#include <QModelIndex>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlDriver>
#include <QSqlTableModel>

#include "widgets/qsmartdialog.h"
#include "core/appconst.h"

PatternSchemeWindow* PatternSchemeWindow::singletonWindow = nullptr;

PatternSchemeWindow::PatternSchemeWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::PatternSchemeWindow)
{
    ui->setupUi(this);
    setObjectName("QPatternSchemeWindow");
    
    dm = DataModule::dm();

    ui->lvPatternPool->setModel(dm->mPatterns);
    ui->lvPatternPool->setModelColumn(dm->mPatterns->fieldIndex(COL_NAME));

    ui->cmbSchemes->setModel(dm->mSchemes);
    ui->cmbSchemes->setModelColumn(dm->mSchemes->fieldIndex(COL_NAME));

    ui->lvItems->setModel(dm->mSchemePatterns);
    ui->lvItems->setModelColumn(4);
    connect(dm->mSchemePatterns, SIGNAL(beforeInsert(QSqlRecord&)),
            this, SLOT(onBeforePatternInsert(QSqlRecord&)));

    connect(dm->mSchemes, SIGNAL(beforeInsert(QSqlRecord&)),
            this, SLOT(onBeforeSchemeInsert(QSqlRecord&)));
}

PatternSchemeWindow::~PatternSchemeWindow()
{
    delete ui;
}

PatternSchemeWindow *PatternSchemeWindow::Instance(QWidget *parent)
{
    if (!singletonWindow){
        singletonWindow = new PatternSchemeWindow(parent);
    }
    return singletonWindow;
}

void PatternSchemeWindow::on_btnClose_clicked()
{
    close();
}

void PatternSchemeWindow::on_btnAddItem_clicked()
{
    //Проверка наличия выбранного шаблона в схеме
    int patternId = dm->mPatterns->data(ui->lvPatternPool->currentIndex().row(), COL_ID).toInt();
    if (dm->mSchemePatterns->rowByValue(COL_PATTERN_ID, patternId) >= 0){
        QSmartDialog::errorDialog(SErrPatternAlreadyAdded);
        return;
    }
    dm->mSchemePatterns->insertRow(0);
    dm->mSchemePatterns->submitAll();
}

void PatternSchemeWindow::onBeforePatternInsert(QSqlRecord &rec)
{
    int patternId = dm->mPatterns->data(ui->lvPatternPool->currentIndex().row(), COL_ID).toInt();
    rec.setValue(COL_PATTERN_ID, patternId);
    rec.setValue(COL_SCHEME_ID, 0);
    rec.setValue(COL_ACTIVE, true);
}

void PatternSchemeWindow::onBeforeSchemeInsert(QSqlRecord &rec)
{
    rec.setValue(COL_NAME, "Новая схема");
}

void PatternSchemeWindow::on_btnRemoveItem_clicked()
{
    dm->mSchemePatterns->removeRow(ui->lvItems->currentIndex().row());
}

void PatternSchemeWindow::on_btnAddScheme_clicked()
{
    dm->mSchemes->insertRow(dm->mSchemes->rowCount());
    dm->mSchemes->submitAll();
}
