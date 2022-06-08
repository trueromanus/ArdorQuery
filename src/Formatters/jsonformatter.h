#ifndef JSONFORMATTER_H
#define JSONFORMATTER_H

#include "outputformatter.h"

class JsonFormatter: public OutputFormatter
{
private:
    QString m_array { "[]" };
    QString m_object { "{}" };
    QString m_string { "\"" };
    QString m_comma { "," };
    QString m_space { " " };
    QString m_colon { ":" };
    QString m_backslash { "\\" };
    QString m_reverse { "r" };
    QString m_newline { "n" };
    QString m_unicode { "u" };
    const QString m_jsonTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };

public:
    JsonFormatter();

    QString format(const QString& data) override;
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // JSONFORMATTER_H
