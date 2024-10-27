#include "postscriptresultmodel.h"

PostScriptResultModel::PostScriptResultModel(QObject *parent)
    : QObject{parent}
{

}

void PostScriptResultModel::setHasErrors(bool hasErrors) noexcept
{
    if (m_hasErrors == hasErrors) return;

    m_hasErrors = hasErrors;
    emit hasErrorsChanged();
}

void PostScriptResultModel::setErrorMessage(const QString &errorMessage) noexcept
{
    if (m_errorMessage == errorMessage) return;

    m_errorMessage = errorMessage;
    emit errorMessageChanged();
}

void PostScriptResultModel::saveToFile(const QString &fileName, bool openAfterSave)
{
    emit needSaveToFile(fileName, openAfterSave);
}
