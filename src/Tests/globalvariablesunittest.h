#ifndef GLOBALVARIABLESUNITTEST_H
#define GLOBALVARIABLESUNITTEST_H

#include <QObject>
#include <QtTest/QtTest>

class GlobalVariablesUnitTest : public QObject
{
    Q_OBJECT
public:
    explicit GlobalVariablesUnitTest(QObject *parent = nullptr);

signals:
private slots:
    void replaceGlobalVariables_notVariables();
    void replaceGlobalVariables_singleVariables();
    void replaceGlobalVariables_multipleVariables();
    void replaceGlobalVariables_anotherVariable();
    void replaceGlobalVariables_multipleTimesVariable();
    void parseLines_singlecorrectVariable();
    void parseLines_singleEmptyVariable();
    void parseLines_singleIncorrectVariable();
    void parseLines_dateTimeUtcNowPredefinedVariable();
    void parseLines_ttime24HoursNowPredefinedVariable();

};

#endif // GLOBALVARIABLESUNITTEST_H
