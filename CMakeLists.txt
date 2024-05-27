cmake_minimum_required(VERSION 3.28)
project(SerialAssis)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_PREFIX_PATH "C:/Users/daydalek/.vcpkg-clion/vcpkg/packages/*x64-mingw-dynamic/")
set(Catch2_DIR "C:/Users/daydalek/.vcpkg-clion/vcpkg/packages/catch2_x64-mingw-dynamic/share/Catch2")
set(simdjson_DIR "C:/Users/daydalek/.vcpkg-clion/vcpkg/packages/simdjson_x64-mingw-dynamic/share/simdjson")
set(msgpack-cxx_DIR "C:/Users/daydalek/.vcpkg-clion/vcpkg/packages/msgpack_x64-mingw-dynamic/share/msgpack-cxx")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        SerialPort
        Test
        REQUIRED)

find_package(Catch2 CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)

file(GLOB_RECURSE RELEASE_SOURCES
        "src/*.cpp"
        "src/include/*.h"
)

#file(GLOB_RECURSE TEST_SERIAL_CONNECTION_SOURCES
#        "testSerialConnectionTest.cpp"
#        "srcSerialConnection.cpp"
#        "srcinclude/SerialConnection.h"
#)
#
add_executable(SerialAssis WIN32 ${RELEASE_SOURCES})
#add_executable(TestSerialConnection ${TEST_SERIAL_CONNECTION_SOURCES})
enable_testing()
add_executable(CheckMessagePack
        test/checkMessagePack.cpp
        src/include/DataParser.h
        src/include/DataWrapper.h
        src/DataParser.cpp
        src/DataWrapper.cpp
        src/include/LocalEnumType.h
)

target_link_libraries(
        SerialAssis
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::SerialPort
        msgpack-cxx
)

target_link_libraries(
        CheckMessagePack
        Qt::Core
        Qt::Test
)

add_test(CheckMessagePack CheckMessagePack)
#target_link_libraries(SerialAssis PRIVATE simdjson::simdjson)

#target_link_libraries(
#        TestSerialConnection
#        Qt::Core
#        Qt::SerialPort
#        Catch2::Catch2
#)

