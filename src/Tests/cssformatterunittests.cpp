/*
    ArdorQuery http tester
    Copyright (C) 2022-2024 Roman Vladimirov
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

#include <QtTest/QtTest>
#include "cssformatterunittests.h"
#include "../Formatters/cssformatter.h"

CssFormatterUnitTests::CssFormatterUnitTests(QObject *parent)
    : QObject{parent}
{

}

void CssFormatterUnitTests::simplestyle_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value;}");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::simplestyle_completedSilent()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value;}");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::simplestyle_multiplevalues_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; property2: lalala ; purupu  : vallll    ;}");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property2</font>: lalala;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">purupu</font>: vallll;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::simplestyle_multiplevalues_completedSilent()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value; property2: lalala ; purupu  : vallll    ;}");

    QCOMPARE(result.size(), 5);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property2</font>: lalala;)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">purupu</font>: vallll;)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::multiplestyles_multiplevalues_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; property2: lalala ; purupu  : vallll;}   .myclass2 { muhers: pruher; muhers2: pruher2; }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property2</font>: lalala;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">purupu</font>: vallll;
}
<font color="#8812a1">.myclass2</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">muhers</font>: pruher;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">muhers2</font>: pruher2;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::multiplestyles_multiplevalues_completedSilent()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value; property2: lalala ; purupu  : vallll;}   .myclass2 { muhers: pruher; muhers2: pruher2; }");

    QCOMPARE(result.size(), 9);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property2</font>: lalala;)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">purupu</font>: vallll;)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(})");
    QCOMPARE(result.value(5)->formattedLine(4), R"(<font color="#8812a1">.myclass2 </font>{)");
    QCOMPARE(result.value(6)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">muhers</font>: pruher;)");
    QCOMPARE(result.value(7)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">muhers2</font>: pruher2;)");
    QCOMPARE(result.value(8)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::nestedstyles_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; .nestedclass { nestedproperty: nestedvalue; } }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#8812a1">.nestedclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">nestedproperty</font>: nestedvalue;
&nbsp;&nbsp;&nbsp;&nbsp;}
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::nestedstyles_silent_completed()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value; .nestedclass { nestedproperty: nestedvalue; } }");

    QCOMPARE(result.size(), 6);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#8812a1">.nestedclass </font>{)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">nestedproperty</font>: nestedvalue;)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;})");
    QCOMPARE(result.value(5)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::comment_beforeStyle()
{
    CssFormatter formatter;
    auto result = formatter.format("/* This is the comment!!!! */.myclass {property: value; .nestedclass { nestedproperty: nestedvalue; } }");
    auto expectedResult = QString(R"a(<font color="#8812a1"><font color="#008000">/* This is the comment!!!! */</font>.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#8812a1">.nestedclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">nestedproperty</font>: nestedvalue;
&nbsp;&nbsp;&nbsp;&nbsp;}
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::comment_silent_beforeStyle()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat("/* This is the comment!!!! */.myclass {property: value; .nestedclass { nestedproperty: nestedvalue; } }");

    QCOMPARE(result.size(), 6);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1"><font color="#008000">/* This is the comment!!!! */</font>.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#8812a1">.nestedclass </font>{)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">nestedproperty</font>: nestedvalue;)");
    QCOMPARE(result.value(4)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;})");
    QCOMPARE(result.value(5)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::comment_insideProperty()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value /* This is the comment!!!! */; }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value <font color="#008000">/* This is the comment!!!! */</font>;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::comment_insideProperty_silent_completed()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value /* This is the comment!!!! */; }");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value<font color="#008000">/* This is the comment!!!! */</font>;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::simplestyle_singlePropertyWithoutSemicolon()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::simplestyle_singlePropertyWithoutSemicolon_silent_completed()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value }");

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(})");
}

void CssFormatterUnitTests::simplestyle_multiplePropertyLastWithoutSemicolon()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; property2: value2 }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;
&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property2</font>: value2;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::simplestyle_multiplePropertyLastWithoutSemicolon_silent_completed()
{
    CssFormatter formatter;
    auto result = formatter.silentFormat(".myclass {property: value; property2: value2 }");

    QCOMPARE(result.size(), 4);
    QCOMPARE(result.value(0)->formattedLine(4), R"(<font color="#8812a1">.myclass </font>{)");
    QCOMPARE(result.value(1)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property</font>: value;)");
    QCOMPARE(result.value(2)->formattedLine(4), R"(&nbsp;&nbsp;&nbsp;&nbsp;<font color="#009dd5">property2</font>: value2;)");
    QCOMPARE(result.value(3)->formattedLine(4), R"(})");
}
