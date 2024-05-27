#include "../src/include/DataParser.h"
#include "../src/include/DataWrapper.h"
#include "../src/include/LocalEnumType.h"
#include "../src/include/SerialConnection.h"
#include "../src/include/SerialMonitor.h"
#include <QtTest>

class integrate_DataParser_DataWrapper_SerialConnection : public QObject {
    Q_OBJECT

  private slots:
    void test_dataWrapperAndDataParser();
};

void integrate_DataParser_DataWrapper_SerialConnection::
    test_dataWrapperAndDataParser() {

}

QTEST_APPLESS_MAIN(integrate_DataParser_DataWrapper_SerialConnection)
#include "integrate_DataParser_DataWrapper_SerialConnection.moc"