#ifndef FORMATTERLINE_H
#define FORMATTERLINE_H

#include <QMultiMap>
#include <QString>

class FormatterLine
{
private:
    QString m_line { "" };
    QString m_formattedLine { "" };
    bool m_alreadyFormatted { false };
    int m_offset { 0 };
    QMultiMap<int, std::tuple<QString, bool, bool>> m_indexes { QMultiMap<int, std::tuple<QString, bool, bool>>() };
    int m_lineIterator { -1 };

public:
    FormatterLine();

    FormatterLine(int offset);

    QString line() const noexcept { return m_line; }

    QString formattedLine(int tabSize = 1) noexcept;

    void setLine(const QString& line) noexcept;

    void setOffset(int offset) noexcept { m_offset = offset; }

    void addIndex(const QString& content, bool left, bool replace = false) noexcept;

    void addCustomIndex(int index, const QString& content, bool left, bool replace = false, bool toTop = false) noexcept;

    void changeContentInLastIndex(const QString& content) noexcept;

    void increaseLineIterator(QChar character) noexcept;

    void increaseLineIteratorString(QString content) noexcept;

    bool isEmpty() noexcept;

    int lineIterator() const noexcept { return m_lineIterator; }

    bool containsCharacter(QChar character) const noexcept;

    void removeLastCharacter() noexcept;

};

#endif // FORMATTERLINE_H
