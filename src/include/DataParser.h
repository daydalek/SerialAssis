
#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "LocalEnumType.h"
#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class DataParser {
  public:
    DataParser() = default;

  public:
    static DataType        getType     (const QByteArray &data) ;
    static QByteArray      getData     (const QByteArray &data) ;
    static QByteArray      getMD5      (const QByteArray &data) ;
    static Md5Validation   validateMd5 (const QByteArray &data) ;
};

#endif // DATAPARSER_H
