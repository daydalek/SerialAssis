//
// Created by daydalek on 24-5-12.
//

#include "../include/MainWindow.h"
#include "../include/SerialConnection.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
// connect(ui->EstablishConnectionButton, &QPushButton::clicked, this,SerialConnection::SerialConnection("COM1", QSerialPort::Baud9600));
}

MainWindow::~MainWindow() {
    delete ui;
}