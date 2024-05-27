/**
 * @file    DataWrapper.h
 * @brief   This class wrap data to be a MessagePack object
 * @note    by wrapping data to a json object,we can provide
 *          more control like point out whether these data are plain text
 *          or file, bring MD5 with the data to checkout if there's any
 *          mistake or not,or even encrypt the data.
 */

#include "LocalEnumType.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCryptographicHash>

#ifndef DATAWRAPPER_H
#define DATAWRAPPER_H


class DataWrapper {
  public:
    DataWrapper() = default;
    QByteArray wrapData(const QByteArray &data, DataType type = DataType::Text,
                        bool withMd5 = false);
};

#endif // DATAWRAPPER_H
