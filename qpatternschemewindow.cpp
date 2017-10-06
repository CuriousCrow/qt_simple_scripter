#include "qpatternschemewindow.h"
#include "ui_qpatternschemewindow.h"
#include <QSqlRelationalDelegate>
#include <QModelIndex>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QSqlDriver>
#include <QSqlTableModel>
#include "widgets/qsmartdialog.h"

QPatternSchemeWindow* QPatternSchemeWindow::singletonWindow = 0;

QPatternSchemeWindow::QPatternSchemeWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QPatternSchemeWindow)
{
  ui->setupUi(this);
  setObjectName("QPatternSchemeWindow");

  dm = QDataModule::dm();

  ui->lvPatternPool->setModel(dm->mPatterns);
  ui->lvPatternPool->setModelColumn(dm->mPatterns->fieldIndex("NAME"));

  ui->cmbSchemes->setModel(dm->mSchemes);
  ui->cmbSchemes->setModelColumn(dm->mSchemes->fieldIndex("NAME"));

  ui->lvItems->setModel(dm->mSchemePatterns);
  ui->lvItems->setModelColumn(4);
  connect(dm->mSchemePatterns, SIGNAL(beforeInsert(QSqlRecord&)),
          this, SLOT(onBeforePatternInsert(QSqlRecord&)));
}

QPatternSchemeWindow::~QPatternSchemeWindow()
{
  delete ui;
}

QPatternSchemeWindow *QPatternSchemeWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QPatternSchemeWindow(parent);    
  }
  return singletonWindow;
}

void QPatternSchemeWindow::on_btnClose_clicked()
{
  close();
}

void QPatternSchemeWindow::on_btnAddItem_clicked()
{
  //Проверка наличия выбранного шаблона в схеме
  int patternId = dm->mPatterns->data(ui->lvPatternPool->currentIndex().row(), "ID").toInt();
  if (dm->mSchemePatterns->rowByValue("PATTERN_ID", patternId) >= 0){
    QSmartDialog::errorDialog(SErrPatternAlreadyAdded);
    return;
  }
  dm->mSchemePatterns->insertRow(0);
  dm->mSchemePatterns->submitAll();
}

void QPatternSchemeWindow::onBeforePatternInsert(QSqlRecord &rec)
{
  int patternId = dm->mPatterns->data(ui->lvPatternPool->currentIndex().row(), "ID").toInt();
  rec.setValue("PATTERN_ID", patternId);
  rec.setValue("SCHEME_ID", 0);
  rec.setValue("ACTIVE", true);
}

void QPatternSchemeWindow::on_btnRemoveItem_clicked()
{
  dm->mSchemePatterns->removeRow(ui->lvItems->currentIndex().row());
}

void QPatternSchemeWindow::on_btnAddScheme_clicked()
{

}
