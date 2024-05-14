#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class SerialMonitor : public QWidget {
  public:
    SerialMonitor(QWidget *parent = nullptr);

  public:
    QHBoxLayout     *MainLayout;
    QVBoxLayout     *LeftLayout;
    QVBoxLayout     *MiddleLayout;
    QVBoxLayout     *RightLayout;
    QHBoxLayout     *ReceiveControlLayout;
    QHBoxLayout     *SendControlLayout;
    QTextEdit       *DataToSendTextBox;
    QTextEdit       *DataReceivedTextBox;
    QCheckBox       *HexDisplayCheckBoxReceive;
    QCheckBox       *HexDisplayCheckBoxSend;
    QPushButton     *ClearButton;
    QPushButton     *SendDataButton;
    QPushButton     *RefreshSerialListButton;
    QPushButton     *EstablishConnectionButton;
    QPushButton     *TerminateConnectionButton;
    QLabel          *DateTimeLabel;
    QComboBox       *BaudRateList;
    QComboBox       *SerialPortList;
};

#endif // UIWINDOW_H
