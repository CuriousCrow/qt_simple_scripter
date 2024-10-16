#ifndef LOOKUPITEMDELEGATE_H
#define LOOKUPITEMDELEGATE_H

#include <QItemDelegate>

#include "models/lsqltablemodel.h"

class LookupItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LookupItemDelegate(QObject *parent = 0);
    void setListModel(LSqlTableModel* model){ listModel = model; }
    //QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex & index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
private:
    LSqlTableModel* listModel;
signals:

public slots:

};

#endif // LOOKUPITEMDELEGATE_H
