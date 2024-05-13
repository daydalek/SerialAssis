//
// Created by daydalek on 24-5-12.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>

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
    SerialConnection *UiSerialConnection{};
    QStandardItemModel *SerialPortModel;

private slots:
    void connectSlots();

    void createConnection();

    void terminateConnection();

    void sendData();

    void receiveData();
};

#endif //MAINWINDOW_H
