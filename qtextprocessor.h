#ifndef QTEXTPROCESSOR_H
#define QTEXTPROCESSOR_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QFile>
#include <QSettings>
#include "sparams.h"

class QTextProcessor : public QObject
{
  Q_OBJECT
public:
  explicit QTextProcessor(QObject *parent = 0);
  static QString processByAccentuator(QString str);
  static QString fileToString(QString filepath, bool isUtf8 = true);
  static bool stringToFile(QString str, QString filepath, bool isUtf8 = true);
  static QStringList splitStringBySize(QString inStr, int size, QString delimiter, QString headerDelimiter);
  static QString removeCDATA(QString& str);
  static QString loadEditorTypes();
  static QString configDir();
  static QString editorScript();
  static QSettings* settings(QWidget* parent = 0);
private:
  static QSettings* _settings;
signals:

public slots:

};

#endif // QTEXTPROCESSOR_H
