/*
    ArdorQuery http tester
    Copyright (C) 2022-2024 Roman Vladimirov
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

#ifndef CSSFORMATTER_H
#define CSSFORMATTER_H

#include "outputformatter.h"

class CssFormatter : public OutputFormatter
{
private:
    const QString m_blockStart { "{" };
    const QString m_blockEnd { "}" };
    const QString m_separator { ":" };
    const QString m_atRules { "@" };
    const QString m_endField { ";" };
    const QString m_import { "@import" };
    const QString m_newline { "\n" };
    const QString m_asteriks { "*" };
    const QString m_slash { "/" };
    const QString m_caretBack { "\r" };
    const QString m_cssTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };
    int m_stackSize { -1 };
    QString m_result { "" };

public:
    CssFormatter();

    QString format(const QString& data) override;

private:
    void setOffset(int stackSize) noexcept;
    void fillValue(const QString& trimmedValue) noexcept;

};

#endif // CSSFORMATTER_H
