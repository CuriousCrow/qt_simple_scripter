#include "qtablehighlighter.h"
#include <QTextCharFormat>
#include "utils/slogger.h"
#include <QBrush>

QTableHighlighter::QTableHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
}

void QTableHighlighter::setEnabled(bool value)
{
    //Если состояние не меняется, то и делать ничего не надо
    if (_isEnabled == value)
        return;

    LOG << "switch highlighter to" << value;
    _isEnabled = value;
    rehighlight();
}

void QTableHighlighter::highlightBlock(const QString &text)
{
    //Если модель для подсветки не задана или подсвечивание выключено
    //сброс подсветки
    if (!_model || !_isEnabled)
        return;

    QTextCharFormat format;

    //Проход по строкам модели
    for (int i=0; i<_model->rowCount(); i++){
        QString pattern = _model->data(i, _patternColName).toString();
        QString color = _model->data(i, _hexcolorColName).toString();
        format.setBackground(QBrush(QColor(color)));
        rx.setPattern(pattern);
        //Если шаблон не корректный, переходим к следующему
        if (rx.isEmpty() || !rx.isValid() || rx.exactMatch(""))
            continue;

        int index = rx.indexIn(text);
        while (index >= 0){
            int length = rx.matchedLength();
            setFormat(index, length, format);
            index = rx.indexIn(text, index + length);
        }
    }
}
