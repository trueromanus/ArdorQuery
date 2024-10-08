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

#include "htmlformatter.h"

HtmlFormatter::HtmlFormatter()
{
    m_selfClosingTags.insert("area");
    m_selfClosingTags.insert("base");
    m_selfClosingTags.insert("br");
    m_selfClosingTags.insert("col");
    m_selfClosingTags.insert("embed");
    m_selfClosingTags.insert("hr");
    m_selfClosingTags.insert("img");
    m_selfClosingTags.insert("input");
    m_selfClosingTags.insert("link");
    m_selfClosingTags.insert("meta");
    m_selfClosingTags.insert("param");
    m_selfClosingTags.insert("source");
    m_selfClosingTags.insert("track");
    m_selfClosingTags.insert("wbr");
}

QString HtmlFormatter::format(const QString &data)
{
    m_stackSize = -1;
    QString currentFullTag = "";
    m_result.clear();
    bool tagStarted = false;
    bool contentStarted = false;

    for(auto character: data) {
        auto latinCharacter = character.toLatin1();

        if (latinCharacter == m_tagStart && !tagStarted) {
            tagStarted = true;
            currentFullTag = latinCharacter;
            if (contentStarted) {
                contentStarted = false;
                m_result.append("\n");
            }
            continue;
        }

        if (latinCharacter == m_tagEnd && tagStarted) {
            tagStarted = false;
            currentFullTag.append(character);
            formatTagWithOffset(currentFullTag);
            currentFullTag.clear();
            continue;
        }

        if (!tagStarted && latinCharacter != m_newline && latinCharacter != m_caretBack && latinCharacter != m_space && latinCharacter != m_tabulator) {
            if (!contentStarted) {
                contentStarted = true;
                setOffset();
            }
            m_result.append(character);
        }
        if (latinCharacter == m_space && contentStarted) m_result.append(m_space);
        if (tagStarted && latinCharacter != m_newline) currentFullTag.append(character);
    }

    auto resultPass = m_result;
    m_result.clear();
    return resultPass;
}

bool HtmlFormatter::isSelfClosedTag(const QString &tag)
{
    if (tag[tag.length() - 2] == m_closedTag) return true;

    auto innerTag = tag;
    innerTag = innerTag.replace("<", "").replace(">", "").replace("/", "");
    auto indexSpace = innerTag.indexOf(" ");
    if (indexSpace > -1) innerTag = innerTag.mid(0, indexSpace);

    return m_selfClosingTags.contains(innerTag);
}

void HtmlFormatter::formatTagWithOffset(QString &tag)
{
    auto closedTag = tag[1] == m_closedTag;
    auto selfClosedTag = isSelfClosedTag(tag);
    auto header = tag.startsWith(m_doctype) || tag.startsWith(m_upperDoctype);
    auto comment = tag.startsWith(m_comment);

    if (header) {
        m_result.append("<font color=\"lightgray\">" + tag.replace("<", "&lt;").replace(">", "&gt;") + "</font>\n");
        return;
    }

    if (comment) {
        setOffset();
        m_result.append("<font color=\"#008000\">" + tag.replace("<", "&lt;").replace(">", "&gt;") + "</font>\n");
        return;
    }

    if (closedTag) {
        if (!selfClosedTag) m_stackSize -= 1;
        setOffset();
        formatTag(tag);
    } else if (selfClosedTag) {
        setOffset();
        formatTag(tag);
    } else {
        setOffset();
        formatTag(tag);
        m_stackSize += 1;
    }
}

void HtmlFormatter::formatTag(QString &tag)
{
    auto contentIndex = tag.indexOf(m_space);
    // it means tag without attributes, and we can use shortcut
    if (contentIndex == -1) {
        m_result.append("<font color=\"#8812a1\">" + tag.replace("<", "&lt;").replace(">", "&gt;") + "</font>\n");
        return;
    }

    bool attributeStarted = false;
    bool stringStarted = false;
    bool tagNameStarted = true;
    bool closedPartStarted = false;

    m_result.append("<font color=\"#8812a1\">");

    foreach(auto character, tag) {
        auto latinCharacter = character.toLatin1();

        if (latinCharacter == m_space && !stringStarted) {
            if (attributeStarted) {
                attributeStarted = false;
                m_result.append("</font>");
            }
            if (tagNameStarted) {
                tagNameStarted = false;
                m_result.append("</font>");
            }
            m_result.append(" ");
        }

        if (latinCharacter == m_tagStart) {
            m_result.append("&lt;");
            continue;
        }

        if (!closedPartStarted && !attributeStarted && (latinCharacter == m_tagEnd || latinCharacter == m_closedTag)) {
            m_result.append("</font><font color=\"#8812a1\">");
            closedPartStarted = true;
        }

        if (latinCharacter == m_tagEnd) {
            m_result.append("&gt;");
            continue;
        }

        if (latinCharacter == m_closedTag) {
            m_result.append("/");
            continue;
        }

        if (latinCharacter == m_attributeDecorator) {
            if (stringStarted) {
                stringStarted = false;
                m_result.append("&quot;</font>");
            } else {
                stringStarted = true;
                m_result.append("<font color=\"#2222dd\">&quot;");
            }
            continue;
        }

        if (tagNameStarted) {
            m_result.append(character);
            continue;
        }

        if (!attributeStarted) {
            attributeStarted = true;
            m_result.append("<font color=\"#994500\">");
        }

        m_result.append(character);
    }

    m_result.append("</font>\n");
}

void HtmlFormatter::setOffset(int tabSize) noexcept
{
    for (auto i = 0; i < m_stackSize * tabSize; i++) {
        m_result.append(m_htmlTab);
    }
}
