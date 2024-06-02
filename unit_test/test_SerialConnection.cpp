#include "../src/include/SerialConnection.h"
#include <QSignalSpy>
#include <QtTest>

class TestSerialConnection : public QObject {
    Q_OBJECT

  private slots:
    // void initTestCase();
    // void cleanupTestCase();

    void testWriteFileExisting();
    void testWriteFileNonExisting();
    void testWriteReadData();
    void testCloseConnection();
    void testOnBytesWritten();

  private:
    SerialConnection *reader{};
    SerialConnection *writer{};
};

// void TestSerialConnection::initTestCase() {
// }

// void TestSerialConnection::cleanupTestCase() {
// }

void TestSerialConnection::testWriteFileNonExisting() {
    reader = new SerialConnection("COM1", QSerialPort::Baud9600);
    writer = new SerialConnection("COM2", QSerialPort::Baud9600);
    QString fileName("test.txt");
    QCOMPARE(reader->writeFile(fileName), SerialConnectionState::FileNotFound);
    delete reader;
    delete writer;
}

void TestSerialConnection::testWriteFileExisting() {
    reader = new SerialConnection("COM1", QSerialPort::Baud9600);
    writer = new SerialConnection("COM2", QSerialPort::Baud9600);
    QString fileName("test.txt");
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write("Test data");
    file.close();
    QCOMPARE(reader->writeFile(fileName), SerialConnectionState::NoError);
    file.remove();
    delete reader;
    delete writer;
}

void TestSerialConnection::testWriteReadData() {
    reader = new SerialConnection("COM1", QSerialPort::Baud9600);
    writer = new SerialConnection("COM2", QSerialPort::Baud9600);
    QByteArray testdata("Test data");
    auto state = writer->writeData(testdata);
    QCOMPARE(state, SerialConnectionState::NoError);
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(1000);
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
    QCOMPARE(reader->readData(), testdata);
    delete reader;
    delete writer;
}

void TestSerialConnection::testCloseConnection() {
    reader = new SerialConnection("COM1", QSerialPort::Baud9600);
    writer = new SerialConnection("COM2", QSerialPort::Baud9600);
    QCOMPARE(reader->closeConnection(), SerialConnectionState::NoError);
    delete reader;
    delete writer;
}

void TestSerialConnection::testOnBytesWritten() {
    reader = new SerialConnection("COM1", QSerialPort::Baud9600);
    writer = new SerialConnection("COM2", QSerialPort::Baud9600);
    QSignalSpy spy(writer, SIGNAL(dataFullyWritten()));
    QByteArray data("Test data");
    auto state = writer->writeData(data);
    QCOMPARE(state, SerialConnectionState::NoError);
    QVERIFY(spy.wait());
    // QCOMPARE(spy.count(), 1);
    delete reader;
    delete writer;
}

QTEST_MAIN(TestSerialConnection)
#include "test_SerialConnection.moc"