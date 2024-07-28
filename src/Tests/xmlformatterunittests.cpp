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


