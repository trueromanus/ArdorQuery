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

#include "formatterfactory.h"
#include "../globalconstants.h"
#include "jsonformatter.h"
#include "htmlformatter.h"
#include "xmlformatter.h"
#include "plaintextformatter.h"
#include "cssformatter.h"

FormatterFactory::FormatterFactory()
{

}

OutputFormatter* FormatterFactory::getFormatter(const QString& formatter)
{
    if (m_instanceCache.contains(formatter)) return m_instanceCache.value(formatter);

    if (formatter == OutputFormatJson) {
        auto jsonFormatter = new JsonFormatter();
        m_instanceCache.insert(OutputFormatJson, jsonFormatter);
        return jsonFormatter;
    }

    if (formatter == OutputFormatHtml) {
        auto htmlFormatter = new HtmlFormatter();
        m_instanceCache.insert(OutputFormatHtml, htmlFormatter);
        return htmlFormatter;
    }

    if (formatter == OutputFormatXml) {
        auto xmlFormatter = new XmlFormatter();
        m_instanceCache.insert(OutputFormatXml, xmlFormatter);
        return xmlFormatter;
    }

    if (formatter == OutputFormatPlainText) {
        auto plainTextFormatter = new PlainTextFormatter();
        m_instanceCache.insert(OutputFormatPlainText, plainTextFormatter);
        return plainTextFormatter;
    }

    if (formatter == OutputFormatCss) {
        auto cssFormatter = new CssFormatter();
        m_instanceCache.insert(OutputFormatCss, cssFormatter);
        return cssFormatter;
    }

    return m_nullableFormatter;
}
