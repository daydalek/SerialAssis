/**
* @file     MainWindow.cpp
* @brief    Implementations of MainWindow.h
* @note     This file is about the integration of UI and SerialConnection,
            which means when you click a button,the SerialConnection or the UI
            will react to the event.
*/

#include "include/MainWindow.h"
#include "include/SerialConnection.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), UI(new SerialMonitor) {
    loadConfig("config.json");
    if (this->DefaultConfig.SendOnBoot) {
        sendonBoot();
    }
    setCentralWidget(UI);
    connectSlots();
    this->SerialPortModel = new QStandardItemModel(this);
    this->BaudRateModel = new QStandardItemModel(this);
    initBaudRate();
    getAvaliableSerialPorts();
}

MainWindow::~MainWindow() {
    delete UI;
    delete UiHandledSerialConnection;
}

void MainWindow::loadConfig(const QString &ConfigPath) {
    this->DefaultConfig = ConfigParser::parseConfig(ConfigPath);
}

/**
 * @note    get all available serial ports and insert them into SerialPortModel.
 *          This function is powered by QSerialPortInfo::availablePorts().
 *          It's called when RefreshSerialListButton is clicked
 */

void MainWindow::getAvaliableSerialPorts() {
    if (this->UiHandledSerialConnection != nullptr) {
        return;
    }
    QList<QSerialPortInfo> SerialPorts = QSerialPortInfo::availablePorts();
    if (SerialPorts.empty()) {
        return;
    }
    this->SerialPortModel->clear();
    for (const auto &SerialPort : SerialPorts) {
        auto *NewItem = new QStandardItem(SerialPort.portName());
        this->SerialPortModel->appendRow(NewItem);
    }
    UI->SerialPortList->setModel(this->SerialPortModel);
}

/**
 * @note    insert all baud rates in BaudRateList to BaudRateModel
 *          BaudRate:9600,19200,38400,57600,115200
 *          default at Baud9600
 */

void MainWindow::initBaudRate() {
    for (const auto &BaudRate : this->BaudRateList) {
        auto *NewItem = new QStandardItem(QString::number(BaudRate));
        NewItem->setData(QString::number(BaudRate));
        this->BaudRateModel->appendRow(NewItem);
    }
    UI->BaudRateList->setModel(this->BaudRateModel);
}

/**
 * @note    connect all buttons,lists and text widgets to slot functions
 */

void MainWindow::connectSlots() {
    connect(UI->EstablishConnectionButton, &QPushButton::clicked, this,
            &MainWindow::createConnection);
    connect(UI->TerminateConnectionButton, &QPushButton::clicked, this,
            &MainWindow::terminateConnection);
    connect(UI->RefreshSerialListButton, &QPushButton::clicked, this,
            &MainWindow::getAvaliableSerialPorts);
    connect(UI->ReceiveAsTextButton, &QPushButton::clicked, this,
            &MainWindow::receiveText);
    connect(UI->ReceiveAsFileButton, &QPushButton::clicked, this,
            &MainWindow::receiveFile);
    connect(UI->SendDataButton, &QPushButton::clicked, this,
            &MainWindow::sendText);
    connect(UI->SendFileButton, &QPushButton::clicked, this,
            &MainWindow::sendFile);
    connect(UI->ClearButton, &QPushButton::clicked, this,
            &MainWindow::clearAll);
    if (this->DefaultConfig.ReceiveOnBoot) {
        this->UiHandledSerialConnection=new SerialConnection(this->DefaultConfig.DefaultReceiveSerialPort);
        connect(UiHandledSerialConnection, &SerialConnection::dataReadyToRead,
                this, &MainWindow::receiveOnBoot);
    }
    // connect(UiHandledSerialConnection, &SerialConnection::dataWritten, this,
    // &MainWindow::showDataWrittenWindow);
}

/**
 * @note    get selected serial port name in SerialList and create a connection
 * with it's name get selected baud rate in BaudRateList and use it as the
 * baudrate of the connection, default at Baud9600
 */

void MainWindow::createConnection() {
    QString SerialPortName = UI->SerialPortList->currentText();
    if (this->UiHandledSerialConnection != nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Already Opened");
        return;
    }
    QSerialPort::BaudRate CurrentBaudRate = static_cast<QSerialPort::BaudRate>(
        UI->BaudRateList->currentText().toInt());
    try {
        this->UiHandledSerialConnection =
            new SerialConnection(SerialPortName, CurrentBaudRate);
    } catch (std::runtime_error &e) {
        QMessageBox::warning(this, "串口打开失败", e.what());
    }
}

/**
 * @note    TerminalConnection when StopConnectionButton is clicked by calling
 * SerialConnection::CloseConnection() Error Code
 * SerialConnectionState::LastSerialConnectionNotCompleted is used to show
 * warning MessageBox.
 */

void MainWindow::terminateConnection() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto ConnectionState = this->UiHandledSerialConnection->closeConnection();
    if (ConnectionState ==
        SerialConnectionState::LastSerialOperationNotCompleted) {
        QMessageBox::warning(this, "Error",
                             "Last Serial Operation Not Completed");
    } else {
        this->getAvaliableSerialPorts();
        this->UiHandledSerialConnection = nullptr;
    }
}

/**
 * @note    send text in TextEdit widget ( DataToSend ) by calling
 * SerialConnection::writeData() possible Error Code :
 * SerialConnection::SerialPortNotOpened
 */

void MainWindow::sendText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    SerialConnectionState ConnectionState =
        this->UiHandledSerialConnection->writeData(
            UI->DataToSendTextBox->toPlainText().toUtf8());
    if (ConnectionState == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
    this->CompletionBoxShown = false;
    connect(this->UiHandledSerialConnection,
            &SerialConnection::dataFullyWritten, this,
            &MainWindow::showDataWrittenWindow);
}

/**
 * @note    send file by calling SerialConnection::writeData()
 *          possible Error code : SerialConnection::SerialPortNotOpened
 */

void MainWindow::sendFile() {
    QString FileName =
        QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
    if (FileName.isEmpty()) {
        return;
    }
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto ConnectionState = this->UiHandledSerialConnection->writeFile(FileName);
    if (ConnectionState == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
    this->CompletionBoxShown = false;
    connect(this->UiHandledSerialConnection,
            &SerialConnection::dataFullyWritten, this,
            &MainWindow::showDataWrittenWindow);
}

/**
 * @note    receive data from another device by calling
            SerialConnection::readData() called when ReceiveAsTextButton is clicked
 *          possible Error code : SerialConnection::NothingToBeReaded
 */

void MainWindow::receiveText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiHandledSerialConnection != nullptr);
    /*if UiHandledSerialConnection is not given nullptr before constructed , the
     * program crashes here*/
    auto TextDataFromSerialPort = this->UiHandledSerialConnection->readData();
    if (TextDataFromSerialPort.isEmpty()) {
        return;
    }
    UI->DataReceivedTextBox->moveCursor(QTextCursor::End);
    UI->DataReceivedTextBox->insertPlainText(
        QString::fromUtf8(TextDataFromSerialPort));
}

/**
* @note     receive data from another device by calling
            SerialConnection::readData called when ReceiveAsFileButton is
clicked
*/

void MainWindow::receiveFile() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    QString NameOfFileToSave =
        QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath());
    if (NameOfFileToSave.isEmpty()) {
        return;
    }
    QByteArray FileDataFromSerialPort =
        this->UiHandledSerialConnection->readData();
    QFile file(NameOfFileToSave);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "File Open Failed");
        return;
    }
    QDataStream out(&file);
    /*pay attention , FileDataFromSerialPort cast from long long to int*/
    out.writeRawData(FileDataFromSerialPort.data(),
                     static_cast<int>(FileDataFromSerialPort.size()));
    file.close();
}

/**
 * @note    clear all contents in send and received textbox
 */

void MainWindow::clearAll() {
    UI->DataToSendTextBox->clear();
    UI->DataReceivedTextBox->clear();
}

void MainWindow::showDataWrittenWindow() {
    if (this->CompletionBoxShown) {
        return;
    }
    this->CompletionBoxShown = true;
    QMessageBox::information(this, "Data Written", "Data Written Successfully");
}

void MainWindow::sendonBoot() {
    try {
        this->UiHandledSerialConnection =
            new SerialConnection(this->DefaultConfig.DefaultSendSerialPort);
    } catch (std::runtime_error) {
        QMessageBox::warning(this, "串口打开失败", "串口打开失败");
        exit(1);
    }
    QString FileName =
        QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
    if (FileName.isEmpty()) {
        exit(1);
    }
    auto ConnectionState = this->UiHandledSerialConnection->writeFile(FileName);
    if (ConnectionState == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        exit(1);
    }
}

void MainWindow::receiveOnBoot() {
    QString FileName = this->DefaultConfig.DefaultSaveFile;
    if (FileName.isEmpty()) {
        FileName = QFileDialog::getSaveFileName(this, "Save File",
                                                QDir::currentPath());
    }
    QFile file(FileName);
    QByteArray FileDataFromSerialPort =
        this->UiHandledSerialConnection->readData();
    file.write(FileDataFromSerialPort);
    disconnect(UiHandledSerialConnection, &SerialConnection::dataReadyToRead,
               this,&receiveOnBoot);
}
