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
    QCOMPARE(result, QString("<font color=\"#ac0097\">\"string\"</font>"));
}

void JsonFormatterUnitTests::objectOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"property\": 35}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#ac0097">"property"</font>: 35
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectOnlyMoreProperties()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"first\": 35, \"second\": 89, \"third\": 45}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#ac0097">"first"</font>: 35,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#ac0097">"second"</font>: 89,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#ac0097">"third"</font>: 45
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::arrayOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("[1]");
    auto expectedResult = QString(R"a(<font color="black">[</font>
&nbsp;&nbsp;&nbsp;&nbsp;1
<font color="black">]</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::arrayInObject()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"items\": [1, 2, 3, 5, 6] }");
    auto str = R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#ac0097">"items"</font>: <font color="black">[</font>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6
&nbsp;&nbsp;&nbsp;&nbsp;<font color="black">]</font>
<font color="black">}</font>)a";
    QCOMPARE(result, QString(str));
}
