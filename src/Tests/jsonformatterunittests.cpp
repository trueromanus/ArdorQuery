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
    QCOMPARE(result, QString("<font color=\"#800000\">\"string\"</font>"));
}

void JsonFormatterUnitTests::objectOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"property\":35}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">35</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectOnlyMoreProperties()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"first\": 35, \"second\": 89, \"third\": 45}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"first"</font>:&nbsp;<font color="#cc7700">35</font>,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"second"</font>:&nbsp;<font color="#cc7700">89</font>,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"third"</font>:&nbsp;<font color="#cc7700">45</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::arrayOnly()
{
    JsonFormatter formatter;
    auto result = formatter.format("[1]");
    auto expectedResult = QString(R"a(<font color="black">[</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">1</font>
<font color="black">]</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::arrayInObject()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"items\": [1, 2, 3, 5, 6] }");
    auto str = R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"items"</font>:&nbsp;<font color="black">[</font>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">1</font>,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">2</font>,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">3</font>,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">5</font>,
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">6</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="black">]</font>
<font color="black">}</font>)a";
    QCOMPARE(result, QString(str));
}

void JsonFormatterUnitTests::objectWithNegativeDigit()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"property\":-35}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">-35</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectWithHexDigit()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"property\":0xF79100}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">0xF79100</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectWithDoubleDigit()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"property\":0.05}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">0.05</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}
