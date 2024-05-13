//
// Created by daydalek on 24-5-12.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainWindow.h"
#include "SerialConnection.h"
#include "UiWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialConnection *UiSerialConnection{};

private slots:
    void ConnectSlots();

    void CreateConnection();

    void TerminateConnection();

    void SendData();

    void ReceiveData();
};

#endif //MAINWINDOW_H
