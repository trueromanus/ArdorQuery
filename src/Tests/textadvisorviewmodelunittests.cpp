#include "textadvisorviewmodelunittests.h"
#include "../ViewModels/textadvisorviewmodel.h"

TextAdvisorViewModelUnitTests::TextAdvisorViewModelUnitTests(QObject *parent)
    : QObject{parent}
{

}

void TextAdvisorViewModelUnitTests::customXHeader()
{
    auto viewModel = new TextAdvisorViewModel(this);
    auto upperCustomHeader = viewModel->isContainsHeader("X-CUSTOM-HEADER customValue1");
    auto lowerCustomHeader = viewModel->isContainsHeader("x-custom-header customValue2");
    auto upperSingleCustomHeader = viewModel->isContainsHeader("X-CUSTOM-HEADER");
    auto loweSingleCustomHeader = viewModel->isContainsHeader("x-custom-header");
    QCOMPARE(upperCustomHeader, true);
    QCOMPARE(lowerCustomHeader, true);
    QCOMPARE(upperSingleCustomHeader, true);
    QCOMPARE(loweSingleCustomHeader, true);
}

void TextAdvisorViewModelUnitTests::falseCustomXHeader()
{
    auto viewModel = new TextAdvisorViewModel(this);
    auto upperCustomHeader = viewModel->isContainsHeader("CUSTOM-HEADER customValue1");
    auto lowerCustomHeader = viewModel->isContainsHeader("custom-header customValue2");
    QCOMPARE(upperCustomHeader, false);
    QCOMPARE(lowerCustomHeader, false);
}

void TextAdvisorViewModelUnitTests::customHeader()
{
    auto viewModel = new TextAdvisorViewModel(this);
    auto lowerCustomHeader = viewModel->isContainsHeader("header X-CUSTOM-HEADER customValue1");
    auto upperCustomHeader = viewModel->isContainsHeader("HEADER x-custom-header customValue2");
    auto differentCustomHeader = viewModel->isContainsHeader("HeAdEr x-custom-header customValue2");
    QCOMPARE(upperCustomHeader, true);
    QCOMPARE(lowerCustomHeader, true);
    QCOMPARE(differentCustomHeader, true);
}

void TextAdvisorViewModelUnitTests::failCustomHeader()
{
    auto viewModel = new TextAdvisorViewModel(this);
    auto lowerCustomHeader = viewModel->isContainsHeader("headerv");
    auto upperCustomHeader = viewModel->isContainsHeader("HEADER1");
    auto differentCustomHeader = viewModel->isContainsHeader("HeAdEra");
    QCOMPARE(upperCustomHeader, false);
    QCOMPARE(lowerCustomHeader, false);
    QCOMPARE(differentCustomHeader, false);
}
