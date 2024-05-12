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

    [[nodiscard]] SerialConnectionState WriteString(const QByteArray &data) const;

    [[nodiscard]] SerialConnectionState WriteFile(const std::string &filename) const;

    [[nodiscard]] QByteArray ReadString() const;

    [[nodiscard]] SerialConnectionState ReadFile(const std::string &newFileName) const;

private:
    QSerialPort *SerialPort;
    QString CurrentConnectedSerialPortName;
    QSerialPort::BaudRate CurrentConnectedSerialPortBaudRate;

};
#endif //SERIAL_CONNECTION_H
