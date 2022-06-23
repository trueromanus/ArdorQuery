#include "xmlformatter.h"

XmlFormatter::XmlFormatter()
{

}

QString XmlFormatter::format(const QString &data)
{
    return data;
}

void XmlFormatter::setOffset(int stackSize, QString &target, bool newLine) noexcept
{
    if (newLine) target.append("\n");
    for (auto i = 0; i < stackSize; i++) {
        target.append(m_xmlTab);
    }
}
