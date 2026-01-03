#include <QTest>
#include "AsterUI/AsterUI.h"

class TestBasic : public QObject
{
    Q_OBJECT

private slots:
    void testVersion();
};

void TestBasic::testVersion()
{
    QVERIFY(!AsterUI::version().isEmpty());
}

QTEST_MAIN(TestBasic)
#include "tst_basic.moc"
