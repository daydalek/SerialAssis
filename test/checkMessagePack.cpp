#include "../src/include/DataParser.h"
#include "../src/include/DataWrapper.h"
#include "../src/include/LocalEnumType.h"
#include <QtTest>
class checkMessagePack : public QObject {
    Q_OBJECT

  private slots:
    void test_dataWrapperAndDataParser();
};

void checkMessagePack::test_dataWrapperAndDataParser() {
    DataWrapper wrapper;
    DataParser parser;
    QByteArray data = "Hello, World!";
    QByteArray wrappedData = wrapper.wrapData(data, DataType::Text, true);
    QCOMPARE(parser.getType(wrappedData), DataType::Text);
    QCOMPARE(parser.getData(wrappedData), data);
    // QCOMPARE(parser.validateMd5(wrappedData), Md5Validation::Passed);
}

QTEST_APPLESS_MAIN(checkMessagePack)
#include "checkMessagePack.moc"