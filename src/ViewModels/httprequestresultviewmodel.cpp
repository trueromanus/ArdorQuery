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
    m_headers.clear();
    foreach (auto header, headers) {
        m_headers.append(header);
    }

    emit headersChanged();
}

void HttpRequestResultViewModel::setBody(const QString &body) noexcept
{
    m_bodyModel->setBody(body);

    emit bodyChanged();
    m_responseSize = body.size();
    emit responseSizeChanged();
}

QString HttpRequestResultViewModel::responseTime() const noexcept
{
    if (!m_hasResultTime) return " - ";

    QTime time(0,0,0,0);
    time = time.addMSecs(m_elapsedTime);

    QString result;
    if (time.minute() > 0) result.append(QString::number(time.minute()) + " mins ");
    if (time.second() > 0) result.append(QString::number(time.second()) + " secs ");
    if (time.msec() > 0) result.append(QString::number(time.msec()) + " msecs ");

    return result;
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
    m_elapsedTimer = nullptr;
    m_hasResultTime = false;
    m_responseSize = 0;
    emit responseTimeChanged();
    emit responseSizeChanged();
}

void HttpRequestResultViewModel::trackRequestTime() noexcept
{
    m_elapsedTimer = new QElapsedTimer();
    m_elapsedTimer->start();
    emit responseTimeChanged();
}

void HttpRequestResultViewModel::untrackRequestTime() noexcept
{
    m_hasResultTime = true;
    m_elapsedTime = m_elapsedTimer->elapsed();
    m_elapsedTimer = nullptr;

    emit responseTimeChanged();
}
