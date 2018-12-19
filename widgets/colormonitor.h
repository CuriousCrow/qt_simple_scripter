#ifndef COLORMONITOR_H
#define COLORMONITOR_H

#include <QWidget>
#include <QFrame>

enum MonitorState {
    Ok = 0,
    Warning = 1,
    Error = 2
};

class ColorMonitor : public QFrame
{
    Q_OBJECT
public:
    explicit ColorMonitor(QWidget *parent = nullptr);
    MonitorState state();
signals:
    void checkMethod(MonitorState &state, QString &description);
    void stateChanged();
public slots:
    void update();
private:
    QString _description;
    MonitorState _state = MonitorState::Ok;
};

#endif // COLORMONITOR_H
