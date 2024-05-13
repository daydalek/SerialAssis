## SerialAssistant

### 1. Introduction

This is a simple serial assistant tool for serial port communication. It is developed by Qt6 under LGPL-3.0 license.
This program is dynamically linked to Qt dlls.Checkout 
[LICENSE](LICENSE) for details about the LGPL-3.0 license.

### 2. Code Structure

- CMakeLists.txt: CMake configuration file
- README.md: This file
- src: Source code directory
  - main.cpp: Main function
  - mainwindow.cpp: Main window implementation
  - SerialConnection.cpp: Serial connection implementation
  - mainwindow.ui: QT designer ui file
- include: Header file directory
  - mainwindow.h: Main window header file
  - SerialConnection.h: Serial connection header file
  - UiWindow.h: UI window header file,exported by Qt uic

  