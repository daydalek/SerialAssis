//
// Created by daydalek on 24-5-12.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainWindow.h"
#include "SerialConnection.h"
#include"UiWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void ConnectSlots();

private:
    Ui::MainWindow *ui;
    SerialConnection *serialConnection{};

private slots:
    void CreateConnection();

    void CloseConnection();

    void SendData();

    void ReceiveData();
};


#endif //MAINWINDOW_H
