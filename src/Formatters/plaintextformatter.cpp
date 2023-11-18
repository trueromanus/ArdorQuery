#include "plaintextformatter.h"

PlainTextFormatter::PlainTextFormatter()
{

}

QString PlainTextFormatter::format(const QString &data)
{
    QString result;

    auto lineWidth = 0;

    for(auto character: data) {
        auto latinCharacter = character.toLatin1();

        if (latinCharacter == m_newline || latinCharacter == m_caretBack) {
            lineWidth = 0;
            result.append(character);
            continue;
        }

        if (lineWidth >= 200) {
            result.append("\n");
            lineWidth = 0;
        } else {
            lineWidth += 1;
        }
        result.append(character);
    }

    return result;
}
