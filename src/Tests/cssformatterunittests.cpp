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
    <font color="#009dd5">property</font>: value;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::simplestyle_multiplevalues_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; property2: lalala ; purupu  : vallll    ;}");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
    <font color="#009dd5">property</font>: value;
    <font color="#009dd5">property2</font>: lalala;
    <font color="#009dd5">purupu</font>: vallll;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::multiplestyles_multiplevalues_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; property2: lalala ; purupu  : vallll;}   .myclass2 { muhers: pruher; muhers2: pruher2; }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
    <font color="#009dd5">property</font>: value;
    <font color="#009dd5">property2</font>: lalala;
    <font color="#009dd5">purupu</font>: vallll;
}
<font color="#8812a1">.myclass2</font> {
    <font color="#009dd5">muhers</font>: pruher;
    <font color="#009dd5">muhers2</font>: pruher2;
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::nestedstyles_completed()
{
    CssFormatter formatter;
    auto result = formatter.format(".myclass {property: value; .nestedclass { nestedproperty: nestedvalue; } }");
    auto expectedResult = QString(R"a(<font color="#8812a1">.myclass</font> {
    <font color="#009dd5">property</font>: value;
    <font color="#8812a1">.nestedclass</font> {
        <font color="#009dd5">nestedproperty</font>: nestedvalue;
    }
}
)a");
    QCOMPARE(result, expectedResult);
}

void CssFormatterUnitTests::comment_beforeStyle()
{
    CssFormatter formatter;
    auto result = formatter.format("/* This is the comment!!!! */.myclass {property: value; .nestedclass { nestedproperty: nestedvalue; } }");
    auto expectedResult = QString(R"a(<font color="#8812a1"><font color="#008000">/* This is the comment!!!! */</font>.myclass</font> {
    <font color="#009dd5">property</font>: value;
    <font color="#8812a1">.nestedclass</font> {
        <font color="#009dd5">nestedproperty</font>: nestedvalue;
    }
}
)a");
    QCOMPARE(result, expectedResult);
}
