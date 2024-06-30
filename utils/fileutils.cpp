#include "fileutils.h"
#include "slogger.h"

QFileUtils::QFileUtils()
{
}

QString QFileUtils::fileToString(QString filepath, bool isUtf8)
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

bool QFileUtils::stringToFile(QString str, QString filepath, bool isUtf)
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

QString QFileUtils::transliterate(QString &inStr)
{
    QString result = inStr;
    result = result.replace("А","A", Qt::CaseInsensitive);
    result = result.replace("Б","B", Qt::CaseInsensitive);
    result = result.replace("В","V", Qt::CaseInsensitive);
    result = result.replace("Г","G", Qt::CaseInsensitive);
    result = result.replace("Д","D", Qt::CaseInsensitive);
    result = result.replace("Е","E", Qt::CaseInsensitive);
    result = result.replace("Ж","ZH", Qt::CaseInsensitive);
    result = result.replace("З","Z", Qt::CaseInsensitive);
    result = result.replace("И","I", Qt::CaseInsensitive);
    result = result.replace("К","K", Qt::CaseInsensitive);
    result = result.replace("Л","L", Qt::CaseInsensitive);
    result = result.replace("М","M", Qt::CaseInsensitive);
    result = result.replace("Н","N", Qt::CaseInsensitive);
    result = result.replace("О","O", Qt::CaseInsensitive);
    result = result.replace("П","P", Qt::CaseInsensitive);
    result = result.replace("Р","R", Qt::CaseInsensitive);
    result = result.replace("С","S", Qt::CaseInsensitive);
    result = result.replace("Т","T", Qt::CaseInsensitive);
    result = result.replace("У","U", Qt::CaseInsensitive);
    result = result.replace("Ф","F", Qt::CaseInsensitive);
    result = result.replace("Х","H", Qt::CaseInsensitive);
    result = result.replace("Ц","C", Qt::CaseInsensitive);
    result = result.replace("Ш","SH", Qt::CaseInsensitive);
    result = result.replace("Щ","SH", Qt::CaseInsensitive);
    result = result.replace("Ь","", Qt::CaseInsensitive);
    result = result.replace("Ъ","", Qt::CaseInsensitive);
    result = result.replace("Э","E", Qt::CaseInsensitive);
    result = result.replace("Ю","YU", Qt::CaseInsensitive);
    result = result.replace("Я","YA", Qt::CaseInsensitive);
    result = result.replace(" ", "_");
    return result;
}

bool QFileUtils::forcePath(const QString &path)
{
  QDir dir;
  return dir.mkpath(path);
}

QString QFileUtils::extractDirPath(const QString &fullPath)
{
  QFileInfo info(fullPath);
  return info.absolutePath();
}

QString QFileUtils::extractFileName(const QString &fullPath)
{
  QFileInfo info(fullPath);
  return info.fileName();
}

QString QFileUtils::legalFilename(QString original)
{
  return "";
  //TODO
//  QRegExp rx("[, \"\\!\\?]{1,}");
//  return original.replace(rx, "_");
}
