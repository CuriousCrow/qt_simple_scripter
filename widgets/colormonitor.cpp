#include "colormonitor.h"
#include <QDebug>

ColorMonitor::ColorMonitor(QWidget *parent) : QFrame(parent)
{
    setStyleSheet("background-color: rgb(100, 255, 100);");
}

void ColorMonitor::update()
{
    MonitorState state = _state;
    QString description = _description;
    emit checkMethod(state, description);
    if (_state == state)
        return;
    qDebug() << "Check state:" << state << description;
    _state = state;
    switch (_state) {
    case Ok:
        setStyleSheet("background-color: rgb(100, 255, 100);");
        break;
    case Warning:
        setStyleSheet("background-color: rgb(255, 255, 50);");
        break;
    case Error:
        setStyleSheet("background-color: rgb(255, 100, 100);");
        break;
    default:
        break;
    }
    _description = description;
    setToolTip(_description);
}
