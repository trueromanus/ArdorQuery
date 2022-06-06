#include "outputformatter.h"

OutputFormatter::OutputFormatter()
{
}

QString OutputFormatter::format(const QString &data)
{
    //In the base class, simple return inputs
    //the actual formatting needs to be done in descendants
    return data;
}
