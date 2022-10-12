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

#include "htmlformatterunittests.h"
#include "../Formatters/htmlformatter.h"

HtmlFormatterUnitTests::HtmlFormatterUnitTests(QObject *parent)
    : QObject{parent}
{

}

void HtmlFormatterUnitTests::htmlLowerHeader()
{
    HtmlFormatter formatter;
    auto result = formatter.format("<!doctype html>");
    auto expectedResult = QString(R"a(<font color="lightgray">&lt;!doctype html&gt;</font>
)a");
    QCOMPARE(result, expectedResult);
}

void HtmlFormatterUnitTests::htmlUpperHeader()
{
    HtmlFormatter formatter;
    auto result = formatter.format("<!DOCTYPE html>");
    auto expectedResult = QString(R"a(<font color="lightgray">&lt;!DOCTYPE html&gt;</font>
)a");
    QCOMPARE(result, expectedResult);
}

void HtmlFormatterUnitTests::emptyFullTag()
{
    HtmlFormatter formatter;
    auto result = formatter.format("<test></test>");
    auto expectedResult = QString(R"(<font color="#8812a1">&lt;test&gt;</font>
<font color="#8812a1">&lt;/test&gt;</font>
)");
    QCOMPARE(result, expectedResult);
}

void HtmlFormatterUnitTests::attributeWithUrl()
{
    HtmlFormatter formatter;
    auto result = formatter.format(R"(<test attr="http://www.test.com"></test>)");
    auto expectedResult = QString(R"(<font color="#8812a1">&lt;test</font> <font color="#994500"> attr=<font color="#2222dd">&quot;http://www.test.com&quot;</font>&gt;</font>
<font color="#8812a1">&lt;/test&gt;</font>
)");
    QCOMPARE(result, expectedResult);
}

void HtmlFormatterUnitTests::commentTag()
{
    HtmlFormatter formatter;
    auto result = formatter.format("<test><!-- comment --></test>");
    auto expectedResult = QString(R"(<font color="#8812a1">&lt;test&gt;</font>
<font color="#008000">&lt;!-- comment --&gt;</font>
<font color="#8812a1">&lt;/test&gt;</font>
)");
    QCOMPARE(result, expectedResult);
}
