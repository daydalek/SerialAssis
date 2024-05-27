#include "../src/include/DataParser.h"
#include "../src/include/DataWrapper.h"
#include "../src/include/LocalEnumType.h"
#include <QtTest>
class unit_DataParser_DataWrapper : public QObject {
    Q_OBJECT

  private slots:
    void test_dataWrapperAndDataParser();
};

void unit_DataParser_DataWrapper::test_dataWrapperAndDataParser() {
    DataWrapper wrapper;
    DataParser parser;
    QByteArray data = "english,514!";
    QByteArray wrappedData = wrapper.wrapData(data, DataType::Text, true);
    qDebug()<< wrappedData.data();
    QCOMPARE(parser.getType(wrappedData), DataType::Text);
    qDebug()<< parser.getData(wrappedData);
    QCOMPARE(parser.getData(wrappedData), data);
    QCOMPARE(parser.validateMd5(wrappedData), Md5Validation::Passed);
}

QTEST_APPLESS_MAIN(unit_DataParser_DataWrapper)
#include "unit_DataParser_DataWrapper.moc"