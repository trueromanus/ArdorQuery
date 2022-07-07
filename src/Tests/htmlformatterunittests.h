#ifndef HTMLFORMATTERUNITTESTS_H
#define HTMLFORMATTERUNITTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class HtmlFormatterUnitTests : public QObject
{
    Q_OBJECT
public:
    explicit HtmlFormatterUnitTests(QObject *parent = nullptr);

private slots:
    void htmlLowerHeader();
    void htmlUpperHeader();
    void emptyFullTag();
};

#endif // HTMLFORMATTERUNITTESTS_H
