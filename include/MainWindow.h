//
// Created by daydalek on 24-5-12.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SerialConnection.h"
#include "UiWindow.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);

    void getAvaliableSerialPorts();

    ~MainWindow() override;

  private:
    SerialMonitor *ui;
    SerialConnection *UiSerialConnection{};
    QStandardItemModel *SerialPortModel;
    QStandardItemModel *BaudRateModel{};
    std::vector<QSerialPort::BaudRate> BaudRateList = {QSerialPort::Baud9600, QSerialPort::Baud19200,
                                                       QSerialPort::Baud38400, QSerialPort::Baud57600,
                                                       QSerialPort::Baud115200};

  private slots:
    void connectSlots();

    void createConnection();

    void terminateConnection();

    void sendData();

    void receiveData();

    void initBaudRate();

    void clearAll();
};

#endif // MAINWINDOW_H
