#include <QSet>
#include "formatterline.h"

FormatterLine::FormatterLine() {}

FormatterLine::FormatterLine(int offset)
{
    m_offset = offset;
}

QString FormatterLine::formattedLine(int tabSize) noexcept
{
    if (m_alreadyFormatted) return m_formattedLine;

    QString result;

    QList<QString> tabs;
    tabs.fill("&nbsp;", tabSize);
    auto tab = tabs.join("");

    for (auto i = 0; i < m_offset; i++) {
        result.append(tab);
    }

    auto index = 0;

    foreach (auto character, m_line) {
        if (m_indexes.contains(index)) {
            auto indexItems = m_indexes.values(index);
            bool isCharacterInserted = false;

            auto reversedIndexItems = QList<std::tuple<QString,bool,bool>>();
            foreach (auto indexItem, indexItems) {
                reversedIndexItems.insert(0, indexItem);
            }

            foreach (auto indexItem, reversedIndexItems) {
                auto content = std::get<0>(indexItem);
                auto left = std::get<1>(indexItem);
                auto replace = std::get<2>(indexItem);
                if (replace) {
                    result.append(content);
                } else {
                    if (left) {
                        result.append(content + (isCharacterInserted ? "" : QString(character)));
                    } else {
                        result.append((isCharacterInserted ? "" : QString(character)) + content);
                    }
                    if (!isCharacterInserted) isCharacterInserted = true;
                }
            }
        } else {
            result.append(character);
        }
        index++;
    }

    m_formattedLine = result;
    m_alreadyFormatted = true;

    return result;
}

void FormatterLine::setLine(const QString &line) noexcept
{
    m_line = line;
    m_indexes.clear();
}

void FormatterLine::addIndex(const QString &content, bool left, bool replace) noexcept
{
    auto tuple = std::make_tuple(content, left, replace);
    m_indexes.insert(m_lineIterator, tuple);
}

void FormatterLine::changeContentInLastIndex(const QString &content) noexcept
{
    if (m_indexes.isEmpty()) return;

    auto lastIndex = m_indexes.size() - 1;
    auto previousItems = m_indexes.values(lastIndex);
    std::get<0>(previousItems[0]) = content;
    m_indexes.remove(lastIndex);
    foreach(auto previousItem, previousItems) {
        m_indexes.insert(lastIndex, previousItem);
    }
}

void FormatterLine::increaseLineIterator(QChar character) noexcept
{
    m_lineIterator++;
    m_line.append(character);
}

bool FormatterLine::isEmpty() noexcept
{
    return m_line.isEmpty();
}
