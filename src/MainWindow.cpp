//
// Created by daydalek on 24-5-12.
//

#include "../include/MainWindow.h"
#include "../include/SerialConnection.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ConnectSlots();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::CreateConnection() {
}

void MainWindow::CloseConnection() {
}

void MainWindow::SendData() {
}

void MainWindow::ConnectSlots() {
    connect(ui->EstablishConnectionButton, &QPushButton::clicked, this, &MainWindow::CreateConnection);
    connect(ui->StopConnectionButton, &QPushButton::clicked, this, &MainWindow::CloseConnection);
    connect(ui->SendDataButton, &QPushButton::clicked, this, &MainWindow::SendData);
    connect(ui->ReceiveDataButton, &QPushButton::clicked, this, &MainWindow::ReceiveData);
}
