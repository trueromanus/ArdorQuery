#ifndef PLAINTEXTFORMATTER_H
#define PLAINTEXTFORMATTER_H

#include <QMap>
#include "outputformatter.h"
#include "formatterline.h"

class PlainTextFormatter : public OutputFormatter
{

private:
    const QString m_newline { "\n" };
    const QString m_caretBack { "\r" };

public:
    PlainTextFormatter();

    QString format(const QString& data) override;
    QMap<int, FormatterLine*> silentFormat(const QString &data) override;
    int silentFormatTab() override { return 1; }
};

#endif // PLAINTEXTFORMATTER_H
