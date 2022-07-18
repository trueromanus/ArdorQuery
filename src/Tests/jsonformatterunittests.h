#ifndef JSONFORMATTERUNITTESTS_H
#define JSONFORMATTERUNITTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class JsonFormatterUnitTests : public QObject
{
    Q_OBJECT
public:
    explicit JsonFormatterUnitTests(QObject *parent = nullptr);

private slots:
    void stringOnly();
    void objectOnly();
    void objectOnlyMoreProperties();
    void arrayOnly();
    void arrayInObject();
    void objectWithNegativeDigit();
    void objectWithHexDigit();
};

#endif // JSONFORMATTERUNITTESTS_H
