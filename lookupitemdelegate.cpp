#include "lookupitemdelegate.h"

#include <QComboBox>
#include <QLineEdit>

LookupItemDelegate::LookupItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void LookupItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{    
    if (editor->inherits("QComboBox")){
        QComboBox* combobox = static_cast<QComboBox*>(editor);
        int itemIndex = combobox->findData(index.data());
        combobox->setCurrentIndex(itemIndex);
        return;
    }    
    QItemDelegate::setEditorData(editor, index);
}

void LookupItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (editor->inherits("QComboBox")){
        QComboBox* combobox = static_cast<QComboBox*>(editor);
        model->setData(index, combobox->currentData());
        return;
    }    
    QItemDelegate::setModelData(editor, model, index);
}
