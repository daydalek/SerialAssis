cmake_minimum_required(VERSION 3.28)
project(SerialAssis)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.6.3\\mingw_64\\lib\\cmake;C:\\Users\\daydalek\\.vcpkg-clion\\vcpkg\\packages")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        Test
        REQUIRED)

find_package(Catch2)
find_package(simdjson)

file(GLOB_RECURSE RELEASE_SOURCES
        "src/*.cpp"
        "src/include/*.h"
)

#file(GLOB_RECURSE TEST_SERIAL_CONNECTION_SOURCES
#        "test/SerialConnectionTest.cpp"
#        "src/SerialConnection.cpp"
#        "src/include/SerialConnection.h"
#)
#
add_executable(SerialAssis WIN32 ${RELEASE_SOURCES})
#add_executable(TestSerialConnection ${TEST_SERIAL_CONNECTION_SOURCES})

target_link_libraries(
        SerialAssis
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::SerialPort
)

#target_link_libraries(
#        TestSerialConnection
#        Qt::Core
#        Qt::SerialPort
#        Catch2::Catch2
#)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
