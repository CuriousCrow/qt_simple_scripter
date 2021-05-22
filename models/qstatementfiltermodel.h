#ifndef QSTATEMENTFILTERMODEL_H
#define QSTATEMENTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "../LSqlTableModel/lsqltablemodel.h"
#include <QRegExp>

class QStatementFilterModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  explicit QStatementFilterModel(QObject *parent = 0);
  void setModel(LSqlTableModel* tableModel){ _model = tableModel; }
  void setFiltered(bool value);
  void setPatternColumn(QString name){ _patternColName = name; }
  ~QStatementFilterModel();

protected:
  LSqlTableModel* _model;
  virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
private:  
  bool _isFiltered = false;
  QString _patternColName = "PATTERN";  
};

#endif // QSTATEMENTFILTERMODEL_H
