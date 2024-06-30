#ifndef REGEXPTESTWINDOW_H
#define REGEXPTESTWINDOW_H

#include <QWidget>
#include "core/datamodule.h"
#include "core/regexphighlighter.h"

namespace Ui {
class RegexpTestWindow;
}

class RegexpTestWindow : public QWidget
{
  Q_OBJECT

public:
  explicit RegexpTestWindow(QWidget *parent = nullptr);
    ~RegexpTestWindow();

  static RegexpTestWindow* Instance(QWidget* parent = nullptr);
private slots:
  void on_edtRegexp_textChanged(const QString &arg1);

private:
  static RegexpTestWindow* singletonWindow;
  QRegexpHighlighter* highlighter;
  Ui::RegexpTestWindow *ui;
};

#endif // REGEXPTESTWINDOW_H
