#include "qcolorbox.h"

QColorBox::QColorBox(QWidget *widget) : QComboBox(widget)
{
    populateList();
}

uint QColorBox::color() const
{
    return itemData(currentIndex(), Qt::UserRole).toUInt();
}

void QColorBox::setColor(uint color)
{
    setCurrentIndex(findData(color, int(Qt::UserRole)));
}

QString QColorBox::hexcolor() const
{
    return itemData(currentIndex(), Qt::DecorationRole).value<QColor>().name();
}

void QColorBox::setHexColor(QString hexcolor)
{
    setCurrentIndex(findData(QColor(hexcolor), Qt::DecorationRole));
}

void QColorBox::populateList()
{
    QStringList colorNames = QColor::colorNames();

    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);        
        insertItem(i, colorNames[i].toUpper());
        setItemData(i, color.rgb(), Qt::UserRole);
        setItemData(i, color, Qt::DecorationRole);
    }
}
