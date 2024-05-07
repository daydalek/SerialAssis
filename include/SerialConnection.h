//
// Created by daydalek on 24-5-7.
//

#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H
#include <string>
#include <QSerialPort>

enum class SerialConnectionState {
    NoError,
    SerialPortNotOpened,
    FileNotFound,
    FileReadError,
    LastSerialOperationNotCompleted,
    NothingToBeReaded
};

class SerialConnection {
public:
    SerialConnection(const std::string &SerialPortName, QSerialPort::BaudRate BaudRate);

    ~SerialConnection();

    SerialConnectionState writeString(const QByteArray &data) const;

    SerialConnectionState writeFile(const std::string &filename) const;

    QByteArray readString() const;

    SerialConnectionState readFile(const std::string &newFileName) const;

private:
    QSerialPort *serialPort;
};
#endif //SERIAL_CONNECTION_H
