
#include "../include/MainWindow.h"
#include "../include/SerialConnection.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>
#include <QDebug>
#include <QSerialPortInfo>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connectSlots();
    this->SerialPortModel = new QStandardItemModel(this);
    this->BaudRateModel = new QStandardItemModel(this);
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

/**
 * get all available serial ports and append them to SerialList
 */
void MainWindow::getAvaliableSerialPorts() {
    QList<QSerialPortInfo> SerialPorts = QSerialPortInfo::availablePorts();
    // for(auto &SerialPort: SerialPorts){
    // qDebug() << SerialPort.portName();
    // }
    if (SerialPorts.empty()) {
        qDebug() << "No Serial Ports Found";
        return;
    }
    this->SerialPortModel->clear();
    for (const auto &SerialPort: SerialPorts) {
        auto *NewItem = new QStandardItem(SerialPort.portName());
        // qDebug() << SerialPort.portName();
        this->SerialPortModel->appendRow(NewItem);
    }
    ui->SerialList->setModel(this->SerialPortModel);
}

/**
 * insert all baud rates in BaudRateList to BaudRateModel
 * BaudRate:9600,19200,38400,57600,115200
 default at Baud9600
 */
void MainWindow::initBaudRate() {
    for (const auto &BaudRate: this->BaudRateList) {
        auto *NewItem = new QStandardItem(QString::number(BaudRate));
        NewItem->setData(QString::number(BaudRate));
        // std::cout << QString::number(BaudRate).toStdString() << std::endl;
        this->BaudRateModel->appendRow(NewItem);
        // std::cout << NewItem->data().toString().toStdString() << std::endl;
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
    connect(ui->ChooseBaudRate, &QPushButton::clicked, this, this->chooseBaudRate);
}

/**
 * get selected serial port name in SerialList and create a connection with it's name
 * get selected baud rate in BaudRateList and use it as the baudrate of the connection,default at Baud9600
 */

void MainWindow::createConnection() {
    if (ui->SerialList->currentIndex().isValid()) {
        QString SerialPortName = ui->SerialList->currentIndex().data().toString();
        if (this->UiSerialConnection != nullptr) {
            QMessageBox::warning(this, "Error", "Serial Port Already Opened");
            return;
        }
        this->UiSerialConnection = new SerialConnection(SerialPortName, this->CurrentBaudRate);
    } else {
        QMessageBox::warning(this, "Error", "Serial Port Not Selected");
    }
}


/**
* TerminalConnection when StopConnectionButton is clicked by calling SerialConnection::CloseConnection()
* Error Code SerialConnectionState::LastSerialConnectionNotCompleted is used to
* show warning MessageBox when close operation failed
 */

void MainWindow::terminateConnection() {
    if (this->UiSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto State = this->UiSerialConnection->closeConnection();
    if (State == SerialConnectionState::LastSerialOperationNotCompleted) {
        /*
         *Possible State:SerialConnectionState::LastSerialOperationNotCompleted
         */
        QMessageBox::warning(this, "Error", "Last Serial Operation Not Completed");
    } else {
        this->UiSerialConnection = nullptr;
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

/**
 * receive data from another device by calling SerialConnection::ReadString()
 * Error Code SerialConnection::NothingToBeReaded is used to show warning MessageBox
 * that there is no data to be readed
 */

void MainWindow::receiveData() {
    if (this->UiSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    auto Data = this->UiSerialConnection->readString();
    ui->DataToSendTextBox->setText(QString::fromUtf8(Data));
}

/**
 * choose a new baud rate from BaudRateList and store it in CurrentBaudRate
 * BaudRate is applied when the SerialConnection is closed and reopened
 */
void MainWindow::chooseBaudRate() {
    if (this->ui->BaudRateList->currentIndex().isValid()) {
        auto BaudRateSelected = ui->BaudRateList->currentIndex().data().toString().toInt();
        if (this->UiSerialConnection == nullptr) {
            QMessageBox::warning(this, "Error", "Serial Port Not Opened");
            return;
        }
        this->CurrentBaudRate = static_cast<QSerialPort::BaudRate>(BaudRateSelected);
    } else {
        return; //BaudRate remains the same
        // this->CurrentBaudRate = QSerialPort::Baud9600;
    }
}
