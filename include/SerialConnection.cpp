//
// Created by daydalek on 24-5-7.
//

#include "SerialConnection.h"
#include <QFile>

SerialConnection::SerialConnection(const std::string &SerialPortName, QSerialPort::BaudRate BaudRate) {
    serialPort = new QSerialPort(QString::fromStdString(SerialPortName));
    serialPort->setBaudRate(BaudRate);
    serialPort->open(QIODevice::ReadWrite);
}

SerialConnection::~SerialConnection() {
    serialPort->close();
    delete serialPort;
}

SerialConnectionState SerialConnection::writeString(const QByteArray &data) const{
    assert(serialPort!=nullptr);
    if (serialPort->write(data) == -1) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    return SerialConnectionState::NoError;
}

SerialConnectionState SerialConnection::writeFile(const std::string &filename) const{
    assert(serialPort!=nullptr);
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        return SerialConnectionState::FileNotFound;
    }
    QByteArray data = file.readAll();
    if (serialPort->bytesToWrite() > 0) {
        return SerialConnectionState::LastSerialOperationNotCompleted;
    }
    if (serialPort->write(data) == -1) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    return SerialConnectionState::NoError;
}

QByteArray SerialConnection::readString() const{
    assert(serialPort!=nullptr);
    if (serialPort->bytesAvailable() == 0) {
        return nullptr;
    }
    QByteArray data = serialPort->readAll();
    return data;
}

SerialConnectionState SerialConnection::readFile(const std::string &newFileName) const{
    assert(serialPort!=nullptr);
    QFile file(QString::fromStdString(newFileName));
    if (serialPort->bytesAvailable() == 0) {
        return SerialConnectionState::NothingToBeReaded;
    }
    const QByteArray data = serialPort->readAll();
    file.write(data);
    return SerialConnectionState::NoError;
}
