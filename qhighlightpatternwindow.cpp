#include "qhighlightpatternwindow.h"
#include "ui_qhighlightpatternwindow.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

QHighlightPatternWindow* QHighlightPatternWindow::singletonWindow = 0;

QHighlightPatternWindow::QHighlightPatternWindow(QWidget *parent) :
  QBaseWindow(parent),
  ui(new Ui::QHighlightPatternWindow)
{
  ui->setupUi(this);
  setObjectName("QHighlightPatternWindow");

  dm = QDataModule::dm();

  patternMapper = new QDataWidgetMapper(this);
  patternMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

  ui->patternListView->setModel(dm->mPatterns);
  ui->patternListView->setModelColumn(dm->mPatterns->fieldIndex("NAME"));

  patternMapper->setModel(dm->mPatterns);
  patternMapper->addMapping(ui->edName, dm->mPatterns->fieldIndex("NAME"));
  patternMapper->addMapping(ui->cmbType, dm->mPatterns->fieldIndex("TYPE"));
  patternMapper->addMapping(ui->edPattern, dm->mPatterns->fieldIndex("PATTERN"));
  patternMapper->addMapping(ui->cmbColor, dm->mPatterns->fieldIndex("HEXCOLOR"), "hexcolor");

  connect(ui->patternListView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          patternMapper, SLOT(setCurrentModelIndex(QModelIndex)));

  connect(dm->mPatterns, SIGNAL(beforeInsert(QSqlRecord&)),
          this, SLOT(initRecord(QSqlRecord&)));
}

QHighlightPatternWindow::~QHighlightPatternWindow()
{
  delete ui;
}

QHighlightPatternWindow *QHighlightPatternWindow::Instance(QWidget *parent)
{
  if (!singletonWindow){
    singletonWindow = new QHighlightPatternWindow(parent);    
  }
  return singletonWindow;
}

void QHighlightPatternWindow::initRecord(QSqlRecord &record)
{
  record.setValue("NAME", "Новый шаблон");
  record.setValue("TYPE", 0);
}

void QHighlightPatternWindow::on_btnApply_clicked()
{
  dm->mPatterns->submitAll();
}

void QHighlightPatternWindow::on_btnCancel_clicked()
{
  dm->mPatterns->revertAll();
  close();
}


void QHighlightPatternWindow::on_btnAdd_clicked()
{
  dm->mPatterns->insertRow(ui->patternListView->currentIndex().row());
}


void QHighlightPatternWindow::on_btnDelete_clicked()
{
  qDebug() << "Deleting row " << ui->patternListView->currentIndex().row();
  dm->mPatterns->removeRow(ui->patternListView->currentIndex().row());
}
