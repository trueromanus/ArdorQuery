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

#include <QList>
#include "cssformatter.h"

CssFormatter::CssFormatter()
{

}

QString CssFormatter::format(const QString &data)
{
    m_stackSize = 0;
    QString currentOpenBlock = "";
    bool isComment = false;
    m_result.clear();

    for(auto character: data) {
        auto latinCharacter = character.toLatin1();

        if (isComment) {
            if (latinCharacter == m_slash && currentOpenBlock.right(1) == m_asteriks) {
                isComment = false;
                currentOpenBlock.append(latinCharacter);
                currentOpenBlock.append("</font>");
                continue;
            } else {
                currentOpenBlock.append(latinCharacter);
                continue;
            }
        }

        if (latinCharacter == m_blockStart) {
            setOffset(m_stackSize);
            m_result.append("<font color=\"#8812a1\">" + currentOpenBlock.trimmed() + "</font> {\n");
            currentOpenBlock.clear();
            m_stackSize += 1;
            continue;
        }
        if (latinCharacter == m_blockEnd) {
            auto trimmedValue = currentOpenBlock.trimmed();
            if (!trimmedValue.isEmpty()) {
                setOffset(m_stackSize);
                fillValue(trimmedValue);
            }

            if (m_stackSize > 0) m_stackSize -= 1;
            setOffset(m_stackSize);
            m_result.append("}\n");
            currentOpenBlock.clear();
            continue;
        }

        if (latinCharacter == m_endField) {
            setOffset(m_stackSize);
            fillValue(currentOpenBlock.trimmed());
            currentOpenBlock.clear();
            continue;
        }

        if (latinCharacter == m_asteriks && !currentOpenBlock.isEmpty() && currentOpenBlock.right(1) == m_slash) {
            isComment = true;
            currentOpenBlock = currentOpenBlock.left(currentOpenBlock.size() - 1);
            currentOpenBlock.append("<font color=\"#008000\">/*");
            continue;
        }

        currentOpenBlock.append(latinCharacter);
    }

    return m_result;
}

QMap<int, FormatterLine *> CssFormatter::silentFormat(const QString &data)
{
    int stackSize = 0;
    QString currentOpenBlock = "";
    bool isComment = false;
    QMap<int, FormatterLine *> result;
    FormatterLine* formatterLine = new FormatterLine(0);
    result[0] = formatterLine;

    for(auto character: data) {
        auto latinCharacter = character.toLatin1();
        if (latinCharacter != m_space && !isComment) formatterLine->increaseLineIterator(latinCharacter);

        if (isComment && latinCharacter == m_slash && currentOpenBlock.right(1) == m_asteriks) {
            isComment = false;
            formatterLine->addIndex("</font>", true, false);
            continue;
        }
        if (isComment) continue;

        if (latinCharacter == m_blockStart) {
            formatterLine->addIndex(" </font>", true, false);
            formatterLine->addCustomIndex(0, "<font color=\"#8812a1\">", true);

            stackSize += 1;
            formatterLine = new FormatterLine(stackSize);
            result[result.size()] = formatterLine;
            continue;
        }
        if (latinCharacter == m_blockEnd) {
            if (stackSize > 0) stackSize -= 1;
            formatterLine->setOffset(stackSize);

            formatterLine = new FormatterLine(stackSize);
            result[result.size()] = formatterLine;

            continue;
        }

        // make space after : character
        if (latinCharacter == m_separator) {
            formatterLine->increaseLineIterator(m_space[0]);
        }

        if (latinCharacter == m_endField) {
            formatterLine->addCustomIndex(0, "<font color=\"#009dd5\">", true, false);
            auto propertyIndex = formatterLine->line().lastIndexOf(m_separator);
            formatterLine->addCustomIndex(propertyIndex, "</font>", true, false);

            formatterLine = new FormatterLine(stackSize);
            result[result.size()] = formatterLine;
            continue;
        }

        if (latinCharacter == m_asteriks && !currentOpenBlock.isEmpty() && currentOpenBlock.right(1) == m_slash) {
            isComment = true;
            auto index = formatterLine->lineIterator() - 1;
            formatterLine->addCustomIndex(index, "<font color=\"#008000\">", true, false);
            continue;
        }
    }

    //remove last redundant item
    if (!result.isEmpty()) {
        auto lastItem = result.value(result.size() - 1);
        if (lastItem->isEmpty()) result.remove(result.size() - 1);
    }

    return result;
}

void CssFormatter::setOffset(int stackSize) noexcept
{
    for (auto i = 0; i < stackSize; i++) {
        m_result.append(m_cssTab);
    }
}

void CssFormatter::fillValue(const QString &trimmedValue) noexcept
{
    if (trimmedValue.indexOf(":") > -1) {
        auto parts = trimmedValue.split(":");
        m_result.append("<font color=\"#009dd5\">" + parts.first().trimmed() + "</font>: ");
        m_result.append(parts.last().trimmed() + ";\n");
    } else {
        m_result.append(trimmedValue + ";\n");
    }
}
