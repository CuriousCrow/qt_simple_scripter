#ifndef STATEMENTFILTERMODEL_H
#define STATEMENTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QRegExp>

#include "models/lsqltablemodel.h"

class StatementFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit StatementFilterModel(QObject *parent = 0);
    void setModel(LSqlTableModel* tableModel){ _model = tableModel; }
    void setFiltered(bool value);
    void setPatternColumn(QString name){ _patternColName = name; }
    ~StatementFilterModel();

protected:
    LSqlTableModel* _model;
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
private:  
    bool _isFiltered = false;
    QString _patternColName = "PATTERN";
};

#endif // STATEMENTFILTERMODEL_H
