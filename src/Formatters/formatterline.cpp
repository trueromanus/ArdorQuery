#include <QSet>
#include <QRegularExpression>
#include "formatterline.h"

const QString FormatterLine::StartSelectionTag = "<span style=\"background-color: #788a8a5c; color: white;\">";
const QString FormatterLine::EndSelectionTag = "</span>";
const QRegularExpression FormatterLine::ColorRegularExpression = QRegularExpression(R"(\<font\ color=\"[A-Za-z0-9\#]{0,}\"\>)", QRegularExpression::CaseInsensitiveOption);

FormatterLine::FormatterLine() {}

FormatterLine::FormatterLine(int offset)
{
    m_offset = offset;
}

FormatterLine::~FormatterLine()
{
    m_line.clear();
    m_formattedLine.clear();
    m_indexes.clear();
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

            auto tuple = getContents(indexItems);
            QString leftContent = std::get<0>(tuple);
            QString replaceContent = std::get<1>(tuple);
            QString rightContent = std::get<2>(tuple);

            result.append(leftContent);
            result.append(replaceContent.isEmpty() ? character : replaceContent);
            result.append(rightContent);
        } else {
            result.append(character);
        }
        index++;
    }

    m_formattedLine = result;
    m_alreadyFormatted = true;

    return result;
}

QString FormatterLine::formattedLineWithSelection(int tabSize, int startSelectionPosition, int endSelectionPosition) noexcept
{
    auto countTabs = tabSize * m_offset;
    QList<QString> tabs;
    tabs.reserve(countTabs);
    tabs.fill("&nbsp;", countTabs);
    QString result = tabs.join("");

    if (endSelectionPosition == -1) endSelectionPosition = m_line.size() - 1;
    if (endSelectionPosition < startSelectionPosition) {
        auto tempEndSelection = startSelectionPosition;
        startSelectionPosition = endSelectionPosition;
        endSelectionPosition = tempEndSelection;
    }

    //need to select all line and don't need make ay other format
    auto index = 0;

    foreach (auto character, m_line) {
        if (m_indexes.contains(index)) {
            auto indexItems = m_indexes.values(index);

            auto tuple = getContents(indexItems);
            QString leftContent = std::get<0>(tuple);
            QString replaceContent = std::get<1>(tuple);
            QString rightContent = std::get<2>(tuple);

            //if inside selection need to clear colors from all tags
            if (index >= startSelectionPosition && index <= endSelectionPosition) {
                if (!leftContent.isEmpty()) leftContent = leftContent.replace(FormatterLine::ColorRegularExpression, "").replace("</font>", "");
                if (!rightContent.isEmpty()) rightContent = rightContent.replace(FormatterLine::ColorRegularExpression, "").replace("</font>", "");
                if (!replaceContent.isEmpty()) replaceContent = replaceContent.replace(FormatterLine::ColorRegularExpression, "").replace("</font>", "");
            }

            if (index == startSelectionPosition) leftContent = leftContent.insert(0, FormatterLine::StartSelectionTag);
            if (index == endSelectionPosition) rightContent = rightContent.append(FormatterLine::EndSelectionTag);

            result.append(leftContent);
            result.append(replaceContent.isEmpty() ? character : replaceContent);
            result.append(rightContent);
        } else {
            auto characterContent = QString(character);
            if (index == startSelectionPosition) characterContent.insert(0, FormatterLine::StartSelectionTag);
            if (index == endSelectionPosition) characterContent.append(FormatterLine::EndSelectionTag);

            result.append(characterContent);
        }
        index++;
    }

    return result;
}

void FormatterLine::fillLineWithOffset(int tabSize) noexcept
{
    if (m_line.size() == 0) return;
    if (m_line.size() > 0 && m_lineWithOffset.size() > 0) return;

    auto offsetCharacters = m_offset * tabSize;
    QString result;
    result.reserve(offsetCharacters + m_line.size());

    for(int i = 0; i < offsetCharacters; i++) {
        result.append(" ");
    }
    result.append(m_line);

    m_lineWithOffset = result;
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

void FormatterLine::addCustomIndex(int index, const QString &content, bool left, bool replace, bool toTop) noexcept
{
    auto tuple = std::make_tuple(content, left, replace);

    if (toTop) {
        auto values = m_indexes.values(index);
        m_indexes.remove(index);

        m_indexes.insert(index, tuple);
        foreach (auto value, values) {
            m_indexes.insert(index, value);
        }
    } else {
        m_indexes.insert(index, tuple);
    }
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

void FormatterLine::increaseLineIterator(QChar content) noexcept
{
    m_lineIterator++;
    m_line.append(content);
}

void FormatterLine::increaseLineIteratorString(QString content) noexcept
{
    m_lineIterator += content.size();
    m_line.append(content);
}

bool FormatterLine::isEmpty() noexcept
{
    return m_line.isEmpty();
}

bool FormatterLine::containsCharacter(QChar character) const noexcept
{
    return m_line.contains(character);
}

void FormatterLine::removeLastCharacter() noexcept
{
    m_line.removeLast();
}

std::tuple<QString, QString, QString> FormatterLine::getContents(const QList<std::tuple<QString, bool, bool> > &items)
{
    auto reversedIndexItems = QList<std::tuple<QString,bool,bool>>();
    foreach (auto indexItem, items) {
        reversedIndexItems.insert(0, indexItem);
    }

    QString leftContent = "";
    QString rightContent = "";
    QString replaceContent = "";

    foreach (auto indexItem, reversedIndexItems) {
        auto content = std::get<0>(indexItem);
        auto left = std::get<1>(indexItem);
        auto replace = std::get<2>(indexItem);
        if (replace) {
            replaceContent.append(content);
        } else {
            if (left) {
                leftContent.append(content);
            } else {
                rightContent.append(content);
            }
        }
    }

    reversedIndexItems.clear();

    return std::make_tuple(leftContent, replaceContent, rightContent);
}


