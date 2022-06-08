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

    for (auto character: data) {
        iterator++;
        if (skipNextCharacters > 0) {
            skipNextCharacters--;
            continue;
        }
        auto latinCharacter = character.toLatin1();
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
            if (!stringStarted) result += "<font color=\"#ac0097\">\"";
            if (stringStarted) result += "\"</font>";

            stringStarted = !stringStarted;
            continue;
        }

        if (m_backslash == latinCharacter && stringStarted) {
            if (iterator < data.count()) {
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

        if (m_space == latinCharacter && !stringStarted) continue;

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
