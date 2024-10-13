#ifndef QTABLEHIGHLIGHTER_H
#define QTABLEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegExp>

#include "models/lsqltablemodel.h"

class QTableHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit QTableHighlighter(QObject *parent = nullptr);
    void setModel(LSqlTableModel* tableModel){ _model = tableModel; }
    void setPatternColumn(QString name){ _patternColName = name; }
    void setHexcolorColumn(QString name){ _hexcolorColName = name; }
    void setEnabled(bool value);
protected:
    LSqlTableModel* _model;
    void highlightBlock(const QString & text);
private:
    QRegExp rx;
    bool _isEnabled = false;
    QString _patternColName = "PATTERN";
    QString _hexcolorColName = "HEXCOLOR";
signals:

public slots:

};

#endif // QTABLEHIGHLIGHTER_H
