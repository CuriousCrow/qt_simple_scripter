#ifndef QLOOKUPITEMDELEGATE_H
#define QLOOKUPITEMDELEGATE_H

#include <QItemDelegate>
#include "models/baseresttablemodel.h"

class QLookupItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit QLookupItemDelegate(QObject *parent = 0);
    void setListModel(BaseRestTableModel* model){ listModel = model; }
    //QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex & index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
private:
    BaseRestTableModel* listModel;
signals:

public slots:

};

#endif // QLOOKUPITEMDELEGATE_H
