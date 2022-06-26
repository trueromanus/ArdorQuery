#include "formatterfactory.h"
#include "../globalconstants.h"
#include "jsonformatter.h"
#include "htmlformatter.h"
#include "xmlformatter.h"

FormatterFactory::FormatterFactory()
{

}

OutputFormatter* FormatterFactory::getFormatter(const QString& formatter)
{
    if (m_instanceCache->contains(formatter)) return m_instanceCache->value(formatter);

    if (formatter == OutputFormatJson) {
        auto jsonFormatter = new JsonFormatter();
        m_instanceCache->insert(OutputFormatJson, jsonFormatter);
        return jsonFormatter;
    }

    if (formatter == OutputFormatHtml) {
        auto htmlFormatter = new HtmlFormatter();
        m_instanceCache->insert(OutputFormatHtml, htmlFormatter);
        return htmlFormatter;
    }

    if (formatter == OutputFormatXml) {
        auto xmlFormatter = new XmlFormatter();
        m_instanceCache->insert(OutputFormatXml, xmlFormatter);
        return xmlFormatter;
    }

    return m_nullableFormatter.get();
}
