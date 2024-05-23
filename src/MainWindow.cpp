
#include "../include/MainWindow.h"
#include "../include/SerialConnection.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new SerialMonitor) {
    setCentralWidget(ui);
    connectSlots();
    this->SerialPortModel = new QStandardItemModel(this);
    this->BaudRateModel = new QStandardItemModel(this);
    initBaudRate();
    getAvaliableSerialPorts();
}

MainWindow::~MainWindow() {
    delete ui;
    delete UiHandledSerialConnection;
}

/**
 * get all available serial ports and append them to SerialList
 */

void MainWindow::getAvaliableSerialPorts() {
    if (this->UiHandledSerialConnection != nullptr) {
        return;
    }
    QList<QSerialPortInfo> SerialPorts = QSerialPortInfo::availablePorts();
    // for(auto &SerialPort: SerialPorts){
    // qDebug() << SerialPort.portName();
    // }
    if (SerialPorts.empty()) {
        // qDebug() << "No Serial Ports Found";
        return;
    }
    this->SerialPortModel->clear();
    for (const auto &SerialPort : SerialPorts) {
        auto *NewItem = new QStandardItem(SerialPort.portName());
        // qDebug() << SerialPort.portName();
        this->SerialPortModel->appendRow(NewItem);
    }
    ui->SerialPortList->setModel(this->SerialPortModel);
}

/**
 * insert all baud rates in BaudRateList to BaudRateModel
 * BaudRate:9600,19200,38400,57600,115200
 * default at Baud9600
 */
void MainWindow::initBaudRate() {
    for (const auto &BaudRate : this->BaudRateList) {
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
    connect(ui->RefreshSerialListButton, &QPushButton::clicked, this, &MainWindow::getAvaliableSerialPorts);
    connect(ui->EstablishConnectionButton, &QPushButton::clicked, this, &MainWindow::createConnection);
    connect(ui->TerminateConnectionButton, &QPushButton::clicked, this, &MainWindow::terminateConnection);
    connect(ui->SendDataButton, &QPushButton::clicked, this, &MainWindow::sendText);
    connect(ui->ClearButton, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(ui->SendFileButton, &QPushButton::clicked, this, &MainWindow::sendFile);
    /*because read data is called every 1 second,so UihandledSerialConnection must be nullptr by
     * default instead of being undefined,if not,the program crashes 1s after it starts.
     */
    connect(ui->ReceiveAsTextButton, &QPushButton::clicked, this, &MainWindow::receiveText);
    connect(ui->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveFile);
    // connect(UiHandledSerialConnection, &SerialConnection::dataWritten, this, &MainWindow::dataWrittenHint);
}

/**
 * get selected serial port name in SerialList and create a connection with it's name
 * get selected baud rate in BaudRateList and use it as the baudrate of the connection,default at Baud9600
 */

void MainWindow::createConnection() {
    QString SerialPortName = ui->SerialPortList->currentText();
    if (this->UiHandledSerialConnection != nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Already Opened");
        return;
    }
    QSerialPort::BaudRate CurrentBaudRate = static_cast<QSerialPort::BaudRate>(ui->BaudRateList->currentText().toInt());
    try {
        this->UiHandledSerialConnection = new SerialConnection(SerialPortName, CurrentBaudRate);
    } catch (std::runtime_error &e) {
        QMessageBox::warning(this, "串口打开失败", e.what());
    }
}

/**
 * TerminalConnection when StopConnectionButton is clicked by calling SerialConnection::CloseConnection()
 * Error Code SerialConnectionState::LastSerialConnectionNotCompleted is used to
 * show warning MessageBox when close operation failed
 */

void MainWindow::terminateConnection() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto State = this->UiHandledSerialConnection->closeConnection();
    if (State == SerialConnectionState::LastSerialOperationNotCompleted) {
        /*
         *Possible State:SerialConnectionState::LastSerialOperationNotCompleted
         */
        QMessageBox::warning(this, "Error", "Last Serial Operation Not Completed");
    } else {
        this->getAvaliableSerialPorts();
        this->UiHandledSerialConnection = nullptr;
    }
}

/**
 * send text in TextEdit widget ( DataToSend ) by calling SerialConnection::WriteString()
 * Possible Error Code : SerialConnection::SerialPortNotOpened
 */

void MainWindow::sendText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    auto State = SerialConnectionState::NoError;
    State = this->UiHandledSerialConnection->writeData(ui->DataToSendTextBox->toPlainText().toUtf8());
    /*
     * Possible State:SerialConnectionState::SerialPortNotOpened
     */
    if (State == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
}

/**
 * send file by calling SerialConnection::writeData()
 * Possible Error Code : SerialConnection::SerialPortNotOpened
 */
void MainWindow::sendFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
    if (fileName.isEmpty()) {
        return;
    }
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto State = this->UiHandledSerialConnection->writeFile(fileName);
    if (State == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
}

/**
 * receive data from another device by calling SerialConnection::ReadString()
 * Possible Error Code : SerialConnection::NothingToBeReaded
 */

void MainWindow::receiveText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiHandledSerialConnection != nullptr);
    /*if UiHandledSerialConnection is not given nullptr before constructed , the program crashes here*/
    auto Data = this->UiHandledSerialConnection->readData();
    if (Data.isEmpty()) {
        return;
    }
    ui->DataReceivedTextBox->moveCursor(QTextCursor::End);
    ui->DataReceivedTextBox->insertPlainText(QString::fromUtf8(Data));
}

/**
 * clear all contents in send and received textbox
 */

void MainWindow::receiveFile() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    QString FileName = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath());
    if (FileName.isEmpty()) {
        return;
    }
    QByteArray data = this->UiHandledSerialConnection->readData();
    QFile file(FileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "File Open Failed");
        return;
    }
    QDataStream out(&file);
    out.writeRawData(data.data(), data.size());
    file.close();
}

void MainWindow::clearAll() {
    ui->DataToSendTextBox->clear();
    ui->DataReceivedTextBox->clear();
}

void MainWindow::dataWrittenHint() {
    QMessageBox::information(this, "Data Written", "Data Written Successfully");
}
