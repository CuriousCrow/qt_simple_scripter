#include "qstatementfiltermodel.h"

#include <QDebug>

QStatementFilterModel::QStatementFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

void QStatementFilterModel::setFiltered(bool value)
{
    _isFiltered = value;
    invalidateFilter();
}

QStatementFilterModel::~QStatementFilterModel()
{

}

bool QStatementFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    //Если модель для подсветки не задана или подсвечивание выключено
    //сброс подсветки
    if (!_model || !_isFiltered)
        return true;

    QModelIndex index = sourceModel()->index(source_row, filterKeyColumn());
    QString statementText = sourceModel()->data(index).toString();
    QRegExp rx = QRegExp();

    //Проход по строкам модели
    for (int i=0; i<_model->rowCount(); i++){
        QString pattern = _model->data(i, _patternColName).toString();
        rx.setPattern(pattern);
        //Если шаблон не корректный, переходим к следующему
        if (rx.isEmpty() || !rx.isValid() || rx.exactMatch(""))
            continue;
        //Если хоть одна подстрока найдена, реплика отображается
        if (rx.indexIn(statementText) >= 0)
            return true;
    }
    return false;
}
