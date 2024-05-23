#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SerialConnection.h"
#include "SerialMonitor.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
            ~MainWindow() override;

  private:
    SerialMonitor                       *ui                         =nullptr;
    SerialConnection                    *UiHandledSerialConnection  =nullptr;
    QStandardItemModel                  *SerialPortModel            =nullptr;
    QStandardItemModel                  *BaudRateModel              =nullptr;
    std::vector<QSerialPort::BaudRate>  BaudRateList = {QSerialPort::Baud9600,  QSerialPort::Baud19200,
                                                        QSerialPort::Baud38400, QSerialPort::Baud57600,
                                                        QSerialPort::Baud115200
                                                        };

  private slots:
    void connectSlots();
    void createConnection();
    void terminateConnection();
    void sendText();
    void sendFile();
    void receiveText();
    void receiveFile();
    void initBaudRate();
    void clearAll();
    void getAvaliableSerialPorts();
    void dataWrittenHint();
};

#endif // MAINWINDOW_H
