## SerialAssistant
[中文README](README_ZH.md)

### 1. Introduction

This is a simple serial assistant tool for serial port communication. It's developed using Qt6 opensource under LGPL-3.0 license.
Checkout [LICENSE](LICENSE) for details about the LGPL-3.0 license.

### 2. Code Structure

- CMakeLists.txt: CMake configuration file
- README.md: This file
- README_ZH: Chinese version of README
- src: Source code directory
  - main.cpp: Main function
  - MainWindow.cpp: Main window implementation.
  - SerialConnection.cpp: Serial connection implementation.
  - SerialMonitor.cpp: SerialMonitor implementation.
- include: Header file directory
  - MainWindow.h: MainWindow header file ,  combining the UI and serial connection together.
  - SerialConnection.h: SerialConnection header file
  - SerialMonitor.h: SerialMonitor header file ,focus on parts that not related with the connection

### 3. Build
  You need to have Qt6 installed on your machine. You can download it from [Qt official website](https://www.qt.io/download).
  Simply modify the CMakeLists.txt file to point out your `CMAKE_PREFIX_PATH`.
  ![cmake_prefix_path](img/cmake_prefix_path.png)
  Then you can build the project using QT Creator or Any other IDE that supports CMake.
  
