#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "ConfigStruct.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
class ConfigParser {
    ConfigParser() = default;

  public:
    static ConfigStruct
    parseConfig(const QString &ConfigFilePath = "config.json");
};

#endif // CONFIGPARSER_H
