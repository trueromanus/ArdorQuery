#ifndef TEXTADVISORVIEWMODELUNITTESTS_H
#define TEXTADVISORVIEWMODELUNITTESTS_H

#include <QObject>
#include <QtTest/QtTest>

class TextAdvisorViewModelUnitTests : public QObject
{
    Q_OBJECT
public:
    explicit TextAdvisorViewModelUnitTests(QObject *parent = nullptr);

private slots:
    void customXHeader();
    void falseCustomXHeader();
    void customHeader();
    void failCustomHeader();

};

#endif // TEXTADVISORVIEWMODELUNITTESTS_H
