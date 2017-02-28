#include "qregexphighlighter.h"
#include <QTextCharFormat>

QRegexpHighlighter::QRegexpHighlighter(QObject *parent) :
  QSyntaxHighlighter(parent)
{
}

void QRegexpHighlighter::setPattern(QString pattern)
{
  rx.setPattern(pattern);
  rehighlight();
}


void QRegexpHighlighter::highlightBlock(const QString &text)
{
  QTextCharFormat format;

  format.setBackground(QBrush(QColor(color)));
  //Если шаблон не корректный, форматирования нет
  if (rx.isEmpty() || !rx.isValid() || rx.exactMatch(""))
    return;

  int index = rx.indexIn(text);
  while (index >= 0){
    int length = rx.matchedLength();
    setFormat(index, length, format);
    index = rx.indexIn(text, index + length);
  }
}
