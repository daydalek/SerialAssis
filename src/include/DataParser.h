
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
    DataType getType(const QByteArray &data) const;
    QByteArray getData(const QByteArray &data) const;
    QByteArray getMD5(const QByteArray &data) const;
    Md5Validation validateMd5(const QByteArray &data) const;
};

#endif // DATAPARSER_H
