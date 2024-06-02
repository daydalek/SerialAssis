/**
* @file     MainWindow.cpp
* @brief    Implementations of MainWindow.h
* @note     This file is about the integration of UI and SerialConnection,
            which means when you click a button,the SerialConnection or the UI
            will react to the event.
*/

#include "include/MainWindow.h"
#include "include/SerialConnection.h"

/**
 * @brief   Constructor for MainWindow.
 * @param   parent The parent widget, default is nullptr.
 * @details Initializes the UI, loads configuration from a JSON file,
 *          sends data on boot if specified in the configuration, sets
 *          the central widget, connects slots, initializes the BaudRate
 *          model, and gets available serial ports.
 */

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), UI(new SerialMonitor) {
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

/**
 * @brief   Destructor for MainWindow.
 * @details Deletes the UI and handles the serial connection.
 */

MainWindow::~MainWindow() {
    delete UI;
    delete UiHandledSerialConnection;
}

/**
 * @brief   Loads the configuration from a specified JSON file.
 * @param   ConfigPath The path to the JSON configuration file.
 * @details Parses the configuration file and assigns the parsed
 *          configuration to DefaultConfig.
 */

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
 * @note    connect all buttons,lists and text widgets to slots
 */

void MainWindow::connectSlots() {
    connect(UI->EstablishConnectionButton, &QPushButton::clicked, this, &MainWindow::createConnection);
    connect(UI->TerminateConnectionButton, &QPushButton::clicked, this, &MainWindow::terminateConnection);
    connect(UI->RefreshSerialListButton, &QPushButton::clicked, this, &MainWindow::getAvaliableSerialPorts);
    connect(UI->ReceiveAsTextButton, &QPushButton::clicked, this, &MainWindow::receiveText);
    connect(UI->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveFile);
    connect(UI->SendDataButton, &QPushButton::clicked, this, &MainWindow::sendText);
    connect(UI->SendFileButton, &QPushButton::clicked, this, &MainWindow::sendFile);
    connect(UI->ClearButton, &QPushButton::clicked, this, &MainWindow::clearAll);
    if (this->DefaultConfig.ReceiveOnBoot) {
        this->UiHandledSerialConnection =
            new SerialConnection(this->DefaultConfig.DefaultReceiveSerialPort, QSerialPort::Baud115200);
        disconnect(UI->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveFile);
        connect(UI->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveOnBoot);
    }
    /*   connect(UiHandledSerialConnection, &SerialConnection::dataWritten, this,
      &MainWindow::showDataWrittenWindow); */
}

/**
 * @note    get selected serial port name in SerialList and create a connection
 *          with it's name get selected baud rate in BaudRateList and use it as the
 *          baudrate of the connection, default at Baud9600
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
 * @note    TerminalConnection when StopConnectionButton is clicked by calling
 *          SerialConnection::CloseConnection() Error Code
 *          SerialConnectionState::LastSerialConnectionNotCompleted is used to show
 *          warning MessageBox.
 */

void MainWindow::terminateConnection() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    auto ConnectionState = this->UiHandledSerialConnection->closeConnection();
    if (ConnectionState == SerialConnectionState::LastSerialOperationNotCompleted) {
        QMessageBox::warning(this, "Error", "Last Serial Operation Not Completed");
    } else {
        this->getAvaliableSerialPorts();
        this->UiHandledSerialConnection = nullptr;
    }
}

/**
 * @note    send text in TextEdit widget ( DataToSend ) by calling
 *          SerialConnection::writeData() possible Error Code :
 *          SerialConnection::SerialPortNotOpened
 */

void MainWindow::sendText() {
    if (this->UiHandledSerialConnection == nullptr) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        return;
    }
    // assert(this->UiSerialConnection != nullptr);
    SerialConnectionState ConnectionState =
        this->UiHandledSerialConnection->writeData(UI->DataToSendTextBox->toPlainText().toUtf8());
    if (ConnectionState == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
    }
    this->CompletionBoxShown = false;
    connect(this->UiHandledSerialConnection, &SerialConnection::dataFullyWritten, this,
            &MainWindow::showDataWrittenWindow);
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
    this->CompletionBoxShown = false;
    connect(this->UiHandledSerialConnection, &SerialConnection::dataFullyWritten, this,
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
    /*assert(this->UiHandledSerialConnection != nullptr); */
    /*if UiHandledSerialConnection is not given nullptr before constructed , the
     * program crashes here*/
    auto TextDataFromSerialPort = this->UiHandledSerialConnection->readData();
    if (TextDataFromSerialPort.isEmpty()) {
        return;
    }
    UI->DataReceivedTextBox->moveCursor(QTextCursor::End);
    UI->DataReceivedTextBox->insertPlainText(QString::fromUtf8(TextDataFromSerialPort));
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
    /*pay attention , FileDataFromSerialPort cast from long long to int*/
    out.writeRawData(FileDataFromSerialPort.data(), static_cast<int>(FileDataFromSerialPort.size()));
    file.close();
}

/**
 * @note    clear all contents in send and received textbox
 */

void MainWindow::clearAll() {
    UI->DataToSendTextBox->clear();
    UI->DataReceivedTextBox->clear();
}

/**
 * @brief   Displays a message box indicating that data has been written.
 * @details Checks if the completion box is already shown. If not, it sets
 *          the flag to true and shows an information message box with
 *          the "Data Written" message.
 */
void MainWindow::showDataWrittenWindow() {
    if (this->CompletionBoxShown) {
        return;
    }
    this->CompletionBoxShown = true;
    QMessageBox::information(this, "Data Written", "Data Written Successfully");
}

/**
 * @brief   Sends data on boot based on the default configuration.
 * @details Attempts to create a new serial connection using the default
 *          serial port and baud rate. If successful, prompts the user to
 *          select a file and writes its content to the serial port. If any
 *          error occurs (e.g., serial port not opened), shows a warning
 *          message and exits the application.
 */

void MainWindow::sendonBoot() {
    try {
        this->UiHandledSerialConnection =
            new SerialConnection(this->DefaultConfig.DefaultSendSerialPort, QSerialPort::Baud115200);
    } catch (std::runtime_error &e) {
        QMessageBox::warning(this, "串口打开失败", "串口打开失败");
        exit(1);
    }
    QString FileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath());
    if (FileName.isEmpty()) {
        exit(1);
    }
    auto ConnectionState = this->UiHandledSerialConnection->writeFile(FileName);
    if (ConnectionState == SerialConnectionState::SerialPortNotOpened) {
        QMessageBox::warning(this, "Error", "Serial Port Not Opened");
        exit(1);
    }
    // delete (UiHandledSerialConnection);
    // UiHandledSerialConnection = nullptr;
    connect(this->UiHandledSerialConnection, &SerialConnection::dataFullyWritten, this,
            &MainWindow::showDataWrittenWindow);
}

/**
 * @brief   Receives data on boot and saves it to a file.
 * @details Uses the default save file path from the configuration. If no
 *          default is set, prompts the user to select a save location.
 *          Reads data from the serial port and writes it to the file.
 */

void MainWindow::receiveOnBoot() {
    QString NameOfFileToSave = this->DefaultConfig.DefaultSaveFile;
    if (NameOfFileToSave.isEmpty()) {
        NameOfFileToSave = QFileDialog::getSaveFileName(this, "Save File", QDir::currentPath());
    }
    /*
    QMessageBox::information(this, "file to save", NameOfFileToSave);
    */
    QByteArray FileDataFromSerialPort = this->UiHandledSerialConnection->readData();
    QFile file(NameOfFileToSave);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "File Open Failed");
        return;
    }
    QDataStream out(&file);
    /*pay attention , FileDataFromSerialPort cast from long long to int*/
    out.writeRawData(FileDataFromSerialPort.data(), static_cast<int>(FileDataFromSerialPort.size()));
    file.close();
    disconnect(UI->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveOnBoot);
    connect(UI->ReceiveAsFileButton, &QPushButton::clicked, this, &MainWindow::receiveFile);
    /* delete UiHandledSerialConnection; */
    /* UiHandledSerialConnection = nullptr;*/
}
