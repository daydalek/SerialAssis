//
// Created by daydalek on 24-5-7.
//

#include "../include/SerialConnection.h"
#include <QFile>

/**
 *
 * @param SerialPortName
 * @param BaudRate as the baudrate for the serial connection , chosed by the user in user interface , default at 9600
 * \return a SerialConnection object
 */

SerialConnection::SerialConnection(const std::string &SerialPortName,
                                   QSerialPort::BaudRate BaudRate = QSerialPort::Baud9600) {
    SerialPort = new QSerialPort(QString::fromStdString(SerialPortName));
    SerialPort->setBaudRate(BaudRate);
    SerialPort->open(QIODevice::ReadWrite);
    CurrentConnectedSerialPortName = QString::fromStdString(SerialPortName);
    CurrentConnectedSerialPortBaudRate = BaudRate;
}

SerialConnection::~SerialConnection() {
    SerialPort->close();
    delete SerialPort;
}

/**
 *
 * @param data as the data to be written, which is inputted in the TextEdit widget in user interface
 * @return
 */
SerialConnectionState SerialConnection::writeString(const QByteArray &data) const {
    assert(SerialPort != nullptr);
    if (SerialPort->write(data) == -1) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    return SerialConnectionState::NoError;
}

/**
 *
 * @param filename as the name of the file to send via serial connection
 * @return
 */
SerialConnectionState SerialConnection::writeFile(const std::string &filename) const {
    assert(SerialPort != nullptr);
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly)) {
        return SerialConnectionState::FileNotFound;
    }
    QByteArray data = file.readAll();
    if (SerialPort->bytesToWrite() > 0) {
        return SerialConnectionState::LastSerialOperationNotCompleted;
    }
    if (SerialPort->write(data) == -1) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    return SerialConnectionState::NoError;
}

/**
 *  read the data inputted in the TextEdit widget on another device
 * @return
 */
QByteArray SerialConnection::readString() const {
    assert(SerialPort != nullptr);
    if (SerialPort->bytesAvailable() == 0) {
        return nullptr;
    }
    QByteArray data = SerialPort->readAll();
    return data;
}

/**
 *
 * @param newFileName as the file name to be created to save the data received
 * @return
 */
SerialConnectionState SerialConnection::readFile(const std::string &newFileName) const {
    assert(SerialPort != nullptr);
    QFile file(QString::fromStdString(newFileName));
    if (SerialPort->bytesAvailable() == 0) {
        return SerialConnectionState::NothingToBeReaded;
    }
    const QByteArray data = SerialPort->readAll();
    file.write(data);
    return SerialConnectionState::NoError;
}

/**
 * 
 * @return SerialPort is closed only when last operation is finished(which is no bytes to read and no bytes to write)
 */
SerialConnectionState SerialConnection::closeConnection() const {
    assert(SerialPort != nullptr);
    if (SerialPort->bytesAvailable() == 0 && SerialPort->bytesToWrite() == 0) {
        SerialPort->close();
        return SerialConnectionState::NoError;
    } else return SerialConnectionState::LastSerialOperationNotCompleted;
}

