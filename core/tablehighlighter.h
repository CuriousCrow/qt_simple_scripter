#ifndef TABLEHIGHLIGHTER_H
#define TABLEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include "models/baseresttablemodel.h"

class QTableHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  explicit QTableHighlighter(QObject *parent = nullptr);
  void setModel(BaseRestTableModel* tableModel){ _model = tableModel; }
  void setPatternColumn(QString name){ _patternColName = name; }
  void setHexcolorColumn(QString name){ _hexcolorColName = name; }
  void setEnabled(bool value);
protected:
  BaseRestTableModel* _model;
  void highlightBlock(const QString & text);
private:
  QRegularExpression rx;
  bool _isEnabled = false;
  QString _patternColName = "PATTERN";
  QString _hexcolorColName = "HEXCOLOR";
signals:

public slots:

};

#endif // TABLEHIGHLIGHTER_H
