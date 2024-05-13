/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *EstablishConnectionButton;
    QPushButton *StopConnectionButton;
    QLabel *SerialListLabel;
    QTextEdit *DataToSendTextBox;
    QLabel *SendReceiveLabel;
    QListView *FileList;
    QLabel *DropToSendLabel;
    QPushButton *SendDataButton;
    QPushButton *ReceiveDataButton;
    QListView *SerialList;
    QListView *BaudRateList;
    QPushButton *ChooseBaudRate;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        EstablishConnectionButton = new QPushButton(centralwidget);
        EstablishConnectionButton->setObjectName("EstablishConnectionButton");
        EstablishConnectionButton->setGeometry(QRect(60, 190, 71, 18));
        StopConnectionButton = new QPushButton(centralwidget);
        StopConnectionButton->setObjectName("StopConnectionButton");
        StopConnectionButton->setGeometry(QRect(60, 220, 71, 18));
        SerialListLabel = new QLabel(centralwidget);
        SerialListLabel->setObjectName("SerialListLabel");
        SerialListLabel->setGeometry(QRect(50, 20, 71, 20));
        DataToSendTextBox = new QTextEdit(centralwidget);
        DataToSendTextBox->setObjectName("DataToSendTextBox");
        DataToSendTextBox->setGeometry(QRect(160, 50, 141, 121));
        SendReceiveLabel = new QLabel(centralwidget);
        SendReceiveLabel->setObjectName("SendReceiveLabel");
        SendReceiveLabel->setGeometry(QRect(180, 20, 91, 21));
        FileList = new QListView(centralwidget);
        FileList->setObjectName("FileList");
        FileList->setGeometry(QRect(340, 50, 141, 121));
        DropToSendLabel = new QLabel(centralwidget);
        DropToSendLabel->setObjectName("DropToSendLabel");
        DropToSendLabel->setGeometry(QRect(360, 20, 111, 21));
        SendDataButton = new QPushButton(centralwidget);
        SendDataButton->setObjectName("SendDataButton");
        SendDataButton->setGeometry(QRect(160, 190, 141, 18));
        ReceiveDataButton = new QPushButton(centralwidget);
        ReceiveDataButton->setObjectName("ReceiveDataButton");
        ReceiveDataButton->setGeometry(QRect(160, 220, 141, 18));
        SerialList = new QListView(centralwidget);
        SerialList->setObjectName("SerialList");
        SerialList->setGeometry(QRect(60, 50, 73, 121));
        BaudRateList = new QListView(centralwidget);
        BaudRateList->setObjectName("BaudRateList");
        BaudRateList->setGeometry(QRect(340, 190, 141, 51));
        ChooseBaudRate = new QPushButton(centralwidget);
        ChooseBaudRate->setObjectName("ChooseBaudRate");
        ChooseBaudRate->setGeometry(QRect(370, 260, 75, 24));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        EstablishConnectionButton->setText(QCoreApplication::translate("MainWindow", "\345\273\272\347\253\213\350\277\236\346\216\245", nullptr));
        StopConnectionButton->setText(QCoreApplication::translate("MainWindow", "\346\226\255\345\274\200\350\277\236\346\216\245", nullptr));
        SerialListLabel->setText(QCoreApplication::translate("MainWindow", "       \344\270\262\345\217\243\345\210\227\350\241\250", nullptr));
        SendReceiveLabel->setText(QCoreApplication::translate("MainWindow", "      \345\217\221\351\200\201/\346\216\245\346\224\266\346\225\260\346\215\256", nullptr));
        DropToSendLabel->setText(QCoreApplication::translate("MainWindow", "   \346\213\226\346\213\275\345\210\260\346\255\244\345\244\204\344\273\245\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
        SendDataButton->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\225\260\346\215\256", nullptr));
        ReceiveDataButton->setText(QCoreApplication::translate("MainWindow", "\346\216\245\346\224\266\346\225\260\346\215\256", nullptr));
        ChooseBaudRate->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\346\263\242\347\211\271\347\216\207", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\344\274\240\350\276\223", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UIWINDOW_H
