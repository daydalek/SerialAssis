#include "include/ConfigParser.h"

ConfigStruct ConfigParser::parseConfig(const QString &ConfigFilePath) {
    ConfigStruct config = ConfigStruct{false, false, "", "", ""};
    QFile file(ConfigFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return config;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject obj = doc.object();
    if (obj.contains("SendOnBoot"))
        config.SendOnBoot = obj["SendOnBoot"].toBool();
    if (obj.contains("ReceiveOnBoot")) {
        config.ReceiveOnBoot = obj["ReceiveOnBoot"].toBool();
        if (config.SendOnBoot && config.ReceiveOnBoot) {
            config.SendOnBoot = false;
            /*don't allow send on boot and receive on boot
             * at the same time*/
        }
    }
    if (obj.contains("DefaultSaveFile"))
        config.DefaultSaveFile = obj["DefaultSaveFile"].toString();
    if (obj.contains("DefaultSendSerialPort"))
        config.DefaultSendSerialPort = obj["DefaultSendSerialPort"].toString();
    if (obj.contains("DefaultReceiveSerialPort"))
        config.DefaultReceiveSerialPort = obj["DefaultReceiveSerialPort"].toString();
    file.close();
    return config;
}