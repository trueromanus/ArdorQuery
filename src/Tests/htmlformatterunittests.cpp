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
