#ifndef QSMARTDIALOG_H
#define QSMARTDIALOG_H

#include <QDialog>
#include <QList>
#include "utils/sparams.h"

#define DEF_FONT_SIZE 5

namespace Ui {
class QSmartDialog;
}

class QSmartDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QSmartDialog(QWidget *parent = 0);
  ~QSmartDialog();

  void addParam(QString caption, QVariant value);
  void addLabel(QString text);
  QVariant valueAt(int index);
  static QString applyFontSize(QString &text, int size = DEF_FONT_SIZE);

  static bool confirmationDialog(QString question, QWidget* parent = 0);
  static bool infoDialog(QString message, QWidget* parent = 0);
  static void errorDialog(QString message, QWidget* parent = 0);
  static void warningDialog(QString message, QWidget* parent = 0);
  static QString inputStringDialog(QString title, QString description, QWidget* parent = 0, QString defaultValue = "");
private:
  Ui::QSmartDialog *ui;
  QList<QWidget*> _widgetList;
};

#endif // QSMARTDIALOG_H
