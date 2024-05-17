#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H
#include <QSerialPort>

enum class SerialConnectionState {
    FileNotFound,
    FileReadError,
    LastSerialOperationNotCompleted,
    NoError,
    NothingToBeReaded,
    SerialPortNotOpened
};

class SerialConnection {
public:
    explicit SerialConnection(const QString& SerialPortName, QSerialPort::BaudRate BaudRate=QSerialPort::Baud9600);
    ~SerialConnection();

    [[nodiscard]] SerialConnectionState writeString(const QByteArray &data) const;
    [[nodiscard]] SerialConnectionState writeFile(const QString& filename)  const;
    [[nodiscard]] QByteArray            readString()                        const;
    [[nodiscard]] SerialConnectionState readFile(const QString& NewFileName)const;
    [[nodiscard]] SerialConnectionState closeConnection()                   const;
    [[nodiscard]] QString               getCurrentSerialPortName()          const;


private:
    QSerialPort             *SerialPort{};
    QString                 CurrentConnectedSerialPortName;
    QSerialPort::BaudRate   CurrentConnectedSerialPortBaudRate;
};
#endif //SERIAL_CONNECTION_H
