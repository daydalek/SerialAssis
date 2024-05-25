## SerialAssistant
[English README](README.md)

### 介绍
这是一个简单的串口通信助手工具。使用Qt6开源库开发，遵循LGPL-3.0许可证。 查看LICENSE了解LGPL-3.0许可证的详细信息。

### 代码结构
- CMakeLists.txt: CMake配置文件
- README_ZH.md: 本文件
- README.md:英文版本的README
- src: 源代码目录
    - include: 头文件目录
      - MainWindow.h: MainWindow头文件，将UI和串口连接结合在一起
      - SerialConnection.h: SerialConnection头文件
      - SerialMonitor.h: SerialMonitor头文件，关注与连接无关的部分
    - main.cpp: 主函数
    - MainWindow.cpp: 主窗口实现
    - SerialConnection.cpp: 串口连接实现 
    - SerialMonitor.cpp: SerialMonitor窗口实现

可以在[doc](doc/html/index.html)查看Doxygen自动抽取的文档。

### 构建指南
你需要安装Qt6。你可以从Qt官方网站下载。[Qt official website](https://www.qt.io/download).

只需修改CMakeLists.txt文件，指出你的CMAKE_PREFIX_PATH 
![cmake_prefix_path](img/cmake_prefix_path.png)

然后你就可以使用QT Creator或任何其他支持CMake的IDE来构建项目。
