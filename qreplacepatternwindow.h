#ifndef QREPLACEPATTERNWINDOW_H
#define QREPLACEPATTERNWINDOW_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include "qbasewindow.h"
#include "qdatamodule.h"

namespace Ui {
class QReplacePatternWindow;
}

class QReplacePatternWindow : public QBaseWindow
{
  Q_OBJECT

public:
  explicit QReplacePatternWindow(QWidget *parent = 0);
  ~QReplacePatternWindow();

  static QReplacePatternWindow* Instance(QWidget* parent);
private slots:
  void on_btnClose_clicked();

  void on_btnSubmit_clicked();

  void on_btnAddPattern_clicked();

  void on_btnDeletePattern_clicked();

private:
  static QReplacePatternWindow* singletonWindow;
  QDataModule* dm;
  QDataWidgetMapper* widgetMapper;
  Ui::QReplacePatternWindow *ui;
};

#endif // QREPLACEPATTERNWINDOW_H
