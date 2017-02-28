#include "sparams.h"
#include <QDebug>
#include <QFile>
#include <QCryptographicHash>
#include <QUuid>
#include <QXmlStreamReader>
#include <QTextStream>

SParams::SParams()
{
}

SParams::SParams(const SParams &copy)
{
    _params = copy._params;
    //    _params.insert("itsCopy","true");
}

QVariant SParams::value(const QString &key) const
{
    QVariant result;
    if (_params.contains(key))
        result = _params.value(key);
    return result;
}

QString SParams::stringValue(const QString &key, const QString &defaultValue) const
{
    QString result = "";
    if (_params.contains(key))
        result = _params.value(key).toString();
    if (result.isEmpty())
        result = defaultValue;
    return result;
}

void SParams::add(const QString &values, const QString &delimeter, bool deleteQuotes)
{
    if (!values.trimmed().isEmpty()) {
        foreach (QString s, values.split(delimeter, QString::SkipEmptyParts)) {
            QString val = s.section("=",1).trimmed();
            if (deleteQuotes) val.remove("\"");
            _params.insert(s.section("=",0,0).trimmed(), val);
        }
    }
}

void SParams::add(const QStringList &keyValues, const QString &defaultParamName, bool deleteQuotes)
{
    int i=0;
    foreach (QString s, keyValues) {
        if (s.contains("=")) {
            QString val = s.section("=",1).trimmed();
            if (deleteQuotes) val.remove("\"");
            _params.insert(s.section("=",0,0).trimmed(), val);
        }
        else {
            QString val = s.trimmed();
            if (deleteQuotes) val.remove("\"");
            _params.insert(defaultParamName + QString::number(++i), val);
        }
    }
}

void SParams::add(const QHash<QString, QVariant> &keyValues, const QString &addPrefix)
{
    if (addPrefix.isEmpty()) {
        foreach (QString s, keyValues.keys())
            _params.insert(s, keyValues.value(s));
    } else {
        foreach (QString s, keyValues.keys())
            _params.insert(addPrefix+"."+s, keyValues.value(s));
    }
}


void SParams::add(const SParams &params, const QString &addPrefix)
{
    if (addPrefix.isEmpty()) {
        foreach (QString s, params.keys())
            _params.insert(s, params.value(s));
    } else {
        foreach (QString s, params.keys())
            _params.insert(addPrefix+"."+s, params.value(s));
    }
}

void SParams::add(const QByteArray &byteArray)
{
    QDataStream ds(byteArray);
    ds.setVersion(QDataStream::Qt_4_7);
    ds >> _params;
}

void SParams::add(int argc, char *argv[])
{
    QStringList listParams;

    for (int i=0; i<argc; i++)
        if (i==0)
            listParams.append(QString("programm=")+QString(argv[0]));
        else
            listParams.append(QString(argv[i]));

    add(listParams);
}

void SParams::addByPrefix(const SParams &params, const QString &prefix, bool deletePrefix)
{
    if (prefix.isEmpty())
        add(params);
    else {
        foreach (QString s, params.keys()) {
            if (s.indexOf(prefix+'.') == 0) {
                if (deletePrefix)
                    _params.insert(s.remove(0, prefix.size()+1), params.value(s));
                else
                    _params.insert(s, params.value(s));
            }
        }
    }
}

void SParams::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_7);
    stream >> _params;

    file.close();
}

void SParams::saveToFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_7);
    stream << _params;

    file.close();
}


void SParams::remove(const QString &key)
{
    _params.remove(key);
}

void SParams::remove(const QStringList &keys)
{
    foreach (QString s, keys) {
        _params.remove(s);
    }
}

void SParams::removeByPrefix(const QString &prefix)
{
    QList<QString> lst;
    foreach (QString s, _params.keys()) {
        lst = s.split(".");
        if (prefix == lst.first())
            _params.remove(s);
    }
}

QByteArray &SParams::toByteArray(QByteArray &dest) const
{
    dest.clear();
    QDataStream ds(&dest, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_7);
    ds << _params;
    return dest;
}

QString SParams::toString(const QString &separator, bool qouted) const
{
    QString result = "";
    foreach (QString s, _params.keys()) {
        if (qouted)
            result.append("\"" + s + "=" + stringValue(s).replace("\"", "&quot;") + "\"" + separator);
        else
            result.append(s + "=" + stringValue(s) + separator);
    }
    result.truncate(result.lastIndexOf(separator));

    return result;
}

QStringList SParams::toStringListSort()
{
    QStringList list;
    if(_params.isEmpty()) return list;
    foreach (QString s, _params.keys()) {
        list.append(s + "=" + stringValue(s));
    }
    list.sort();
    return list;
}

QByteArray SParams::calcMd5()
{
    QString str = toStringListSort().join(",");
    return QCryptographicHash::hash(str.toLocal8Bit(),QCryptographicHash::Md5).toHex();
}

void SParams::toHash(QHash <QString, QVariant> &dest, const QString &addPrefix)
{
    if (addPrefix.isEmpty()) {
        foreach (QString s, _params.keys())
            dest.insert(s, _params.value(s));
    } else {
        foreach (QString s, _params.keys())
            dest.insert(addPrefix+"."+s, _params.value(s));
    }
}

QString SParams::toText() const
{
    QString result = "";
    QStringList akeys;
    akeys.append(_params.keys());
    akeys.sort();

    foreach (QString s, akeys) {
        result.append(QString("* %1 = \"%2\"\n").arg(s).arg(stringValue(s)));
    }
    return result;
}

QString SParams::toHtml() const
{
    QString result = "<ul>";
    QStringList akeys;
    akeys.append(_params.keys());
    akeys.sort();

    foreach (QString s, akeys) {
        result.append(QString("<li><strong>%1</strong>=\"%2\"</li>").arg(s).arg(textToHtml(stringValue(s))));
    }
    result.append("</ul>");
    return result;
}

QString SParams::toXml(const QString &root, const QString &element)
{
    QString result = QString("<%1>").arg(root);
    foreach (QString s, _params.keys()) {
        QString str;
        if (stringValue(s).contains(QRegExp("[<|>|\\r|\\n|\\t]"))) {
            str.append("<![CDATA[");
            str.append(stringValue(s));
            str.append("]]>");
            }
        else
            str.append(stringValue(s));

        result.append(QString("<%1 name=\"%2\">%3</%1>").arg(element).arg(s).arg(str));
    }

    result.append(QString("</%1>").arg(root));
    return result;
}

QString SParams::toXmlAttrs()
{
  QString res = "";
  foreach(QString key, keys()){
    if (!value(key).toString().isEmpty())
      res += (key + "=\"" + value(key).toString() + "\" ");
  }
  return res;
}

QString SParams::toJson()
{
    QString result = "{";

    foreach (QString s, _params.keys()) {
        result.append(QString("\"%1\":\"%2\",").arg(s).arg(stringValue(s)));
    }
    result.truncate(result.lastIndexOf(","));
    result.append("}");

    return result;
}

QString SParams::toCsv()
{
    QString result;

    foreach (QString s, _params.keys()) {
        QString data = stringValue(s);
        QByteArray ba = data.toUtf8();
        data = QString(ba.toPercentEncoding());
        result.append(QString("\"%1=%2\"\n").arg(s).arg(data));
    }

    return result;
}

bool SParams::containsValue(const QVariant &value) const
{
    foreach (QString s, _params.keys()) {
        if (_params.value(s) == value) return true;
    }
    return false;
}

QList<QString> SParams::keysWithoutPrefix() const
{
    QStringList lst;
    foreach (QString s, _params.keys()) {
        lst.append(s.split(".").last());
    }
    return lst;
}

void SParams::clear()
{
    _params.clear();
}

int SParams::count() const
{
    return _params.count();
}

void SParams::clearKeyPrefix(const QString &prefix)
{
    std::ignore = prefix;

    QList<QString> lst;
    foreach (QString s, _params.keys()) {
        lst = s.split(".");
        _params.insert(lst.last(), _params.value(lst.last()));
        _params.remove(s);
    }
}

QString SParams::replaceVariables(const QString &str)
{
    QString vResult = str;

    foreach (QString s, _params.keys()) {
        if (_params.contains(s))
            vResult.replace("%"+s+"%", _params.value(s).toString());
    }

    return vResult;
}

QString SParams::textToHtml(const QString &text)
{
    QString result = text;
    result.replace("&", "&amp;");
    result.replace("%", "&#37;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    result.replace("\"", "&quot;");
    result.replace("\n", "<br />");

    return result;
}

SParams SParams::fromString(const QString &values)
{
    SParams result;
    result.add(values);
    return result;
}

SParams SParams::fromList(const QStringList &keyValues)
{
    SParams result;
    result.add(keyValues);
    return result;
}

SParams SParams::fromHash(QHash<QString, QVariant> &keyValues)
{
    SParams result;
    result.add(keyValues);
    return result;
}

SParams SParams::fromByteArray(const QByteArray &byteArray)
{
    SParams result;
    result.add(byteArray);
    return result;
}

QString SParams::stringToMd5(const QString &text)
{
    return QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5).toHex();
}

QString SParams::Md5Uuid()
{
    return stringToMd5(QUuid::createUuid().toString());
}

QString SParams::loadTextFile(const QString &fileName)
{
    QString result;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        result = stream.readAll();
        file.close();
    }
    return result;
}

void SParams::loadFromXml(const QString &xml, const QString &root, const QString &element)
{
    std::ignore = root;
    QXmlStreamReader reader(xml);
    QString str;

    reader.readNextStartElement();
    str = reader.name().toString();

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        str = reader.name().toString();

        if ((reader.name() == element)) {
            QString paramName = reader.attributes().value("name").toString();
            _params.insert(paramName, reader.readElementText());
        }
    }
}

void SParams::loadFromXmlFile(const QString &xml, const QString &root, const QString &element)
{
    QString str;
    str = loadTextFile(xml);
    if (!str.isEmpty())
        loadFromXml(str, root, element);
}



QDataStream& operator<<(QDataStream &out, const SParams& sparams)
{
    out.setVersion(QDataStream::Qt_4_7);
    out << sparams._params;
    return out;
}

QDataStream& operator<<(QDataStream &out, const SParams* sparams)
{
    out.setVersion(QDataStream::Qt_4_7);
    out << sparams->_params;
    return out;
}


QDataStream& operator>>(QDataStream &in, SParams& sparams)
{
    QHash <QString, QVariant> params;
    in.setVersion(QDataStream::Qt_4_7);
    in >> sparams._params;
    return in;
}

QDataStream& operator>>(QDataStream &in, SParams* sparams)
{
    QHash <QString, QVariant> params;
    in.setVersion(QDataStream::Qt_4_7);
    in >> sparams->_params;
    return in;
}

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<(QDebug dbg, const SParams &r)
{
    dbg.nospace() << "SParams(itemCount=" << r.count() << ", " << r.toString(",", true) << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const SParams *r)
{
    dbg.nospace() << "SParams*(itemCount=" << r->count() << ", " << r->toString(",", true) << ")";
    return dbg.space();
}



#endif



