//
// Created by daydalek on 24-5-12.
//

#include "../include/MainWindow.h"
#include "../include/SerialConnection.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>
#include <QSerialPortInfo>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connectSlots();
    this->SerialPortModel = new QStandardItemModel(this);
    this->BaudRateModel=new QStandardItemModel(this);
    initBaudRate();
    getAvaliableSerialPorts();
    auto *RefreshSerialPortList = new QTimer(this);
    connect(RefreshSerialPortList, &QTimer::timeout, this, &MainWindow::getAvaliableSerialPorts);
    RefreshSerialPortList->start(5000);
}

MainWindow::~MainWindow() {
    delete ui;
    delete UiSerialConnection;
}

void MainWindow::getAvaliableSerialPorts() {
    QList<QSerialPortInfo> SerialPorts = QSerialPortInfo::availablePorts();
    this->SerialPortModel->clear();
    for (const auto &SerialPort: SerialPorts) {
        auto *NewItem = new QStandardItem(SerialPort.portName());
        this->SerialPortModel->appendRow(NewItem);
    }
    ui->SerialList->setModel(this->SerialPortModel);
}

void MainWindow::initBaudRate() {
    for (const auto &BaudRate: this->BaudRateList) {
        auto *NewItem = new QStandardItem(QString::number(BaudRate));
        this->BaudRateModel->appendRow(NewItem);
    }
    ui->BaudRateList->setModel(this->BaudRateModel);
}


/**
* connect all buttons,lists and text widgets to slot functions
*/

void MainWindow::connectSlots() {
    connect(ui->EstablishConnectionButton, &QPushButton::clicked, this, this->createConnection);
    connect(ui->StopConnectionButton, &QPushButton::clicked, this, this->terminateConnection);
    connect(ui->SendDataButton, &QPushButton::clicked, this, this->sendData);
    connect(ui->ReceiveDataButton, &QPushButton::clicked, this, this->receiveData);
}

/**
 * get selected serial port name in SerialList and create a connection with it's name
 * get selected baud rate in BaudRateList and use it as the baudrate of the connection,default at Baud9600
 */

void MainWindow::createConnection() {
    if(ui->SerialList->currentIndex().isValid()) {
        QString SerialPortName = ui->SerialList->currentIndex().data().toString();
        // assert(SerialPortName != "");
        QSerialPort::BaudRate BaudRate = static_cast<QSerialPort::BaudRate>(ui->BaudRateList->currentIndex().data().
            toInt());
        this->UiSerialConnection = new SerialConnection(SerialPortName.toStdString(), BaudRate);
    }else {
        QMessageBox::warning(this, "Error", "Serial Port Not Selected");
    }
}


/**
* TerminalConnection when StopConnectionButton is clicked by calling SerialConnection::CloseConnection()
* Error Code SerialConnectionState::LastSerialConnectionNotCompleted is used to
* show warning MessageBox when close operation failed
 */

void MainWindow::terminateConnection() {
    if(this->UiSerialConnection == nullptr){
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto State = this->UiSerialConnection->closeConnection();
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

void MainWindow::sendData() {
    if (this->UiSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Seiral Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    auto State = this->UiSerialConnection->writeString(ui->DataToSendTextBox->toPlainText().toUtf8());
    /*
     * Possible State:SerialConnectionState::SerialPortNotOpened
     */
    if (State == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
}

void MainWindow::receiveData() {
    if (this->UiSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Seiral Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    auto Data = this->UiSerialConnection->readString();
    ui->DataToSendTextBox->setText(QString::fromUtf8(Data));
}
