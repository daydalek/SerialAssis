/**
 * @file    SerialConnection.h
 * @brief   This class is responsible for the serial connection
 * @note    This class is a wrapper of QSerialPort,including sending data, sending file, receiving data,
 *          also there's a Errorcode class named SerialConnectionState ,which can be passed to MainWindow to
 *          do some specific operations,such as pop up a window to warn users that serialport is not opened.
 */

#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H
#include <QSerialPort>
#include <QObject>
#include <QFile>

/**
 *  @brief  SerialConnectionState is the error code class of SerialConnection ,
 *          use to simplify the error handling in MainWindow
 */
enum class SerialConnectionState {
    FileNotFound,
    FileReadError,
    LastSerialOperationNotCompleted,
    NoError,
    NothingToBeReaded,
    SerialPortNotOpened
};

class SerialConnection : public QObject{
    Q_OBJECT
public:
    explicit SerialConnection(const QString& SerialPortName, \
        QSerialPort::BaudRate BaudRate=QSerialPort::Baud9600);
            ~SerialConnection() override;

    [[nodiscard]] SerialConnectionState writeData(const QByteArray &data)       const;
    [[nodiscard]] SerialConnectionState writeFile(const QString& NewFileName)   const;
    [[nodiscard]] SerialConnectionState closeConnection()                       const;
    [[nodiscard]] QByteArray            readData()                              ;

    signals:
    void dataFullyWritten();
    void dataReadyToRead();

private:
    QSerialPort             *SerialPort = nullptr;
    QSerialPort::BaudRate   CurrentConnectedSerialPortBaudRate;
    QString                 CurrentConnectedSerialPortName;

private slots:
    void onBytesWritten();
};
#endif //SERIAL_CONNECTION_H
