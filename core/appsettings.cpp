#include "appsettings.h"
#include <QApplication>

#include "utils/slogger.h"

#define DELIMITER "/"

QSettings* AppSettings::_settings = nullptr;

QVariant AppSettings::val(QString key, QVariant defValue)
{
  if (!_settings)
    init();
  QVariant res = _settings->value(key, defValue);
  INFO << "Load:" << key << res;
  return res;
}

QVariant AppSettings::val(QString section, QString param, QVariant defValue)
{
  return val(section + DELIMITER + param, defValue);
}

void AppSettings::setVal(QString key, QVariant val)
{
  if (!_settings)
    init();
  _settings->setValue(key, val);
  INFO << "Save:" << key << val;
}

void AppSettings::setVal(QString section, QString param, QVariant val)
{
  setVal(section + DELIMITER + param, val);
}

QString AppSettings::strVal(QString section, QString name, QVariant defValue)
{
    QString fullName = name;
    if (!section.isEmpty())
        fullName = fullName.prepend(section + DELIMITER);
    return val(fullName, defValue).toString();
}

int AppSettings::intVal(QString section, QString name, QVariant defValue)
{
    QString fullName = name;
    if (!section.isEmpty())
        fullName = fullName.prepend(section + DELIMITER);
    return val(fullName, defValue).toInt();
}

bool AppSettings::boolVal(QString section, QString name, QVariant defValue)
{
    QString fullName = name;
    if (!section.isEmpty())
        fullName = fullName.prepend(section + DELIMITER);
    return val(fullName, defValue).toBool();
}

void AppSettings::remove(const QString &key)
{
  if (!_settings)
    init();
  _settings->remove(key);
}

QString AppSettings::applicationPath()
{
  return QApplication::applicationDirPath() + DELIMITER;
}

QString AppSettings::applicationName()
{
  return QApplication::applicationName();
}

void AppSettings::sync()
{
    if (!_settings)
      init();
    _settings->sync();
}

void AppSettings::init()
{
  QString settingsPath = applicationPath() + applicationName() + ".ini";
  qDebug() << "Loading settings: " + settingsPath;
  _settings = new QSettings(settingsPath, QSettings::IniFormat);
}

AppSettings::AppSettings(){}

bool AppSettings::contains(QString section, QString name)
{
    if (!_settings)
      init();
    QString fullName = name;
    if (!section.isEmpty())
        fullName.prepend(section + DELIMITER);
    return _settings->contains(fullName);
}
