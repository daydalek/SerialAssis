//
// Created by daydalek on 24-5-7.
//

#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H
#include <string>
#include <QSerialPort>
#include <set>

enum class SerialConnectionState {
    NoError,
    SerialPortNotOpened,
    FileNotFound,
    FileReadError,
    LastSerialOperationNotCompleted,
    NothingToBeReaded,
    NothingToSend,
    BlankSerialPortName
};

class SerialConnection {
public:
    SerialConnection(const std::string &SerialPortName, QSerialPort::BaudRate BaudRate);

    ~SerialConnection();

    [[nodiscard]] SerialConnectionState writeString(const QByteArray &data) const;

    [[nodiscard]] SerialConnectionState writeFile(const std::string &filename) const;

    [[nodiscard]] QByteArray readString() const;

    [[nodiscard]] SerialConnectionState readFile(const std::string &newFileName) const;

    [[nodiscard]] SerialConnectionState closeConnection() const;


private:
    QSerialPort *SerialPort;
    QString CurrentConnectedSerialPortName;
    QSerialPort::BaudRate CurrentConnectedSerialPortBaudRate;

};
#endif //SERIAL_CONNECTION_H
