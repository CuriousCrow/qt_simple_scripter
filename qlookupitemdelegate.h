#ifndef QLOOKUPITEMDELEGATE_H
#define QLOOKUPITEMDELEGATE_H

#include <QItemDelegate>
#include <qusersqltablemodel.h>

class QLookupItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit QLookupItemDelegate(QObject *parent = 0);
    void setListModel(QUserSqlTableModel* model){ listModel = model; }
    //QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex & index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
private:
    QUserSqlTableModel* listModel;
signals:

public slots:

};

#endif // QLOOKUPITEMDELEGATE_H
