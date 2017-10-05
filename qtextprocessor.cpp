#include "qtextprocessor.h"
#include <QDebug>
#include <QApplication>
#include <QTextCodec>

#define DIR_CONFIG "/Config/"
#define DIR_PERL "/Perl/"
#define FILE_IN_ACCENT_BAT "temp.txt"
#define FILE_OUT_ACCENT_BAT "temp.acc.txt"
#define FILE_ACCENT_BAT "accent.bat"
#define FILE_EDITOR_LIST "editors.lst"
#define FILE_EDITOR_SCRIPT "editors.script"
#define FILE_SETTINGS "QtSimpleScripter.ini"

QSettings* QTextProcessor::_settings = 0;

QTextProcessor::QTextProcessor(QObject *parent) :
  QObject(parent)
{
}

QString QTextProcessor::processByAccentuator(QString str)
{
  QString perlPath = QApplication::applicationDirPath() + DIR_PERL;
  //Запись строки в файл
  stringToFile(str, perlPath + FILE_IN_ACCENT_BAT, false);

  //Запуск программы Акцентуатор
  QProcess accentuatorProcess;
  QStringList args;
  args << FILE_IN_ACCENT_BAT;
  accentuatorProcess.start(perlPath + FILE_ACCENT_BAT, args);
  accentuatorProcess.waitForFinished();
  accentuatorProcess.close();

  //Чтение строки из файла
  QString result = fileToString(perlPath + FILE_OUT_ACCENT_BAT, false);
  return result.isEmpty() ? str : result;
}


QString QTextProcessor::fileToString(QString filepath, bool isUtf8)
{
  QFile file(filepath);
  if (!file.exists()){
    qDebug() << "File" << file.fileName() << "doesn't exists";
    return "";
  }
  file.open(QIODevice::ReadOnly);
  QString result;
  if (isUtf8){
    result = QString::fromUtf8(file.readAll());
  }
  else {
    result = QString::fromLocal8Bit(file.readAll());
  }

  file.close();
  return result;
}

bool QTextProcessor::stringToFile(QString str, QString filepath, bool isUtf)
{
  QFile file(filepath);
  bool result = file.open(QIODevice::WriteOnly);
  if (result){
    if (isUtf){
      file.write(str.toUtf8());
    } else {
      file.write(str.toLocal8Bit());
    }
    file.close();
  }
  return result;
}

QStringList QTextProcessor::splitStringBySize(QString inStr, int size, QString delimiter, QString headerDelimiter)
{
  inStr = inStr.trimmed();
  QString header;
  if (inStr.contains(headerDelimiter)){
    header = inStr.section(headerDelimiter, 0, 0) + headerDelimiter;
    inStr.remove(header);
  }
  else {
    header = "";
  }
  QStringList tempSl = inStr.split(delimiter, QString::SkipEmptyParts);

  QStringList resultSl;
  QString resLine = "";
  foreach(QString rawChop, tempSl){
    if (header.length() + resLine.length() > size){
      resultSl.append(header + resLine);
      resLine.clear();
    }
    resLine += rawChop + delimiter;
  }
  resultSl.append(header + resLine);
  return resultSl;
}

QString QTextProcessor::removeCDATA(QString &str)
{
  QRegExp rx("<\\!\\[CDATA\\[(.*)\\]\\]>");
  rx.setMinimal(true);
  return str.replace(rx, "\\1");
}

QString QTextProcessor::loadEditorTypes()
{
  QString filePath = configDir() + FILE_EDITOR_LIST;
  return fileToString(filePath);
}

QString QTextProcessor::configDir()
{
  return QApplication::applicationDirPath() + DIR_CONFIG;
}

QString QTextProcessor::editorScript()
{
  return fileToString(configDir() + FILE_EDITOR_SCRIPT);
}

QStringList QTextProcessor::splitSqlScript(QString script)
{
  QStringList result;
  QString semicolonless;
  bool inQuotes = false;
  foreach (QChar chr, script) {
    if (chr == '\'') {
      inQuotes = !inQuotes;
    }
    else if (chr == ';') {
      if (inQuotes) {
        semicolonless += "[semicolon]";
        continue;
      }
    }
    semicolonless.append(chr);
  }
  qDebug() << semicolonless;
  foreach (QString item, semicolonless.split(";", QString::SkipEmptyParts)) {
    item = item.trimmed();
    if (item.isEmpty())
      continue;
    result.append(item.replace("[semicolon]", ";"));
  }
  return result;
}

QSettings *QTextProcessor::settings(QWidget *parent)
{
  if (!_settings){
    _settings = new QSettings("QtSimpleScripter.ini", QSettings::IniFormat, (QObject*)parent);
  }
  return _settings;
}
