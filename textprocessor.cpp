#include "textprocessor.h"

#include <QApplication>
#include <QTextCodec>

#include "utils/slogger.h"
#include "datamodule.h"

#define DIR_CONFIG "/Config/"
#define DIR_PERL "/Perl/"
#define FILE_IN_ACCENT_BAT "temp.txt"
#define FILE_OUT_ACCENT_BAT "temp.acc.txt"
#define FILE_ACCENT_BAT "accent.bat"
#define FILE_EDITOR_LIST "editors.lst"
#define FILE_EDITOR_SCRIPT "editors.script"
#define FILE_SETTINGS "QtSimpleScripter.ini"

QTextCodec* win1251 = QTextCodec::codecForName("Windows-1251");

TextProcessor::TextProcessor(QObject *parent) :
    QObject(parent)
{
}

QString TextProcessor::processByAccentuator(QString str)
{
    QString perlPath = QApplication::applicationDirPath() + DIR_PERL;
    //Запись строки в файл
    stringToFileWin1251(str, perlPath + FILE_IN_ACCENT_BAT);

    //Запуск программы Акцентуатор
    QProcess accentuatorProcess;
    QStringList args;
    args << FILE_IN_ACCENT_BAT;
    accentuatorProcess.start(perlPath + FILE_ACCENT_BAT, args);
    accentuatorProcess.waitForFinished();
    accentuatorProcess.close();

    //Чтение строки из файла
    QString result = fileWin1251ToString(perlPath + FILE_OUT_ACCENT_BAT);
    return result.isEmpty() ? str : result;
}


QString TextProcessor::fileToString(QString filepath, bool isUtf8)
{
    QFile file(filepath);
    if (!file.exists()){
        WARNING << "File" << file.fileName() << "doesn't exists";
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

QString TextProcessor::fileWin1251ToString(QString filepath)
{
    QFile file(filepath);
    if (!file.exists()){
        WARNING << "File" << file.fileName() << "doesn't exists";
        return "";
    }
    file.open(QIODevice::ReadOnly);
    QString result = win1251->toUnicode(file.readAll());

    file.close();
    return result;
}

bool TextProcessor::stringToFile(QString str, QString filepath, bool isUtf)
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

bool TextProcessor::stringToFileWin1251(QString str, QString filepath)
{
    QFile file(filepath);
    bool result = file.open(QIODevice::WriteOnly);
    if (result){
        result = file.write(win1251->fromUnicode(str));
        file.close();
    }
    return result;
}

QStringList TextProcessor::splitStringBySize(QString inStr, int size, QString headerDelimiter)
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
    QStringList sentences = splitStringBySentences(inStr);
    QString resLine;
    QStringList resultSl;
    foreach(QString sentence, sentences){
        if (header.length() + resLine.length() + 1 > size){
            resultSl.append(header + SSpace + resLine);
            resLine.clear();
        }
        resLine += sentence.trimmed() + SSpace;
    }
    resultSl.append(header + SSpace + resLine);
    return resultSl;
}

QStringList TextProcessor::splitStringBySentences(QString inStr)
{
    QRegExp rxBound("([\\.\\?\\!])\\s");
    inStr = rxBound.replaceIn(inStr, "\\1||");
    QStringList resSl = inStr.split("||", Qt::SkipEmptyParts);
    return resSl;
}

QString TextProcessor::removeCDATA(QString &str)
{
    QRegExp rx("<\\!\\[CDATA\\[(.*)\\]\\]>");
    rx.setMinimal(true);
    return rx.replaceIn(str, "\\1");
}

QString TextProcessor::loadEditorTypes()
{
    QString filePath = configDir() + FILE_EDITOR_LIST;
    return fileToString(filePath);
}

QString TextProcessor::configDir()
{
    return QApplication::applicationDirPath() + DIR_CONFIG;
}

QString TextProcessor::editorScript()
{
    return fileToString(configDir() + FILE_EDITOR_SCRIPT);
}

QStringList TextProcessor::splitSqlScript(QString script)
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
    foreach (QString item, semicolonless.split(";", Qt::SkipEmptyParts)) {
        item = item.trimmed();
        if (item.isEmpty())
            continue;
        result.append(item.replace("[semicolon]", ";"));
    }
    return result;
}
