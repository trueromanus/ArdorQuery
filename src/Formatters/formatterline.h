#ifndef FORMATTERLINE_H
#define FORMATTERLINE_H

#include <QMultiMap>
#include <QString>

class FormatterLine
{
private:
    QString m_line { "" };
    QString m_lineWithOffset { "" };
    QString m_formattedLine { "" };
    bool m_alreadyFormatted { false };
    int m_offset { 0 };
    QMultiMap<int, std::tuple<QString, bool, bool>> m_indexes { QMultiMap<int, std::tuple<QString, bool, bool>>() };
    int m_lineIterator { -1 };
    static const QString StartSelectionTag;
    static const QString EndSelectionTag;
    static const QRegularExpression ColorRegularExpression;

public:
    FormatterLine();

    FormatterLine(int offset);

    ~FormatterLine();

    QString line() const noexcept { return m_line; }

    QString lineWithOffset() const noexcept { return m_lineWithOffset; }

    QString formattedLine(int tabSize = 1) noexcept;

    QString formattedLineWithSelection(int tabSize, int startSelectionPosition, int endSelectionPosition) noexcept;

    void fillLineWithOffset(int tabSize = 1) noexcept;

    void setLine(const QString& line) noexcept;

    void setOffset(int offset) noexcept { m_offset = offset; }

    int offset() const noexcept { return m_offset; }

    void addIndex(const QString& content, bool left, bool replace = false) noexcept;

    void addCustomIndex(int index, const QString& content, bool left, bool replace = false, bool toTop = false) noexcept;

    void changeContentInLastIndex(const QString& content) noexcept;

    void increaseLineIterator(QChar character) noexcept;

    void increaseLineIteratorString(QString content) noexcept;

    bool isEmpty() noexcept;

    int lineIterator() const noexcept { return m_lineIterator; }

    bool containsCharacter(QChar character) const noexcept;

    void removeLastCharacter() noexcept;

private:
    std::tuple<QString,QString,QString> getContents(const QList<std::tuple<QString,bool,bool>>& items);

};

#endif // FORMATTERLINE_H
