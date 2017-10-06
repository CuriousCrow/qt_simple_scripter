#ifndef QFILEUTILS_H
#define QFILEUTILS_H

#include <QFile>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>

class QFileUtils
{
public:
    QFileUtils();
    //Загрука файла в строку
    static QString fileToString(QString filepath, bool isUtf8 = true);
    //Сохранение строки в файл
    bool stringToFile(QString str, QString filepath, bool isUtf = true);
    //Транслитерация строки
    static QString transliterate(QString& inStr);
};

#endif // QFILEUTILS_H
