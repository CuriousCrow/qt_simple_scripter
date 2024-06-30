#include "patternschemewindow.h"
#include "ui_patternschemewindow.h"
#include <QModelIndex>
#include <QDebug>
#include "widgets/qsmartdialog.h"

PatternSchemeWindow* PatternSchemeWindow::singletonWindow = nullptr;

PatternSchemeWindow::PatternSchemeWindow(QWidget *parent) :
    BaseWindow(parent),
  ui(new Ui::PatternSchemeWindow)
{
  ui->setupUi(this);
  setObjectName("QPatternSchemeWindow");

  dm = QDataModule::dm();

  ui->lvPatternPool->setModel(dm->mPatterns);
  ui->lvPatternPool->setModelColumn(dm->mPatterns->colIdxByName("NAME"));

  ui->cmbSchemes->setModel(dm->mSchemes);
  ui->cmbSchemes->setModelColumn(dm->mSchemes->colIdxByName("NAME"));

  ui->lvItems->setModel(dm->mSchemePatterns);
  ui->lvItems->setModelColumn(4);
//  connect(dm->mSchemePatterns, SIGNAL(beforeInsert(QSqlRecord&)),
//          this, SLOT(onBeforePatternInsert(QSqlRecord&)));
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
//  int patternId = dm->mPatterns->cellData(ui->lvPatternPool->currentIndex().row(), "ID").toInt();
//  if (dm->mSchemePatterns->rowByValue("PATTERN_ID", patternId) >= 0){
//    QSmartDialog::errorDialog(SErrPatternAlreadyAdded);
//    return;
//  }
  dm->mSchemePatterns->insertRow(0);
  dm->mSchemePatterns->tryToSubmitAll();
}

//void QPatternSchemeWindow::onBeforePatternInsert(QSqlRecord &rec)
//{
//  int patternId = dm->mPatterns->data(ui->lvPatternPool->currentIndex().row(), "ID").toInt();
//  rec.setValue("PATTERN_ID", patternId);
//  rec.setValue("SCHEME_ID", 0);
//  rec.setValue("ACTIVE", true);
//}

void PatternSchemeWindow::on_btnRemoveItem_clicked()
{
  dm->mSchemePatterns->removeRow(ui->lvItems->currentIndex().row());
}

void PatternSchemeWindow::on_btnAddScheme_clicked()
{

}
