//
// Created by daydalek on 24-5-14.
//

#include "../include/SerialMonitor.h"

SerialMonitor::SerialMonitor(QWidget *parent) : QWidget(parent) {
    MainLayout = new QHBoxLayout(this);
    LeftLayout = new QVBoxLayout();
    MiddleLayout = new QVBoxLayout();
    RightLayout = new QVBoxLayout();
    DataToSendTextBox = new QTextEdit();
    DataReceivedTextBox = new QTextEdit();
    ClearButton = new QPushButton("清除");
    SendDataButton = new QPushButton("发送");
    RefreshSerialListButton = new QPushButton("刷新");
    BaudRateList = new QComboBox();
    SerialPortList = new QComboBox();
    EstablishConnectionButton = new QPushButton("打开串口");
    TerminateConnectionButton = new QPushButton("关闭串口");
    DateTimeLabel = new QLabel();
    ReceiveControlLayout = new QHBoxLayout();
    SendControlLayout = new QHBoxLayout();

    // 左侧布局：接收区
    ReceiveControlLayout->addWidget(ClearButton);

    LeftLayout->addWidget(new QLabel("接收区"));
    LeftLayout->addWidget(DataReceivedTextBox);
    LeftLayout->addLayout(ReceiveControlLayout);

    SendControlLayout->addWidget(SendDataButton);

    MiddleLayout->addWidget(new QLabel("发送区"));
    MiddleLayout->addWidget(DataToSendTextBox);
    MiddleLayout->addLayout(SendControlLayout);

    RightLayout->addWidget(RefreshSerialListButton);
    RightLayout->addWidget(new QLabel("波特率"));
    RightLayout->addWidget(BaudRateList);
    RightLayout->addWidget(new QLabel("串口选择"));
    RightLayout->addWidget(SerialPortList);
    RightLayout->addWidget(EstablishConnectionButton);
    RightLayout->addWidget(TerminateConnectionButton);
    RightLayout->addWidget(DateTimeLabel);

    MainLayout->addLayout(LeftLayout);
    MainLayout->addLayout(MiddleLayout);
    MainLayout->addLayout(RightLayout);

    setWindowTitle("串口助手");

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [this]() { DateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")); });
        timer->start(1000);
}

