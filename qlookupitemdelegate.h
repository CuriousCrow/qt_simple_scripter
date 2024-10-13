#ifndef QLOOKUPITEMDELEGATE_H
#define QLOOKUPITEMDELEGATE_H

#include <QItemDelegate>

#include "models/lsqltablemodel.h"

class QLookupItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit QLookupItemDelegate(QObject *parent = 0);
    void setListModel(LSqlTableModel* model){ listModel = model; }
    //QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex & index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
private:
    LSqlTableModel* listModel;
signals:

public slots:

};

#endif // QLOOKUPITEMDELEGATE_H
