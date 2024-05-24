/**
* @file     MainWindow.cpp
* @brief    Implementations of MainWindow.h
* @note     This file is about the integration of UI and SerialConnection,
            which means when you click a button,the SerialConnection or the UI
            will react to the event.
*/
#include "include/MainWindow.h"
#include "include/SerialConnection.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QStandardItemModel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), UI(new SerialMonitor) {
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
        // std::cout << QString::number(BaudRate).toStdString() << std::endl;
        this->BaudRateModel->appendRow(NewItem);
        // std::cout << NewItem->data().toString().toStdString() << std::endl;
    }
    UI->BaudRateList->setModel(this->BaudRateModel);
}

/**
 * @note    connect all buttons,lists and text widgets to slot functions
 */

void MainWindow::connectSlots() {
    connect(UI->RefreshSerialListButton, &QPushButton::clicked, this, &MainWindow::getAvaliableSerialPorts);
    connect(UI->EstablishConnectionButton, &QPushButton::clicked, this, &MainWindow::createConnection);
    connect(UI->TerminateConnectionButton, &QPushButton::clicked, this, &MainWindow::terminateConnection);
    connect(UI->SendDataButton, &QPushButton::clicked, this, &MainWindow::sendText);
    connect(UI->ClearButton, &QPushButton::clicked, this, &MainWindow::clearAll);
    connect(UI->SendFileButton, &QPushButton::clicked, this, &MainWindow::sendFile);
    /** because read data is called every 1 second,so UihandledSerialConnection must be nullptr by
     *  default instead of being undefined,if not,the program crashes 1s after it starts.
     */
    connect(UI->ReceiveAsTextButton, &QPushButton::clicked, this, &MainWindow::receiveText);
    connect(UI->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveFile);
    // connect(UiHandledSerialConnection, &SerialConnection::dataWritten, this, &MainWindow::dataWrittenHint);
}

/**
 * @note    get selected serial port name in SerialList and create a connection with it's name
 *          get selected baud rate in BaudRateList and use it as the baudrate of the connection,default at Baud9600
 */

void MainWindow::createConnection() {
    QString SerialPortName = UI->SerialPortList->currentText();
    if (this->UiHandledSerialConnection != nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Already Opened");
        return;
    }
    QSerialPort::BaudRate CurrentBaudRate = static_cast<QSerialPort::BaudRate>(UI->BaudRateList->currentText().toInt());
    try {
        this->UiHandledSerialConnection = new SerialConnection(SerialPortName, CurrentBaudRate);
    } catch (std::runtime_error &e) {
        QMessageBox::warning(this, "串口打开失败", e.what());
    }
}

/**
 * @note    TerminalConnection when StopConnectionButton is clicked by calling SerialConnection::CloseConnection()
 *          Error Code SerialConnectionState::LastSerialConnectionNotCompleted is used to
 *          show warning MessageBox.
 */

void MainWindow::terminateConnection() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto ConnectionState = this->UiHandledSerialConnection->closeConnection();
    if (ConnectionState == SerialConnectionState::LastSerialOperationNotCompleted) {
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
 * @note    send text in TextEdit widget ( DataToSend ) by calling SerialConnection::writeData()
 *          Possible Error Code : SerialConnection::SerialPortNotOpened
 */

void MainWindow::sendText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    auto ConnectionState = SerialConnectionState::NoError;
    ConnectionState = this->UiHandledSerialConnection->writeData(UI->DataToSendTextBox->toPlainText().toUtf8());
    /*
     * Possible State:SerialConnectionState::SerialPortNotOpened
     */
    if (ConnectionState == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
}

/**
 * @note    send file by calling SerialConnection::writeData()
 *          possible Error code : SerialConnection::SerialPortNotOpened
 */
void MainWindow::sendFile() {
    QString FileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
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
}

/**
 * @note    receive data from another device by calling SerialConnection::readData()
            called when ReceiveAsTextButton is clicked
 *          possible Error code : SerialConnection::NothingToBeReaded
 */

void MainWindow::receiveText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiHandledSerialConnection != nullptr);
    /*if UiHandledSerialConnection is not given nullptr before constructed , the program crashes here*/
    auto TextDataFromSerialPort = this->UiHandledSerialConnection->readData();
    if (TextDataFromSerialPort.isEmpty()) {
        return;
    }
    UI->DataReceivedTextBox->moveCursor(QTextCursor::End);
    UI->DataReceivedTextBox->insertPlainText(QString::fromUtf8(TextDataFromSerialPort));
}

/**
* @note     receive data from another device by calling SerialConnection::readData
            called when ReceiveAsFileButton is clicked
*/

void MainWindow::receiveFile() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    QString NameOfFileToSave = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath());
    if (NameOfFileToSave.isEmpty()) {
        return;
    }
    QByteArray FileDataFromSerialPort = this->UiHandledSerialConnection->readData();
    QFile file(NameOfFileToSave);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "File Open Failed");
        return;
    }
    QDataStream out(&file);
    out.writeRawData(FileDataFromSerialPort.data(), FileDataFromSerialPort.size());
    file.close();
}

/**
 * @note    clear all contents in send and received textbox
 */

void MainWindow::clearAll() {
    UI->DataToSendTextBox->clear();
    UI->DataReceivedTextBox->clear();
}

void MainWindow::dataWrittenHint() {
    QMessageBox::information(this, "Data Written", "Data Written Successfully");
}
