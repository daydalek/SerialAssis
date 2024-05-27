/**
 * @file    MainWindow.h
 * @brief   This class is responsible for connecting the UI and the serial connection together.
 * @note    including call the functions in SerialConnection when buttons are clicked,
 *          insert BaudRate into BaudRateList,update SerialPortList constantly and show
 *          the data received from serial port in the TextEdit widget or save it as file.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SerialConnection.h"
#include "SerialMonitor.h"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <vector>


class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private:
    SerialMonitor                      *UI                            = nullptr;
    SerialConnection                   *UiHandledSerialConnection     = nullptr;
    QStandardItemModel                 *SerialPortModel               = nullptr;
    QStandardItemModel                 *BaudRateModel                 = nullptr;
    std::vector<QSerialPort::BaudRate> BaudRateList = {QSerialPort::Baud9600, QSerialPort::Baud19200,
                                                       QSerialPort::Baud38400, QSerialPort::Baud57600,
                                                       QSerialPort::Baud115200};
    /**
     * @note    due to unknown reason,the messagebox to inform user that sending completed is
     *          shown multiple times,so I have to use this ugly way to prevent it from happening.
     *          I'll find a way to fix it sooner or later.
     */
    bool                                CompletionBoxShown            = false;


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
    void showDataWrittenWindow();
};

#endif // MAINWINDOW_H
