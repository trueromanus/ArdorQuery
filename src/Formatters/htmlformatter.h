#ifndef HTMLFORMATTER_H
#define HTMLFORMATTER_H

#include <QSet>
#include "outputformatter.h"

class HtmlFormatter : public OutputFormatter
{
private:
    QString m_tag { "<>" };
    QString m_attributeEqual { "=" };
    QString m_attributeDecorator { "\"" };
    QString m_closedTag { "/" };
    QString m_doctype { "!doctype" };
    QString m_comment { "!--" };
    QString m_space { " " };
    QString m_newline { "\n" };
    QString m_caretBack { "\r" };
    QString m_scriptTag { "script" };
    QSet<QString> m_selfClosingTags { QSet<QString>() };
    const QString m_htmlTab { "&nbsp;&nbsp;&nbsp;&nbsp;" };

public:
    HtmlFormatter();

    QString format(const QString& data) override;
    void setOffset(int stackSize, QString& target, bool newLine = false) noexcept;
};

#endif // HTMLFORMATTER_H
