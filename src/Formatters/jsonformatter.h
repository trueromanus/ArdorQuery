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

#ifndef JSONFORMATTER_H
#define JSONFORMATTER_H

#include "outputformatter.h"

class JsonFormatter: public OutputFormatter
{
private:
    QString m_array { "[]" };
    QString m_object { "{}" };
    QString m_string { "\"" };
    QString m_comma { "," };
    QString m_space { " " };
    QString m_colon { ":" };
    QString m_backslash { "\\" };
    QString m_reverse { "r" };
    QString m_newline { "n" };
    QString m_unicode { "u" };
    const QString m_jsonTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };

public:
    JsonFormatter();

    QString format(const QString& data) override;
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // JSONFORMATTER_H
