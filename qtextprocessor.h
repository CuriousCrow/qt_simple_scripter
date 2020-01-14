#ifndef QTEXTPROCESSOR_H
#define QTEXTPROCESSOR_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QFile>
#include "utils/sparams.h"

class QTextProcessor : public QObject
{
  Q_OBJECT
public:
  explicit QTextProcessor(QObject *parent = nullptr);
  static QString processByAccentuator(QString str);
  static QString fileToString(QString filepath, bool isUtf8 = true);
  static bool stringToFile(QString str, QString filepath, bool isUtf8 = true);
  static QStringList splitStringBySize(QString inStr, int size, QString headerDelimiter);
  static QStringList splitStringBySentences(QString inStr);
  static QString removeCDATA(QString& str);
  static QString loadEditorTypes();
  static QString configDir();
  static QString editorScript();
  static QStringList splitSqlScript(QString script);
private:

signals:

public slots:

};

#endif // QTEXTPROCESSOR_H
