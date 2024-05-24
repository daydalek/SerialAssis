/**
 * @file    SerialMonitor.h
 * @brief   This class is all about the user interface
 * @note    This class is nothing but some widgets,
 *          including buttons, textedit, combobox, etc.
 *          this class is NOT responsible for integration between UI
 *          and SerialConnection, which is MainWindow's job.
 */


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
    explicit SerialMonitor(QWidget *parent = nullptr);

  public:
    QHBoxLayout     *MainLayout;
    QVBoxLayout     *LeftLayout;
    QVBoxLayout     *MiddleLayout;
    QVBoxLayout     *RightLayout;
    QHBoxLayout     *ReceiveControlLayout;
    QHBoxLayout     *SendControlLayout;
    QTextEdit       *DataToSendTextBox;
    QTextEdit       *DataReceivedTextBox;
    QPushButton     *ClearButton;
    QPushButton     *SendDataButton;
    QPushButton     *SendFileButton;
    QPushButton     *RefreshSerialListButton;
    QPushButton     *EstablishConnectionButton;
    QPushButton     *TerminateConnectionButton;
    QPushButton     *ReceiveAsTextButton;
    QPushButton     *ReceiveAsFileButton;
    QComboBox       *BaudRateList;
    QComboBox       *SerialPortList;
    QLabel          *DateTimeLabel;
};

#endif // UIWINDOW_H
