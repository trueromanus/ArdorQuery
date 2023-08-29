#include "postscriptresponsemodel.h"

PostScriptResponseModel::PostScriptResponseModel(QObject *parent)
    : QObject{parent}
{

}

void PostScriptResponseModel::setHeaders(QStringList headers) noexcept
{
    if (m_headers == headers) return;

    m_headers = headers;
    emit headersChanged();
}

void PostScriptResponseModel::setStatusCode(int statusCode) noexcept
{
    if (m_statusCode == statusCode) return;

    m_statusCode = statusCode;
    emit statusCodeChanged();
}

void PostScriptResponseModel::setErrorMessage(const QString &errorMessage) noexcept
{
    if (m_errorMessage == errorMessage) return;

    m_errorMessage = errorMessage;
    emit errorMessageChanged();
}

void PostScriptResponseModel::setBodySize(int bodySize) noexcept
{
    if (m_bodySize == bodySize) return;

    m_bodySize = bodySize;
    emit bodySizeChanged();
}

void PostScriptResponseModel::setRoute(const QString &route) noexcept
{
    if (m_route == route) return;

    m_route = route;
    emit routeChanged();

}
