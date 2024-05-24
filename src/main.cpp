/**
 * @file    main.cpp
 * @brief   The entrance of the program
 */

#include "include/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    return QApplication::exec();
}
