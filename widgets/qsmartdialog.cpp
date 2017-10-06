#include "qsmartdialog.h"
#include "ui_qsmartdialog.h"
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

#define TITLE_ERROR "Ошибка"
#define TITLE_WARNING "Предупреждение"
#define TITLE_CONFIRMATION "Подтверждение"
#define TITLE_INFO "Внимание"

static QString SFontSizePattern = "<font size='%1'>%2</font>";

QSmartDialog::QSmartDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QSmartDialog)
{
  ui->setupUi(this);
}

QSmartDialog::~QSmartDialog()
{
  delete ui;
}

void QSmartDialog::addParam(QString caption, QVariant value)
{
  addLabel(caption);
  QLineEdit* editor = new QLineEdit(value.toString(), this);
  _widgetList.append(editor);
  editor->setObjectName("editor_" + QString::number(_widgetList.count()));
  ui->loInputWidgets->addWidget(editor);
}

void QSmartDialog::addLabel(QString text)
{
  QLabel* label = new QLabel(text, this);
  ui->loInputWidgets->addWidget(label);
}

QVariant QSmartDialog::valueAt(int index)
{
  if (index >= _widgetList.count()){
    return "";
  }
  QLineEdit* editor = (QLineEdit*)_widgetList.at(index);
  return editor->text();
}

QString QSmartDialog::applyFontSize(QString &text, int size)
{
  return SFontSizePattern.arg(QString::number(size), text);
}

bool QSmartDialog::confirmationDialog(QString question, QWidget* parent)
{  
  return QMessageBox::question(parent, TITLE_CONFIRMATION, applyFontSize(question)) == QMessageBox::Yes;
}

bool QSmartDialog::infoDialog(QString message, QWidget *parent)
{
  QMessageBox::information(parent, TITLE_INFO, applyFontSize(message));
  return true;
}

void QSmartDialog::errorDialog(QString message, QWidget *parent)
{
  QMessageBox::critical(parent, TITLE_ERROR, applyFontSize(message));
}


void QSmartDialog::warningDialog(QString message, QWidget* parent)
{
  QMessageBox::warning(parent, TITLE_WARNING, applyFontSize(message));
}

QString QSmartDialog::inputStringDialog(QString title, QString description, QWidget *parent, QString defaultValue)
{
  return QInputDialog::getText(parent, title, applyFontSize(description), QLineEdit::Normal, defaultValue);
}
