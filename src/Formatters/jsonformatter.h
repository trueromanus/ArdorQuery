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
    const QString m_jsonTab { "    " };

public:
    JsonFormatter();

    QString format(const QString& data);
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // JSONFORMATTER_H
