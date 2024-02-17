#include "globalvariablesunittest.h"
#include "../ListModels/globalvariableslistmodel.h"

GlobalVariablesUnitTest::GlobalVariablesUnitTest(QObject *parent)
    : QObject{parent}
{

}

void GlobalVariablesUnitTest::replaceGlobalVariables_notVariables()
{
    GlobalVariablesListModel model;
    QString testString = "test string for empty item";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string for empty item";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::replaceGlobalVariables_singleVariables()
{
    GlobalVariablesListModel model;
    model.addVariable("variable", "value");
    QString testString = "test string {{variable}} empty item";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string value empty item";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::replaceGlobalVariables_multipleVariables()
{
    GlobalVariablesListModel model;
    model.addVariable("variable", "value");
    model.addVariable("variable2", "value2");
    QString testString = "test string {{variable}} empty {{variable2}}";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string value empty value2";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::replaceGlobalVariables_anotherVariable()
{
    GlobalVariablesListModel model;
    model.addVariable("variable2", "value");
    QString testString = "test string {{variable}} empty item";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string {{variable}} empty item";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::replaceGlobalVariables_multipleTimesVariable()
{
    GlobalVariablesListModel model;
    model.addVariable("variable", "value");
    QString testString = "test string {{variable}} empty {{variable}}";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string value empty value";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::parseLines_singlecorrectVariable()
{
    GlobalVariablesListModel model;
    model.addLine();
    model.setLine(0, "variable value");
    model.parseLines();
    QString testString = "test string {{variable}} empty";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string value empty";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::parseLines_singleEmptyVariable()
{
    GlobalVariablesListModel model;
    model.addLine();
    model.setLine(0, "variable");
    model.parseLines();
    QString testString = "test string {{variable}} empty";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string {{variable}} empty";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::parseLines_singleIncorrectVariable()
{
    GlobalVariablesListModel model;
    model.addLine();
    model.setLine(0, "variable2");
    model.parseLines();
    QString testString = "test string {{variable}} empty";
    auto result = model.replaceGlobalVariables(testString);

    QString resultString = "test string {{variable}} empty";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::parseLines_dateTimeUtcNowPredefinedVariable()
{
    GlobalVariablesListModel model;
    QString testString = "test string {{dateTimeNow}} empty";
    auto result = model.replaceGlobalVariables(testString);

    QDateTime date = QDateTime::currentDateTime();
    QString dateAsAstring = date.toString(Qt::ISODate);

    QString resultString = "test string " + dateAsAstring + " empty";
    QCOMPARE(result, resultString);
}

void GlobalVariablesUnitTest::parseLines_ttime24HoursNowPredefinedVariable()
{
    GlobalVariablesListModel model;
    QString testString = "test string {{time24HoursNow}} empty";
    auto result = model.replaceGlobalVariables(testString);

    auto time = QDateTime::currentDateTimeUtc().time();
    QString timeAsAstring = time.toString("hh:mm:ss");

    QString resultString = "test string " + timeAsAstring + " empty";
    QCOMPARE(result, resultString);
}
