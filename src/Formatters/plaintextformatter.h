#ifndef PLAINTEXTFORMATTER_H
#define PLAINTEXTFORMATTER_H

#include "outputformatter.h"

class PlainTextFormatter : public OutputFormatter
{

private:
    const QString m_newline { "\n" };
    const QString m_caretBack { "\r" };

public:
    PlainTextFormatter();

    QString format(const QString& data) override;
};

#endif // PLAINTEXTFORMATTER_H
