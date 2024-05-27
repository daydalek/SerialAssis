/**
 * @file    DataWrapper.cpp
 * @brief   Implementations of DataWrapper.h
 */

#include "include/DataWrapper.h"

QByteArray DataWrapper::wrapData(const QByteArray &data, DataType type,
                                 bool withMd5) {
    QJsonObject DataObject;
    QJsonValue DataValue(QString(data.toBase64()));
    DataObject.insert("Data", DataValue);
    DataObject.insert("type", static_cast<int>(type));
    if (withMd5) {
        QByteArray Md5 =
            QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
        QJsonValue Md5Value(QString(Md5.toBase64()));
        DataObject.insert("md5", Md5Value);
    }
    QJsonDocument DataDocument(DataObject);
    QByteArray WrappedDataByteArray = DataDocument.toJson();
    return WrappedDataByteArray;
}
