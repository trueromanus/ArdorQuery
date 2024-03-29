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

#include "jsonformatter.h"

JsonFormatter::JsonFormatter()
{

}

QString JsonFormatter::format(const QString &data)
{
    int stackSize = 0;
    bool stringStarted = false;
    QString result;
    int iterator = -1;
    int skipNextCharacters = 0;
    QString currentString = "";
    bool digitStarted = false;

    for (auto character: data) {
        iterator++;
        if (skipNextCharacters > 0) {
            skipNextCharacters--;
            continue;
        }
        auto latinCharacter = character.toLatin1();
        if (stringStarted && m_string != latinCharacter) {
            if (m_backslash != latinCharacter) currentString.append(character);
            continue;
        }
        if (digitStarted && !m_digits.contains(latinCharacter)) {
            result.append("</font>");
            digitStarted = false;
        }
        auto charIndex = m_array.indexOf(latinCharacter);
        if (charIndex > -1) {
            if (charIndex == 0) {
                result += "<font color=\"black\">[</font>\n";
                stackSize += 1;
                setOffset(stackSize, result);
            }
            if (charIndex == 1) {
                stackSize -= 1;
                setOffset(stackSize, result, true);
                result += "<font color=\"black\">]</font>";
            }
            continue;
        }

        charIndex = m_object.indexOf(latinCharacter);
        if (charIndex > -1) {
            if (charIndex == 0) {
                result += "<font color=\"black\">{</font>\n";
                stackSize += 1;
                setOffset(stackSize, result);
            }
            if (charIndex == 1) {
                stackSize -= 1;
                setOffset(stackSize, result, true);
                result += "<font color=\"black\">}</font>";
            }
            continue;
        }

        if (m_string == latinCharacter) {
            if (stringStarted) {
                auto isProperty = iterator < data.size() - 1 ? data[iterator + 1].toLatin1() == m_colon : false;
                result += (isProperty ? m_propertyStringStart : m_plainStringStart)  + currentString + "\"</font>";
                currentString.clear();
            }

            stringStarted = !stringStarted;
            continue;
        }

        if (m_backslash == latinCharacter && stringStarted) {
            if (iterator < data.size()) {
                auto nextCharacter = data[iterator + 1].toLatin1();
                if (nextCharacter == m_reverse || nextCharacter == m_newline) skipNextCharacters += 1;
                if (nextCharacter == m_unicode) {
                    skipNextCharacters += 5;
                    auto code = data.mid(iterator, 6).toUtf8();
                    result.append(code);
                }
            }
            continue;
        }

        if (m_comma == latinCharacter && !stringStarted) {
            result += ",\n";
            setOffset(stackSize, result);
            continue;
        }

        if (m_colon == latinCharacter && !stringStarted) {
            result.append(":&nbsp;");
            continue;
        }

        if ((m_space == latinCharacter || m_tabulator == latinCharacter || m_newlineDivider == latinCharacter) && !stringStarted) continue;

        if (!stringStarted && !digitStarted && m_startDigits.contains(latinCharacter)) {
            result.append("<font color=\"#cc7700\">");
            result.append(latinCharacter);
            digitStarted = true;
            continue;
        }

        result.append(character);
    }

    return result;
}

void JsonFormatter::setOffset(int stackSize, QString& target, bool newLine) noexcept
{
    if (newLine) target.append("\n");
    for (auto i = 0; i < stackSize; i++) {
        target.append(m_jsonTab);
    }
}
