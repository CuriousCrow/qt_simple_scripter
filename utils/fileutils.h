#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QTextCodec>

class FileUtils
{
public:
    FileUtils();
    //Загрука файла в строку
    static QString fileToString(QString filepath, bool isUtf8 = true);
    //Сохранение строки в файл
    static bool stringToFile(QString str, QString filepath, bool isUtf = true);
    //Транслитерация строки
    static QString transliterate(QString& inStr);
    //
    static bool forcePath(const QString &path);

    static QString extractDirPath(const QString &fullPath);

    static QString extractFileName(const QString &fullPath);

    static QString legalFilename(QString original);
};

#endif // FILEUTILS_H
