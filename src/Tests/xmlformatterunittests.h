#ifndef XMLFORMATTERUNITTESTS_H
#define XMLFORMATTERUNITTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class XmlFormatterUnitTests : public QObject
{
    Q_OBJECT
public:
    explicit XmlFormatterUnitTests(QObject *parent = nullptr);

signals:

private slots:
    void xmlOnlyHeaderSilent();
    void xmlEmptyTagSilent();

};

#endif // XMLFORMATTERUNITTESTS_H
