//
// Created by daydalek on 24-5-12.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <vector>
#include "MainWindow.h"
#include "SerialConnection.h"
#include "UiWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void getAvaliableSerialPorts();

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QSerialPort::BaudRate CurrentBaudRate = QSerialPort::Baud9600;
    SerialConnection *UiSerialConnection{};
    QStandardItemModel *SerialPortModel;
    QStandardItemModel *BaudRateModel{};
    std::vector<QSerialPort::BaudRate> BaudRateList = {
        QSerialPort::Baud9600,
        QSerialPort::Baud19200,
        QSerialPort::Baud38400,
        QSerialPort::Baud57600,
        QSerialPort::Baud115200
    };

private slots:
    void connectSlots();

    void createConnection();

    void terminateConnection();

    void sendData();

    void receiveData();

    void initBaudRate();

    void chooseBaudRate();
};

#endif //MAINWINDOW_H
