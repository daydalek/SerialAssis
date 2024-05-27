
#include "include/DataParser.h"

DataType DataParser::getType(const QByteArray &data) {
    QJsonDocument DataDocument = QJsonDocument::fromJson(data);
    QJsonObject DataObject = DataDocument.object();
    return static_cast<DataType>(DataObject["type"].toInt());
}

QByteArray DataParser::getData(const QByteArray &data) {
    QJsonDocument DataDocument = QJsonDocument::fromJson(data);
    QJsonObject DataObject = DataDocument.object();
    return QByteArray::fromBase64(DataObject["Data"].toString().toUtf8());
}

QByteArray DataParser::getMD5(const QByteArray &data) {
    QJsonDocument DataDocument = QJsonDocument::fromJson(data);
    QJsonObject DataObject = DataDocument.object();
    return DataObject["md5"].toString().toUtf8();
}

Md5Validation DataParser::validateMd5(const QByteArray &data) {
    QJsonDocument DataDocument = QJsonDocument::fromJson(data);
    QJsonObject DataObject = DataDocument.object();
    if (DataObject["md5"].toString().isEmpty()) {
        return Md5Validation::Invalid;
    }
    QByteArray Data =
        QByteArray::fromBase64(DataObject["Data"].toString().toUtf8());
    QString Md5 = DataObject["md5"].toString();
    auto hash = QString(QCryptographicHash::hash(Data, QCryptographicHash::Md5)
                            .toHex()
                            .toBase64());
    if (hash == Md5) {
        return Md5Validation::Passed;
    }
    return Md5Validation::Failed;
}