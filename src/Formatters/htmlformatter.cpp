#include "htmlformatter.h"

HtmlFormatter::HtmlFormatter()
{

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
    bool isClosedTag = false;
    bool isPreviousClosedTag = false;
    int closedTagStackSize = 0;
    QString result;

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
                    isClosedTag = true;
                    stackSize -= 1;
                    if (isPreviousClosedTag && closedTagStackSize > 0 && closedTagStackSize - stackSize > 1) {
                        stackSize += 1;
                    }
                    closedTagStackSize = stackSize;
                } else {
                    isPreviousClosedTag = false;
                    stackSize += 1;
                }
                setOffset(stackSize, result);
                result += "<font color=\"#8812a1\">&lt;";
                tagStarted = true;
            }
            if (charIndex == 1) {
                tagStarted = false;
                attributeStarted = false;
                result += "<font color=\"#8812a1\">&gt;</font>\n";
                if (isClosedTag || data[iterator - 1] == m_closedTag) {
                    isClosedTag = false;
                    if (!isPreviousClosedTag) stackSize -= 1;
                    isPreviousClosedTag = true;
                }
            }
            continue;
        }

        if (!contentStarted) {
            // <tag attribute=".."> handle attribute content
            if (character == m_attributeDecorator && attributeStarted) {
                if (attributeQuoteStarted) {
                    result += "&quot;</font>";
                } else {
                    result += "<font color=\"#2222dd\">&quot;";
                }
                attributeQuoteStarted = !attributeQuoteStarted;
                continue;
            }

            // <tag attribute=..> handle attribute equal
            if (character == m_attributeEqual && attributeStarted) {
                result += "=</font>";
                continue;
            }

            if (character != m_space && tagNameEnded && !attributeStarted) {
                attributeStarted = true;
                result += "<font color=\"#994500\">";
            }

            if (character == m_space && tagStarted && attributeStarted) {
                attributeStarted = false;
                result += "</font> ";
            }

            if (character == m_space && tagStarted && !tagNameEnded) {
                tagNameEnded = true;
                result += "</font> ";
            }
        }

        // <X> <any content> </X>
        if (character != m_newline && character != m_space && !contentStarted && !tagStarted && !attributeStarted) {
            contentStarted = true;
            stackSize += 1;
            setOffset(stackSize, result);
        }

        if (character == m_newline || character == m_caretBack) continue;

        if (character == m_space && !tagStarted) continue;

        result += character;
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
