#ifndef DICTCOMBOBOX_H
#define DICTCOMBOBOX_H

#include <QComboBox>
#include <QObject>

class DictCombobox : public QComboBox
{
  Q_OBJECT
  //Declare new widget property to set and retrieve current data values
  Q_PROPERTY(QVariant data READ currentData WRITE setData NOTIFY dataChanged USER true)
public:
  DictCombobox(QWidget *parent = nullptr);
signals:
  void dataChanged();
private:
  void setData(QVariant value);
};

#endif // DICTCOMBOBOX_H
