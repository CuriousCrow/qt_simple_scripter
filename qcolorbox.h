#ifndef QCOLORBOX_H
#define QCOLORBOX_H

#include <QComboBox>

class QColor;
class QWidget;

class QColorBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(uint color READ color WRITE setColor USER true)
    Q_PROPERTY(QString hexcolor READ hexcolor WRITE setHexColor USER true)
public:
    QColorBox(QWidget *widget = 0);

public:
    uint color() const;
    void setColor(uint c);

    QString hexcolor() const;
    void setHexColor(QString hexcolor);
private:
    void populateList();
};

#endif // QCOLORBOX_H
