#ifndef QREGEXPTESTWINDOW_H
#define QREGEXPTESTWINDOW_H

#include <QWidget>
#include "qdatamodule.h"
#include "qregexphighlighter.h"

namespace Ui {
class QRegexpTestWindow;
}

class QRegexpTestWindow : public QWidget
{
  Q_OBJECT

public:
  explicit QRegexpTestWindow(QWidget *parent = 0);
  ~QRegexpTestWindow();

  static QRegexpTestWindow* Instance(QWidget* parent = 0);
private slots:
  void on_edtRegexp_textChanged(const QString &arg1);

private:
  static QRegexpTestWindow* singletonWindow;
  QRegexpHighlighter* highlighter;
  Ui::QRegexpTestWindow *ui;
};

#endif // QREGEXPTESTWINDOW_H
