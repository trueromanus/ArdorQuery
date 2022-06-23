#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H

#include "outputformatter.h"

class XmlFormatter : public OutputFormatter
{

    const QString m_xmlTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };
    QString m_tagStart { "<" };
    QString m_tagEnd { ">" };
    QString m_attributeEqual { "=" };
    QString m_attributeDecorator { "\"" };
    QString m_closedTag { "/" };
    QString m_exclamationPoint { "!" };
    QString m_space { " " };
    QString m_newline { "\n" };
    QString m_caretBack { "\r" };

public:
    XmlFormatter();

    QString format(const QString& data) override;
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // XMLFORMATTER_H
