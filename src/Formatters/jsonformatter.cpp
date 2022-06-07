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

    for (auto character: data) {
        iterator++;
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
                setOffset(stackSize, result);
                result += "\n<font color=\"black\">}</font>";
            }
            continue;
        }

        if (m_string == latinCharacter) {
            if (!stringStarted) result += "<font color=\"#ac0097\">\"";
            if (stringStarted) result += "\"</font>";

            stringStarted = !stringStarted;
            continue;
        }

        if (m_comma == latinCharacter && !stringStarted) {
            result += ",\n";
            setOffset(stackSize, result);
            continue;
        }

        if (m_space == latinCharacter && !stringStarted) {
            if (iterator > 0 && data[iterator - 1].toLatin1() != m_colon) continue;
            if (iterator == 0) continue;
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
