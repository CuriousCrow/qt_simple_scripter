#include "qreplacepatternwindow.h"
#include "ui_qreplacepatternwindow.h"
#include <QDebug>

QReplacePatternWindow* QReplacePatternWindow::singletonWindow = nullptr;

QReplacePatternWindow::QReplacePatternWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QReplacePatternWindow)
{
  ui->setupUi(this);
  setObjectName("QReplacePatternWindow");

  dm = QDataModule::dm();
  ui->lvPatterns->setModel(dm->mReplacePatterns);
  ui->lvPatterns->setModelColumn(dm->mReplacePatterns->fieldIndex("NAME"));

  widgetMapper = new QDataWidgetMapper(this);
  widgetMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
  widgetMapper->setModel(dm->mReplacePatterns);
  widgetMapper->addMapping(ui->edtName, dm->mReplacePatterns->fieldIndex("NAME"));
  widgetMapper->addMapping(ui->edtRegexp, dm->mReplacePatterns->fieldIndex("REGEXP"));
  widgetMapper->addMapping(ui->edtPattern, dm->mReplacePatterns->fieldIndex("PATTERN"));
  widgetMapper->addMapping(ui->sbPriority, dm->mReplacePatterns->fieldIndex("PRIORITY"));
  widgetMapper->addMapping(ui->cmbUsageType, dm->mReplacePatterns->fieldIndex("USAGE_TYPE"), "currentIndex");

  connect(ui->lvPatterns->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          widgetMapper, SLOT(setCurrentModelIndex(QModelIndex)));
}

QReplacePatternWindow::~QReplacePatternWindow()
{
  delete ui;
}

QReplacePatternWindow *QReplacePatternWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QReplacePatternWindow(parent);    
  }
  return singletonWindow;
}

void QReplacePatternWindow::on_btnClose_clicked()
{
  dm->mReplacePatterns->revertAll();
  close();
}

void QReplacePatternWindow::on_btnSubmit_clicked()
{
  dm->mReplacePatterns->submitAll();
  dm->mReplacePatterns->select();
}

void QReplacePatternWindow::on_btnAddPattern_clicked()
{
  dm->mReplacePatterns->insertRow(ui->lvPatterns->currentIndex().row());
}

void QReplacePatternWindow::on_btnDeletePattern_clicked()
{
  qDebug() << "Deleting row " << ui->lvPatterns->currentIndex().row();
  dm->mReplacePatterns->removeRow(ui->lvPatterns->currentIndex().row());
}
