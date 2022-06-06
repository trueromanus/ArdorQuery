#include "jsonformatterunittests.h"
#include "../Formatters/jsonformatter.h"

JsonFormatterUnitTests::JsonFormatterUnitTests(QObject *parent)
    : QObject{parent}
{

}

void JsonFormatterUnitTests::stringOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("\"string\"");
    QCOMPARE(result, QString("<font color=\"#ff66b3\">\"string\"</font>"));
}

void JsonFormatterUnitTests::objectOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"property\": 35}");
    auto expectedResult = QString(R"a(<font color="#cc00ff">{</font>
    <font color="#ff66b3">"property"</font>: 35
<font color="#cc00ff">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectOnlyMoreProperties()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"first\": 35, \"second\": 89, \"third\": 45}");
    auto expectedResult = QString(R"a(<font color="#cc00ff">{</font>
    <font color="#ff66b3">"first"</font>: 35,
    <font color="#ff66b3">"second"</font>: 89,
    <font color="#ff66b3">"third"</font>: 45
<font color="#cc00ff">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::arrayOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("[1]");
    auto expectedResult = QString(R"a(<font color="#cc00ff">[</font>
    1
<font color="#cc00ff">]</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::arrayInObject()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"items\": [1, 2, 3, 5, 6] }");
    auto str = R"a(<font color="#cc00ff">{</font>
    <font color="#ff66b3">"items"</font>: <font color="#cc00ff">[</font>
        1,
        2,
        3,
        5,
        6
    <font color="#cc00ff">]</font>
<font color="#cc00ff">}</font>)a";
    QCOMPARE(result, QString(str));
}
