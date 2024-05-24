#include "../include/SerialConnection.h"

#include <QFile>
#include <QMessageBox>

#define SERIAL_UNABLE_TO_WRITE -1

/**
 * @param SerialPortName    the name of the serial port to open, which is shown in the ComboBox widget
 *                          in user interface and chosed by the user.
 * @param BaudRate          as the baudrate for the serial connection , chosed by the user in user interface , default at 9600
 * @return                  a SerialConnection object
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
 * @param           DataToWrite as the data to be written, which is inputted in the TextEdit widget in user interface
 * @return          the Error Code of the operation,used to show warning MessageBox when write operation failed
 */

SerialConnectionState SerialConnection::writeData(const QByteArray &DataToWrite) const {
    // assert(SerialPort != nullptr);
    if (SerialPort->write(DataToWrite) == SERIAL_UNABLE_TO_WRITE) {
        return SerialConnectionState::SerialPortNotOpened;
    }
    // connect(SerialPort, &QSerialPort::bytesWritten, this, &SerialConnection::onBytesWritten);
    return SerialConnectionState::NoError;
}

SerialConnectionState SerialConnection::writeFile(const QString &NameOfFileToWrite) const {
    QFile file(NameOfFileToWrite);
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
 * @return  data readed from serialport. these data will be passed to MainWindow,user is responsible to deside
 *          whether to save it as file or show in the TextEdit widget.
 */

QByteArray SerialConnection::readData() const {
    if (SerialPort->bytesAvailable() == 0) {
        return nullptr;
    }
    QByteArray DataFromSerialPort = SerialPort->readAll();
    // QMessageBox::information(nullptr, "Data Received", "Data Received Successfully");
    return DataFromSerialPort;
}

/**
 * @return  the Error Code. SerialPort is closed only when last operation is finished(which is nothing to read and nothing to write)
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
