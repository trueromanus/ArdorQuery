/*
    ArdorQuery http tester
    Copyright (C) 2022 Roman Vladimirov
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "httprequestresultviewmodel.h"

HttpRequestResultViewModel::HttpRequestResultViewModel(QObject *parent)
    : QObject{parent}
{
    m_startResultTime = QDateTime::currentDateTimeUtc();
}

void HttpRequestResultViewModel::setStatusCode(const int statusCode) noexcept
{
    if (m_statusCode == statusCode) return;

    m_statusCode = statusCode;
    emit statusCodeChanged();
    emit displayStatusCodeChanged();
}

void HttpRequestResultViewModel::setHeaders(const QStringList &headers) noexcept
{
    m_headers->clear();
    m_headers->append(headers);

    emit headersChanged();
}

void HttpRequestResultViewModel::setBody(const QString &body) noexcept
{
    if (m_body == body) return;

    m_body = body;
    emit bodyChanged();
    m_responseSize = m_body.length();
    emit responseSizeChanged();
}

QString HttpRequestResultViewModel::responseTime() const noexcept
{
    auto difference = m_startResultTime.msecsTo(m_endResultTime);
    QTime time;
    time = time.addMSecs(static_cast<int>(difference));

    if (time.minute() < 0) return " - ";

    return QString::number(time.minute()) + ":" + QString::number(time.second()) + ":" + QString::number(time.msec());
}

void HttpRequestResultViewModel::setNetworkError(const QString &networkError) noexcept
{
    if (m_networkError == networkError) return;

    m_networkError = networkError;
    emit networkErrorChanged();
}

void HttpRequestResultViewModel::reset() noexcept
{
    setStatusCode(0);
    QStringList empty;
    setHeaders(empty);
    setBody("");
    m_startResultTime = QDateTime::currentDateTimeUtc();
    m_endResultTime =  QDateTime::currentDateTimeUtc();
    m_responseSize = 0;
    emit responseTimeChanged();
    emit responseSizeChanged();
}

void HttpRequestResultViewModel::trackRequestTime() noexcept
{
    m_startResultTime = QDateTime::currentDateTimeUtc();
}

void HttpRequestResultViewModel::untrackRequestTime() noexcept
{
    m_endResultTime = QDateTime::currentDateTimeUtc();
    emit responseTimeChanged();
}
