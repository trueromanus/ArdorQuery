#ifndef OUTPUTFORMATTER_H
#define OUTPUTFORMATTER_H

#include <QString>

class OutputFormatter
{
public:
    OutputFormatter();

    virtual QString format(const QString& data);
};

#endif // OUTPUTFORMATTER_H
