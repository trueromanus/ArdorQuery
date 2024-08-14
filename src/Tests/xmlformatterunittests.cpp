#include "xmlformatterunittests.h"
#include "../Formatters/xmlformatter.h"

XmlFormatterUnitTests::XmlFormatterUnitTests(QObject *parent)
    : QObject{parent}
{}

void XmlFormatterUnitTests::xmlOnlyHeaderSilent()
{
    XmlFormatter formatter;
    auto result = formatter.silentFormat(R"(<?xml version="1.0" encoding="UTF-8"?>)");

    QCOMPARE(result.size(), 1);
    QCOMPARE(result.value(0)->formattedLine(1), R"(<font color="gray">&lt;?xml version="1.0" encoding="UTF-8"?&gt;</font>)");
}

void XmlFormatterUnitTests::xmlEmptyTagSilent()
{
    XmlFormatter formatter;
    auto result = formatter.silentFormat(
        R"(
<?xml version="1.0" encoding="UTF-8"?>
<doc>
</doc>
)"
    );

    QCOMPARE(result.size(), 3);
    QCOMPARE(result.value(0)->formattedLine(1), R"(<font color="gray">&lt;?xml version="1.0" encoding="UTF-8"?&gt;</font>)");
    QCOMPARE(result.value(1)->formattedLine(1), R"(<font color="#8812a1">&lt;doc&gt;</font>)");
    QCOMPARE(result.value(2)->formattedLine(1), R"(<font color="#8812a1">&lt;/doc&gt;</font>)");
}

void XmlFormatterUnitTests::xmlNestedEmptyTagSilent()
{
    XmlFormatter formatter;
    auto result = formatter.silentFormat(
        R"(
<?xml version="1.0" encoding="UTF-8"?>
<doc>
    <part>
    </part>
</doc>
)"
        );

    QCOMPARE(result.size(), 5);
    QCOMPARE(result.value(0)->formattedLine(1), R"(<font color="gray">&lt;?xml version="1.0" encoding="UTF-8"?&gt;</font>)");
    QCOMPARE(result.value(1)->formattedLine(1), R"(<font color="#8812a1">&lt;doc&gt;</font>)");
    QCOMPARE(result.value(2)->formattedLine(1), R"(&nbsp;<font color="#8812a1">&lt;part&gt;</font>)");
    QCOMPARE(result.value(3)->formattedLine(1), R"(&nbsp;<font color="#8812a1">&lt;/part&gt;</font>)");
    QCOMPARE(result.value(4)->formattedLine(1), R"(<font color="#8812a1">&lt;/doc&gt;</font>)");
}

void XmlFormatterUnitTests::attibuteWithUrlSilent()
{
    XmlFormatter formatter;
    auto result = formatter.silentFormat(R"(<test attr="http://www.test.com"></test>)");

    QCOMPARE(result.size(), 2);

    QCOMPARE(result.value(0)->formattedLine(1), R"(<font color="#8812a1">&lt;test</font><font color="#994500"> attr=<font color="#2222dd">&quot;http://www.test.com&quot;</font>&gt;</font>)");
    QCOMPARE(result.value(1)->formattedLine(1), R"(<font color="#8812a1">&lt;/test&gt;</font>)");
}


