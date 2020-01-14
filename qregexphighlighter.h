#ifndef QREGEXPHIGHLIGHTER_H
#define QREGEXPHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegExp>

class QRegexpHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  explicit QRegexpHighlighter(QObject *parent = nullptr);
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

#endif // QREGEXPHIGHLIGHTER_H
