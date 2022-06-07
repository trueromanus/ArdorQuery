#include "formatterfactory.h"
#include "../globalconstants.h"
#include "jsonformatter.h"

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
    //TODO: implement xml formatter
    //if (formatter == OutputFormatXml) m_instanceCache->insert(OutputFormatJson, JsonFormatter());

    return m_nullableFormatter.get();
}
