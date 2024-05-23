#include "../include/SerialConnection.h"
#include <QFile>
#include <QMessageBox>

#define SERIAL_UNABLE_TO_WRITE -1

/**
 * @param SerialPortName
 * @param BaudRate as the baudrate for the serial connection , chosed by the user in user interface , default at 9600
 * @return a SerialConnection object
 */

SerialConnection::SerialConnection(const QString &SerialPortName, QSerialPort::BaudRate BaudRate) {
    SerialPort = new QSerialPort();
    SerialPort->setPortName(SerialPortName);
    SerialPort->setBaudRate(BaudRate);
    SerialPort->open(QIODevice::ReadWrite);
    if (!SerialPort->isOpen()) {
        throw std::runtime_error("Serial Port is not opened");
    }
    CurrentConnectedSerialPortBaudRate = BaudRate;
}

SerialConnection::~SerialConnection() {
    SerialPort->close();
    delete SerialPort;
}

/**
 * @param data as the data to be written, which is inputted in the TextEdit widget in user interface
 * @return the Error Code of the operation
 */

SerialConnectionState SerialConnection::writeData(const QByteArray &data) const {
    // assert(SerialPort != nullptr);
    if (SerialPort->write(data) == SERIAL_UNABLE_TO_WRITE) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    // connect(SerialPort, &QSerialPort::bytesWritten, this, &SerialConnection::onBytesWritten);
    return SerialConnectionState::NoError;
}

SerialConnectionState SerialConnection::writeFile(const QString &NewFileName) const {
    QFile file(NewFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return SerialConnectionState::FileNotFound;
    }
    QByteArray data = file.readAll();
    if (SerialPort->write(data) == SERIAL_UNABLE_TO_WRITE) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    // connect(SerialPort, &QSerialPort::bytesWritten, this, &SerialConnection::onBytesWritten);
    return SerialConnectionState::NoError;
}

/**
 *  read the data inputted in the QTextEdit widget on another device
 * @return the data readed from the serial port
 */

QByteArray SerialConnection::readData() const {
    if (SerialPort->bytesAvailable() == 0) {
        return nullptr;
    }
    QByteArray data = SerialPort->readAll();
    // QMessageBox::information(nullptr, "Data Received", "Data Received Successfully");
    return data;
}

/**
 * @return SerialPort is closed only when last operation is finished(which is no bytes to read and no bytes to write)
 */

SerialConnectionState SerialConnection::closeConnection() const {
    // assert(SerialPort != nullptr);
    if (SerialPort->bytesAvailable() == 0 && SerialPort->bytesToWrite() == 0) {
        SerialPort->close();
        return SerialConnectionState::NoError;
    }
    return SerialConnectionState::LastSerialOperationNotCompleted;
}


void SerialConnection::onBytesWritten() {
    emit dataWritten();
}