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
    int stackSize = -1;
    bool attributeStarted = false;
    bool attributeQuoteStarted = false;
    bool tagStarted = false;
    bool tagNameEnded = false;
    bool contentStarted = false;
    int iterator = -1;
    bool isLastSelfClosedTag = false;
    bool isAttributeStringStarted = false;
    QString result;
    QString currentTag = "";

    for(auto character: data) {
        iterator++;

        auto latinCharacter = character.toLatin1();

        // < ... > handle tags
        auto charIndex = m_tag.indexOf(latinCharacter);
        if (charIndex > -1) {
            if (contentStarted) {
                contentStarted = false;
                result += "\n";
            }
            attributeStarted = false;
            tagNameEnded = false;
            if (charIndex == 0) {
                if (iterator + 1 < data.length() && data[iterator + 1] == m_closedTag) {
                    stackSize -= 1;
                } else {
                    if (!isLastSelfClosedTag) stackSize += 1;
                    isLastSelfClosedTag = false;
                }
                setOffset(stackSize, result);
                result += "<font color=\"#8812a1\">&lt;";
                tagStarted = true;
            }
            if (charIndex == 1) {
                tagStarted = false;
                attributeStarted = false;
                result += "<font color=\"#8812a1\">&gt;</font>\n";
                if (data[iterator - 1] == m_closedTag || m_selfClosingTags.contains(currentTag)) {
                    //stackSize -= 1;
                    isLastSelfClosedTag = true;
                }
                currentTag.clear();
            }
            continue;
        }

        if (!contentStarted) {
            // <tag attribute=".."> handle attribute content
            if (character == m_attributeDecorator && attributeStarted) {
                if (attributeQuoteStarted) {
                    result += "&quot;</font>";
                    isAttributeStringStarted = false;
                } else {
                    result += "<font color=\"#2222dd\">&quot;";
                    isAttributeStringStarted = true;
                }
                attributeQuoteStarted = !attributeQuoteStarted;
                continue;
            }

            // <tag attribute=..> handle attribute equal
            if (character == m_attributeEqual && attributeStarted && !isAttributeStringStarted) {
                result += "=</font>";
                continue;
            }

            if (character != m_space && tagNameEnded && !attributeStarted) {
                attributeStarted = true;
                result += "<font color=\"#994500\">";
            }

            if (character == m_space && tagStarted && attributeStarted && !isAttributeStringStarted) {
                attributeStarted = false;
                result += "</font> ";
            }

            if (character == m_space && tagStarted && !tagNameEnded) {
                tagNameEnded = true;
                result += "</font> ";
            }

            if (character == m_closedTag && tagStarted && !isAttributeStringStarted) {
                result += "<font color=\"#8812a1\">/</font>";
                continue;
            }
        }

        // <X> <any content> </X>
        if (character != m_newline && character != m_space && !contentStarted && !tagStarted && !attributeStarted) {
            contentStarted = true;
            stackSize += 1;
            setOffset(stackSize, result);
        }

        if (character == m_newline || character == m_caretBack) continue;

        if (character == m_space && !tagStarted && !contentStarted) continue;

        result += character;

        if (tagStarted && !tagNameEnded) currentTag += character;
    }

    return result;
}

void HtmlFormatter::setOffset(int stackSize, QString &target, bool newLine) noexcept
{
    if (newLine) target.append("\n");
    for (auto i = 0; i < stackSize; i++) {
        target.append(m_htmlTab);
    }
}
