//
// Created by daydalek on 24-5-12.
//

#include "../include/MainWindow.h"
#include "../include/SerialConnection.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // ConnectSlots();
}

MainWindow::~MainWindow() {
    delete ui;
}

/**
* connect all buttons,lists and text widgets to slot functions
*/
void MainWindow::ConnectSlots() {
    connect(ui->EstablishConnectionButton, &QPushButton::clicked, this, this->CreateConnection);
    connect(ui->StopConnectionButton, &QPushButton::clicked, this, this->TerminateConnection);
    connect(ui->SendDataButton, &QPushButton::clicked, this, this->SendData);
    connect(ui->ReceiveDataButton, &QPushButton::clicked, this, this->ReceiveData);
}

/**
 * get selected serial port name in SerialList and create a connection with it's name
 * get selected baud rate in BaudRateList and use it as the BaudRate of the Connection,default at Baud9600
 */
void MainWindow::CreateConnection() {
    QString SerialPortName = ui->SerialList->currentIndex().data().toString();
    QSerialPort::BaudRate BaudRate = static_cast<QSerialPort::BaudRate>(ui->BaudRateList->currentIndex().data().
        toInt());
    this->UiSerialConnection = new SerialConnection(SerialPortName.toStdString(), BaudRate);
}


/**
* TerminalConnection when StopConnectionButton is clicked by calling SerialConnection::CloseConnection()
* Error Code SerialConnectionState::LastSerialConnectionNotCompleted is used to
* show warning MessageBox when close operation failed
 */

void MainWindow::TerminateConnection() {
    auto State = this->UiSerialConnection->CloseConnection();
    if (State == SerialConnectionState::NoError) {
        delete this->UiSerialConnection;
    } else {
        /*
         *Possible State:SerialConnectionState::LastSerialOperationNotCompleted
         */
        QMessageBox::warning(this, "Error", "Last Serial Operation Not Completed");
    }
}

/**
 * send text in TextEdit widget ( DataToSend ) by calling SerialConnection::WriteString()
 * Error Code SerialConnection::SerialPortNotOpened is used to show warning MessageBox
 * that SerialConnection hasn't established yet
 */
void MainWindow::SendData() {
    auto State = this->UiSerialConnection->WriteString(ui->DataToSendTextBox->toPlainText().toUtf8());
    /*
     * Possible State:SerialConnectionState::SerialPortNotOpened
     */
    if (State == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
}
