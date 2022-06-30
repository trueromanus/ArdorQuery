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

#ifndef HTMLFORMATTER_H
#define HTMLFORMATTER_H

#include <QSet>
#include "outputformatter.h"

class HtmlFormatter : public OutputFormatter
{
private:
    QString m_tag { "<>" };
    QString m_attributeEqual { "=" };
    QString m_attributeDecorator { "\"" };
    QString m_closedTag { "/" };
    QString m_doctype { "!doctype" };
    QString m_comment { "!--" };
    QString m_exclamationPoint { "!" };
    QString m_space { " " };
    QString m_newline { "\n" };
    QString m_caretBack { "\r" };
    QString m_scriptTag { "script" };
    QSet<QString> m_selfClosingTags { QSet<QString>() };
    const QString m_htmlTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };

public:
    HtmlFormatter();

    QString format(const QString& data) override;
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // HTMLFORMATTER_H
