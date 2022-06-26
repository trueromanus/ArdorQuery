#include "xmlformatter.h"

XmlFormatter::XmlFormatter()
{

}

QString XmlFormatter::format(const QString &data)
{
    QString currentFullTag = "";
    int iterator = -1;
    m_result.clear();
    m_stackSize = 0;
    bool tagStarted = false;
    bool contentStarted = false;

    for(auto character: data) {
        iterator++;

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

        if (!tagStarted && latinCharacter != m_newline && latinCharacter != m_caretBack && latinCharacter != m_space) {
            if (!contentStarted) {
                contentStarted = true;
                setOffset();
            }
            m_result.append(character);
        }
        if (latinCharacter == m_space && contentStarted) m_result.append(m_space);
        if (tagStarted) currentFullTag.append(character);
    }

    auto resultPass = m_result;
    m_result.clear();
    return resultPass;
}

void XmlFormatter::formatTagWithOffset(QString &tag)
{
    auto closedTag = tag[1] == m_closedTag;
    auto selfClosedTag = tag[tag.length() - 2] == m_closedTag;
    auto header = tag[1] == m_question;

    if (header) {
        m_result.append("<font color=\"#8812a1\">" + tag.replace("<", "&lt;").replace(">", "&gt;") + "</font>\n");
        return;
    }

    if (closedTag) {
        m_stackSize -= 1;
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

void XmlFormatter::formatTag(QString &tag)
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

        if (!closedPartStarted && (latinCharacter == m_tagEnd || latinCharacter == m_closedTag)) {
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

void XmlFormatter::setOffset(int tabSize) noexcept
{
    for (auto i = 0; i < m_stackSize * tabSize; i++) {
        m_result.append(m_xmlTab);
    }
}
