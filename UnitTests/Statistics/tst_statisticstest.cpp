#include <QtTest>
#include "stats/statistics.hh"


class StatisticsTest : public QObject
{
    Q_OBJECT

public:
    StatisticsTest();

private slots:
    void test_morePassengers();
    void test_nysseRemoved();
    void test_newNysse();
    void test_nysseLeft();
    void test_passengerRemoved();

};

StatisticsTest::StatisticsTest()
{

}

void StatisticsTest::test_morePassengers()
{
    Game::Statistics teststats;
    teststats.morePassengers(5);
    QVERIFY(teststats.getPassengertotal() == 5);
}

void StatisticsTest::test_nysseRemoved()
{
    Game::Statistics teststats;
    teststats.nysseRemoved();
    QVERIFY(teststats.getScore() == teststats.getNYSSESCORE());
}

void StatisticsTest::test_newNysse()
{
    Game::Statistics teststats;
    teststats.newNysse();
    QVERIFY(teststats.getNyssetotal() == 1);
}

void StatisticsTest::test_nysseLeft()
{
    Game::Statistics teststats;
    teststats.nysseLeft();
    QVERIFY(teststats.getNyssesleaved() == 1);
}

void StatisticsTest::test_passengerRemoved()
{
    Game::Statistics teststats;
    teststats.passengerRemoved();
    QVERIFY(teststats.getScore() == teststats.getPASSENGERSCORE());

}

QTEST_APPLESS_MAIN(StatisticsTest)

#include "tst_statisticstest.moc"
