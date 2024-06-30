#include "replacepatternwindow.h"
#include "ui_replacepatternwindow.h"
#include "utils/slogger.h"

ReplacePatternWindow* ReplacePatternWindow::singletonWindow = nullptr;

ReplacePatternWindow::ReplacePatternWindow(QWidget *parent) :
    BaseWindow(parent),
  ui(new Ui::ReplacePatternWindow)
{
  ui->setupUi(this);
  setObjectName("QReplacePatternWindow");

  dm = QDataModule::dm();
  ui->lvPatterns->setModel(dm->mReplacePatterns);
  ui->lvPatterns->setModelColumn(dm->mReplacePatterns->colIdxByName("NAME"));

  widgetMapper = new QDataWidgetMapper(this);
  widgetMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
  widgetMapper->setModel(dm->mReplacePatterns);
  widgetMapper->addMapping(ui->edtName, dm->mReplacePatterns->colIdxByName("NAME"));
  widgetMapper->addMapping(ui->edtRegexp, dm->mReplacePatterns->colIdxByName("REGEXP"));
  widgetMapper->addMapping(ui->edtPattern, dm->mReplacePatterns->colIdxByName("PATTERN"));
  widgetMapper->addMapping(ui->sbPriority, dm->mReplacePatterns->colIdxByName("PRIORITY"));
  widgetMapper->addMapping(ui->cmbUsageType, dm->mReplacePatterns->colIdxByName("USAGE_TYPE"), "currentIndex");

  connect(ui->lvPatterns->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          widgetMapper, SLOT(setCurrentModelIndex(QModelIndex)));
}

ReplacePatternWindow::~ReplacePatternWindow()
{
  delete ui;
}

ReplacePatternWindow *ReplacePatternWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
      singletonWindow = new ReplacePatternWindow(parent);    
  }
  return singletonWindow;
}

void ReplacePatternWindow::on_btnClose_clicked()
{
  dm->mReplacePatterns->revertAll();
  close();
}

void ReplacePatternWindow::on_btnSubmit_clicked()
{
  dm->mReplacePatterns->tryToSubmitAll();
  dm->mReplacePatterns->refresh();
}

void ReplacePatternWindow::on_btnAddPattern_clicked()
{
  dm->mReplacePatterns->insertRow(ui->lvPatterns->currentIndex().row());
}

void ReplacePatternWindow::on_btnDeletePattern_clicked()
{
  LOG << "Deleting row " << ui->lvPatterns->currentIndex().row();
  dm->mReplacePatterns->removeRow(ui->lvPatterns->currentIndex().row());
}
