#ifndef FORMATTERFACTORY_H
#define FORMATTERFACTORY_H

#include "outputformatter.h"
#include <QMap>
#include <QScopedPointer>

class FormatterFactory
{
private:
    QScopedPointer<QMap<QString, OutputFormatter*>> m_instanceCache { new QMap<QString, OutputFormatter*>() };
    QScopedPointer<OutputFormatter> m_nullableFormatter { new OutputFormatter() };

public:
    FormatterFactory();

    OutputFormatter* getFormatter(const QString& formatter);

};

#endif // FORMATTERFACTORY_H
