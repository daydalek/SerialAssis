#include "../src/include/ConfigParser.h"
#include <QtTest>

class TestConfigParser : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();

    void parseConfig_HappyPath();
    void parseConfig_FileDoesNotExist();
    void parseConfig_InvalidJson();
    void parseConfig_SendAndReceiveOnBoot();
};

void TestConfigParser::initTestCase() {
}

void TestConfigParser::cleanupTestCase() {
    remove("invalid.json");
    remove("config.json");
    remove("something.json");
}

void TestConfigParser::parseConfig_HappyPath() {
    QFile file("config.json");
    file.open(QIODevice::WriteOnly);
    file.write("{\"SendOnBoot\":true,\"ReceiveOnBoot\":false,\"DefaultSaveFile\":\"default_save_file\","
               "\"DefaultSendSerialPort\":\"default_send_serial_port\",\"DefaultReceiveSerialPort\":\"default_receive_"
               "serial_port\"}");
    file.close();
    ConfigParser parser;
    ConfigStruct config = parser.parseConfig("config.json");

    QCOMPARE(config.SendOnBoot, true);
    QCOMPARE(config.ReceiveOnBoot, false);
    QCOMPARE(config.DefaultSaveFile, QString("default_save_file"));
    QCOMPARE(config.DefaultSendSerialPort, QString("default_send_serial_port"));
    QCOMPARE(config.DefaultReceiveSerialPort, QString("default_receive_serial_port"));
}

void TestConfigParser::parseConfig_FileDoesNotExist() {
    ConfigParser parser;
    ConfigStruct config = parser.parseConfig("something.json");

    QCOMPARE(config.SendOnBoot, false);
    QCOMPARE(config.ReceiveOnBoot, false);
    QCOMPARE(config.DefaultSaveFile, QString(""));
    QCOMPARE(config.DefaultSendSerialPort, QString(""));
    QCOMPARE(config.DefaultReceiveSerialPort, QString(""));
}

void TestConfigParser::parseConfig_InvalidJson() {
    QFile file("invalid.json");
    file.open(QIODevice::WriteOnly);
    file.write("aaaabalhbalh");
    ConfigParser parser;
    ConfigStruct config = parser.parseConfig("invalid.json");

    QCOMPARE(config.SendOnBoot, false);
    QCOMPARE(config.ReceiveOnBoot, false);
    QCOMPARE(config.DefaultSaveFile, QString(""));
    QCOMPARE(config.DefaultSendSerialPort, QString(""));
    QCOMPARE(config.DefaultReceiveSerialPort, QString(""));
}

void TestConfigParser::parseConfig_SendAndReceiveOnBoot() {
    remove("config.json");
    QFile file("config.json");
    file.open(QIODevice::WriteOnly);
    file.write("{\"SendOnBoot\":true,\"ReceiveOnBoot\":true,\"DefaultSaveFile\":\"default_save_file\","
               "\"DefaultSendSerialPort\":\"default_send_serial_port\",\"DefaultReceiveSerialPort\":\"default_receive_"
               "serial_port\"}");
    file.close();
    ConfigParser parser;
    ConfigStruct config = parser.parseConfig("config.json");

    QCOMPARE(config.SendOnBoot, false);
    QCOMPARE(config.ReceiveOnBoot, true);
    QCOMPARE(config.DefaultSaveFile, QString("default_save_file"));
    QCOMPARE(config.DefaultSendSerialPort, QString("default_send_serial_port"));
    QCOMPARE(config.DefaultReceiveSerialPort, QString("default_receive_serial_port"));
}

QTEST_MAIN(TestConfigParser)
#include "test_ConfigParser.moc"