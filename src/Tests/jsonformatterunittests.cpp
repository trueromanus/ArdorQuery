/*
    ArdorQuery http tester
    Copyright (C) 2022 Roman Vladimirov
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

void JsonFormatterUnitTests::stringOnlySilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("\"string\"");
    QCOMPARE(result.size(), 1);
    auto first = result.value(0);
    QCOMPARE(first->line(), "\"string\"");
    QCOMPARE(first->formattedLine(4), QString("<font color=\"#800000\">\"string\"</font>"));
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

void JsonFormatterUnitTests::objectOnlySilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"property\":35}");
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">35</font>)");
    QCOMPARE(result.value(1)->line(), "\"property\":35");
    QCOMPARE(result.value(2)->formattedLine(4), R"(<font color="black">}</font>)");
    QCOMPARE(result.value(2)->line(), "}");
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

void JsonFormatterUnitTests::objectOnlyMorePropertiesSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"first\": 35, \"second\": 89, \"third\": 45}");
    QCOMPARE(result.size(), 5);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"first"</font>:&nbsp;<font color="#cc7700">35</font>,)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"second"</font>:&nbsp;<font color="#cc7700">89</font>,)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"third"</font>:&nbsp;<font color="#cc7700">45</font>)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"first\":35,");
    QCOMPARE(result.value(2)->line(), "\"second\":89,");
    QCOMPARE(result.value(3)->line(), "\"third\":45");
    QCOMPARE(result.value(4)->line(), "}");
}

void JsonFormatterUnitTests::objectOnlyMorePropertiesWithNewLines()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"first\": 35,\n \"second\": 89,\n \"third\": 45}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"first"</font>:&nbsp;<font color="#cc7700">35</font>,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"second"</font>:&nbsp;<font color="#cc7700">89</font>,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"third"</font>:&nbsp;<font color="#cc7700">45</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectOnlyMorePropertiesWithNewLinesSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"first\": 35,\n \"second\": 89,\n \"third\": 45}");
    QCOMPARE(result.size(), 5);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"first"</font>:&nbsp;<font color="#cc7700">35</font>,)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"second"</font>:&nbsp;<font color="#cc7700">89</font>,)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"third"</font>:&nbsp;<font color="#cc7700">45</font>)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"first\":35,");
    QCOMPARE(result.value(2)->line(), "\"second\":89,");
    QCOMPARE(result.value(3)->line(), "\"third\":45");
    QCOMPARE(result.value(4)->line(), "}");
}

void JsonFormatterUnitTests::objectOnlyMorePropertiesWithTabulators()
{
    JsonFormatter formatter;
    auto result = formatter.format("{\"first\": 35,\t \"second\": 89,\t \"third\": 45}");
    auto expectedResult = QString(R"a(<font color="black">{</font>
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"first"</font>:&nbsp;<font color="#cc7700">35</font>,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"second"</font>:&nbsp;<font color="#cc7700">89</font>,
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"third"</font>:&nbsp;<font color="#cc7700">45</font>
<font color="black">}</font>)a");
    QCOMPARE(result, expectedResult);
}

void JsonFormatterUnitTests::objectOnlyMorePropertiesWithTabulatorsSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"first\": 35,\t \"second\": 89,\t \"third\": 45}");
    QCOMPARE(result.size(), 5);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"first"</font>:&nbsp;<font color="#cc7700">35</font>,)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"second"</font>:&nbsp;<font color="#cc7700">89</font>,)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"third"</font>:&nbsp;<font color="#cc7700">45</font>)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"first\":35,");
    QCOMPARE(result.value(2)->line(), "\"second\":89,");
    QCOMPARE(result.value(3)->line(), "\"third\":45");
    QCOMPARE(result.value(4)->line(), "}");
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

void JsonFormatterUnitTests::arrayOnlySilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("[1]");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">[</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">1</font>)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(<font color="black">]</font>)");

    QCOMPARE(result.value(0)->line(), "[");
    QCOMPARE(result.value(1)->line(), "1");
    QCOMPARE(result.value(2)->line(), "]");
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

void JsonFormatterUnitTests::arrayInObjectSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"items\": [1, 2, 3, 5, 6] }");

    QCOMPARE(result.size(), 9);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"items"</font>:&nbsp;<font color="black">[</font>)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">1</font>,)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">2</font>,)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">3</font>,)");
    QCOMPARE(result.value(5)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">5</font>,)");
    QCOMPARE(result.value(6)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#cc7700">6</font>)");
    QCOMPARE(result.value(7)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="black">]</font>)");
    QCOMPARE(result.value(8)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"items\":[");
    QCOMPARE(result.value(2)->line(), "1,");
    QCOMPARE(result.value(3)->line(), "2,");
    QCOMPARE(result.value(4)->line(), "3,");
    QCOMPARE(result.value(5)->line(), "5,");
    QCOMPARE(result.value(6)->line(), "6");
    QCOMPARE(result.value(7)->line(), "]");
    QCOMPARE(result.value(8)->line(), "}");
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

void JsonFormatterUnitTests::objectWithNegativeDigitSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"property\":-35}");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">-35</font>)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"property\":-35");
    QCOMPARE(result.value(2)->line(), "}");
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

void JsonFormatterUnitTests::objectWithHexDigitSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"property\":0xF79100}");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">0xF79100</font>)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"property\":0xF79100");
    QCOMPARE(result.value(2)->line(), "}");
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

void JsonFormatterUnitTests::objectWithDoubleDigitSilent()
{
    JsonFormatter formatter;
    auto result = formatter.silentFormat("{\"property\":0.05}");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="black">{</font>)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#750f8a">"property"</font>:&nbsp;<font color="#cc7700">0.05</font>)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(<font color="black">}</font>)");

    QCOMPARE(result.value(0)->line(), "{");
    QCOMPARE(result.value(1)->line(), "\"property\":0.05");
    QCOMPARE(result.value(2)->line(), "}");
}
