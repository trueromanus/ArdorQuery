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

QMap<int, FormatterLine *> PlainTextFormatter::silentFormat(const QString &data)
{
    QMap<int, FormatterLine *> result;
    FormatterLine* formatterLine = new FormatterLine(0);
    result[0] = formatterLine;

    auto lineWidth = 0;

    for(auto character: data) {
        auto latinCharacter = character.toLatin1();
        formatterLine->increaseLineIterator(latinCharacter);

        if (latinCharacter == m_newline || latinCharacter == m_caretBack) {
            formatterLine = new FormatterLine(0);
            result[result.size()] = formatterLine;

            lineWidth = 0;
            continue;
        }

        if (lineWidth >= 200) {
            formatterLine = new FormatterLine(0);
            result[result.size()] = formatterLine;

            lineWidth = 0;
        } else {
            lineWidth += 1;
        }
    }

    return result;
}
