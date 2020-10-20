#include "dictcombobox.h"



DictCombobox::DictCombobox(QWidget *parent) : QComboBox(parent)
{
  connect(this, SIGNAL(currentIndexChanged(int)),
          SIGNAL(dataChanged()));
}

void DictCombobox::setData(QVariant value)
{
  //Search for row that corresponds currentData=value
  int row = findData(value);
  if (row < 0 || row == currentIndex())
    return;
  setCurrentIndex(row);
}
