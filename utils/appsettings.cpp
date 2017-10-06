#include "appsettings.h"
#include <QApplication>
#include <QDebug>

QSettings* AppSettings::_settings = 0;

QVariant AppSettings::val(QString name, QVariant defValue)
{
    if (!_settings) {
        QString settingsPath = QApplication::applicationDirPath() + "/localmediacorpus.ini";
        qDebug() << "Loading settings: " + settingsPath;
        _settings = new QSettings(settingsPath, QSettings::IniFormat);
    }
    QVariant res = _settings->value(name, defValue);
    qDebug() << name << res;
    return res;
}

QString AppSettings::strVal(QString section, QString name, QVariant defValue)
{
    QString fullName = name;
    if (!section.isEmpty())
        fullName = fullName.prepend(section + "/");
    return val(fullName, defValue).toString();
}

int AppSettings::intVal(QString section, QString name, QVariant defValue)
{
    QString fullName = name;
    if (!section.isEmpty())
        fullName = fullName.prepend(section + "/");
    return val(fullName, defValue).toInt();
}

bool AppSettings::boolVal(QString section, QString name, QVariant defValue)
{
    QString fullName = name;
    if (!section.isEmpty())
        fullName = fullName.prepend(section + "/");
    return val(fullName, defValue).toBool();
}

QString AppSettings::applicationPath()
{
    return QApplication::applicationDirPath() + "/";
}

void AppSettings::sync()
{
    val("");
    _settings->sync();
}

AppSettings::AppSettings(){}

bool AppSettings::contains(QString section, QString name)
{
    if (!_settings)
        val("");
    QString fullName = name;
    if (!section.isEmpty())
        fullName.prepend(section + "/");
    return _settings->contains(fullName);
}
