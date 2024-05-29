#ifndef CONFIGSTRUCT_H
#define CONFIGSTRUCT_H

#include <QString>

class ConfigStruct {
  public:
    bool      SendOnBoot;
    bool      ReceiveOnBoot;
    QString   DefaultSaveFile;
    QString   DefaultSendSerialPort;
    QString   DefaultReceiveSerialPort;
};

#endif // CONFIGSTRUCT_H
