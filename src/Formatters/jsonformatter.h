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
#include "formatterline.h"

class JsonFormatter: public OutputFormatter
{
private:
    const QString m_array { "[]" };
    const QString m_object { "{}" };
    const QString m_string { "\"" };
    const QString m_comma { "," };
    const QString m_space { " " };
    const QString m_colon { ":" };
    const QString m_backslash { "\\" };
    const QString m_reverse { "r" };
    const QString m_newline { "n" };
    const QString m_unicode { "u" };
    const QString m_tabulator { "\t" };
    const QString m_newlineDivider { "\n" };
    const QString m_jsonTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };
    const QString m_propertyStringStart { "<font color=\"#750f8a\">\"" };
    const QString m_plainStringStart { "<font color=\"#800000\">\"" };
    const QString m_startDigits { "-0123456789" };
    const QString m_digits { "0123456789.EexabcdfABCDF" };

public:
    JsonFormatter();

    QString format(const QString& data) override;
    QMap<int, FormatterLine*> silentFormat(const QString& data) override;
    int silentFormatTab() override { return 4; }
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // JSONFORMATTER_H
