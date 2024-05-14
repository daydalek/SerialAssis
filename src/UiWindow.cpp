//
// Created by daydalek on 24-5-14.
//

#include "../include/UiWindow.h"

SerialMonitor::SerialMonitor(QWidget *parent) : QWidget(parent) {
    // 初始化成员变量
    MainLayout = new QHBoxLayout(this);
    LeftLayout = new QVBoxLayout();
    MiddleLayout = new QVBoxLayout();
    RightLayout = new QVBoxLayout();
    DataToSendTextBox = new QTextEdit();
    DataReceivedTextBox = new QTextEdit();
    HexDisplayCheckBoxReceive = new QCheckBox("16进制");
    HexDisplayCheckBoxSend = new QCheckBox("16进制");
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
    ReceiveControlLayout->addWidget(HexDisplayCheckBoxReceive);
    ReceiveControlLayout->addWidget(ClearButton);

    LeftLayout->addWidget(new QLabel("接收区"));
    LeftLayout->addWidget(DataReceivedTextBox);
    LeftLayout->addLayout(ReceiveControlLayout);

    // 中间布局：发送区
    SendControlLayout->addWidget(HexDisplayCheckBoxSend);
    SendControlLayout->addWidget(SendDataButton);

    MiddleLayout->addWidget(new QLabel("发送区"));
    MiddleLayout->addWidget(DataToSendTextBox);
    MiddleLayout->addLayout(SendControlLayout);

    // 右侧布局：串口设置和操作
    RightLayout->addWidget(RefreshSerialListButton);
    RightLayout->addWidget(new QLabel("波特率"));
    RightLayout->addWidget(BaudRateList);
    RightLayout->addWidget(new QLabel("串口选择"));
    RightLayout->addWidget(SerialPortList);
    RightLayout->addWidget(EstablishConnectionButton);
    RightLayout->addWidget(TerminateConnectionButton);
    RightLayout->addWidget(DateTimeLabel);

    // 将三个布局添加到主布局中
    MainLayout->addLayout(LeftLayout);
    MainLayout->addLayout(MiddleLayout);
    MainLayout->addLayout(RightLayout);

    // 设置窗口标题和大小
    setWindowTitle("串口助手");

    // 设置定时器更新时间
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [this]() { DateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")); });
        timer->start(1000);
    show();
}

