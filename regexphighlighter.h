#ifndef REGEXPHIGHLIGHTER_H
#define REGEXPHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegExp>

class RegexpHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit RegexpHighlighter(QObject *parent = nullptr);
    void setPattern(QString pattern);
    void setColor(QColor val){ color = val; }
private:
    QRegExp rx;
    QColor color = Qt::yellow;
signals:

public slots:

protected:
    void highlightBlock(const QString &text);
};

#endif // REGEXPHIGHLIGHTER_H
