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

#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H

#include "outputformatter.h"

class XmlFormatter : public OutputFormatter
{

    const QString m_xmlTab { "&nbsp;" };
    QString m_tagStart { "<" };
    QString m_tagEnd { ">" };
    QString m_attributeEqual { "=" };
    QString m_attributeDecorator { "\"" };
    QString m_closedTag { "/" };
    QString m_exclamationPoint { "!" };
    QString m_question { "?" };
    QString m_space { " " };
    QString m_newline { "\n" };
    QString m_caretBack { "\r" };
    QString m_tabulator { "\t" };
    int m_stackSize { -1 };
    QString m_result { "" };

public:
    XmlFormatter();

    QString format(const QString& data) override;
    void formatTagWithOffset(QString& tag);
    void formatTag(QString& tag);
    void setOffset(int tabSize = 4) noexcept;
};

#endif // XMLFORMATTER_H
