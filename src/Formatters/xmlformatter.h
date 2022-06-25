#ifndef XMLFORMATTER_H
#define XMLFORMATTER_H

#include "outputformatter.h"

class XmlFormatter : public OutputFormatter
{

    const QString m_xmlTab { "&nbsp;" };
    QString m_tagStart { "<" };
    QString m_tagEnd { ">" };
    QString m_attributeEqual { "=" };
    QString m_attributeDecorator { "\"" };
    QString m_closedTag { "/" };
    QString m_exclamationPoint { "!" };
    QString m_space { " " };
    QString m_newline { "\n" };
    QString m_caretBack { "\r" };
    int m_stackSize { -1 };
    QString m_result { "" };

public:
    XmlFormatter();

    QString format(const QString& data) override;
    void formatTagWithOffset(QString& tag);
    void formatTag(QString& tag);
    void setOffset(int tabSize = 4) noexcept;
};

#endif // XMLFORMATTER_H
