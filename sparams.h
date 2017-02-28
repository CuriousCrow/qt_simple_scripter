#ifndef SPARAMS_H
#define SPARAMS_H

#include <QString>
#include <QHash>
#include <QList>
#include <QStringList>
#include <QVariant>

class SParams
{
public:
    SParams();
    SParams(const SParams &copy);
    QVariant value(const QString &key) const;
    QString stringValue(const QString &key, const QString &defaultValue = "") const;
    QString toString(const QString &separator = ",", bool qouted = false) const; //сохранение параметров в строку
    QStringList toStringListSort();
    QByteArray calcMd5();
    QString toText() const;
    QString toHtml() const;
    QString toXml(const QString &root = "sparams", const QString &element = "param");
    QString toXmlAttrs();
    QString toJson();
    QString toCsv();
    void toHash(QHash <QString, QVariant> &dest, const QString &addPrefix = ""); //перекладывание параметров в QHash с указанием префикса
    QByteArray &toByteArray(QByteArray &dest) const;
    QVariant paramValueAt(int i) const { return _params.value(_params.keys().at(i)); }
    QString paramNameAt(int i) const { return _params.keys().at(i);}
    void setValue(const QString &key, const QVariant &value) { _params.insert(key, value); }
    void add(const QString &values, const QString &delimeter = ",", bool deleteQuotes = true); //добавление параметров из строки
    void add(const QStringList &keyValues, const QString &defaultParamName = "param", bool deleteQuotes = true); //перкладывает содержимое списка в QHash с разбиением пары key=value
    void add(const QHash<QString, QVariant> &keyValues, const QString &addPrefix = "");
    void add(const SParams &params, const QString &addPrefix = "");
    void add(const QByteArray &byteArray);
    void add(int argc, char *argv[]);
    void addByPrefix(const SParams &params, const QString &prefix, bool deletePrefix = true);
    void loadFromFile(const QString &fileName);
    void saveToFile(const QString &fileName);
    void loadFromXmlFile(const QString &fileName, const QString &root = "sparams", const QString &element = "param");
    void loadFromXml(const QString &xml, const QString &root = "sparams", const QString &element = "param");
    void remove(const QString &key);
    void remove(const QStringList &keys); //удаление параметров по списку QList
    void removeByPrefix(const QString &prefix); //удаление параметров по префексу
    bool containsValue(const QVariant &value) const;
    bool contains(const QString &key) const {return _params.contains(key);}
    QList<QString> keys() const {return _params.keys();}
    QList<QString> keysWithoutPrefix() const;
    void clear();
    int count() const;
    bool isEmpty() const {return _params.isEmpty();}
    void clearKeyPrefix(const QString &prefix); //зачистку префиксов у параметров
    QString replaceVariables(const QString &str);
    static SParams fromString(const QString &values);
    static SParams fromList(const QStringList &keyValues);
    static SParams fromHash(QHash<QString, QVariant> &keyValues);
    static SParams fromByteArray(const QByteArray &byteArray);
    static QString textToHtml(const QString &text);
    static QString stringToMd5(const QString &text);
    static QString Md5Uuid();
    static QString loadTextFile(const QString &fileName);
    friend QDataStream &operator << (QDataStream &out, const SParams &sparams);
    friend QDataStream &operator << (QDataStream &out, const SParams *sparams);
    friend QDataStream &operator >> (QDataStream &in, SParams &sparams);
    friend QDataStream &operator >> (QDataStream &in, SParams *sparams);

protected:
    QHash<QString, QVariant> _params;
};

QDebug operator<<(QDebug, const SParams &);
QDebug operator<<(QDebug, const SParams *);

#endif // SPARAMS_H
