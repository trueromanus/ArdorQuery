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

#include <QClipboard>
#include <QGuiApplication>
#include "httprequestresultviewmodel.h"

HttpRequestResultViewModel::HttpRequestResultViewModel(QObject *parent)
    : QObject{parent}
{
    m_sizes.append("B");
    m_sizes.append("KB");
    m_sizes.append("MB");
    m_sizes.append("GB");
    m_sizes.append("TB");
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

void HttpRequestResultViewModel::setBody(const QByteArray &body) noexcept
{
    if (body.isEmpty()) {
        m_bodyModel->setBody("", "");
        m_isFormatting = false;
        m_showImage = false;
        emit isFormattingChanged();
        return;
    }

    auto outputFormat = m_outputFormat;
    m_actualFormat.clear();
    if (m_outputFormat == OutputFormatAuto) {
        outputFormat = getFormatFromContentType();
        m_actualFormat = outputFormat;
    }

    m_isFormatting = !outputFormat.isEmpty();
    m_showImage = outputFormat == OutputFormatImage;

    m_bodyModel->setBody(body, outputFormat);

    m_responseSize = body.size();
    if (m_responseSize > 0) {
        setResponseReadableSize(getReadableSize(m_responseSize));
    } else {
        setResponseReadableSize("");
    }
    emit responseSizeChanged();
    emit isFormattingChanged();
    emit showImageChanged();
    emit actualFormatChanged();
}

void HttpRequestResultViewModel::reformatting() noexcept
{
    auto outputFormat = m_outputFormat;
    if (outputFormat == OutputFormatAuto) outputFormat = getFormatFromContentType();

    m_isFormatting = !outputFormat.isEmpty();
    m_showImage = outputFormat == OutputFormatImage;

    m_bodyModel->reformatBody(outputFormat);

    emit isFormattingChanged();
    emit showImageChanged();
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

void HttpRequestResultViewModel::setResponseReadableSize(const QString &responseReadableSize) noexcept
{
    if (m_responseReadableSize == responseReadableSize) return;

    m_responseReadableSize = responseReadableSize;
    emit responseReadableSizeChanged();
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
    m_isRunning = true;
    m_elapsedTimer = new QElapsedTimer();
    m_elapsedTimer->start();
    emit responseTimeChanged();
    emit displayStatusChanged();
    emit isRunningChanged();
}

void HttpRequestResultViewModel::untrackRequestTime() noexcept
{
    m_isRunning = false;
    m_hasResultTime = true;
    m_elapsedTime = m_elapsedTimer->elapsed();
    m_elapsedTimer = nullptr;

    emit responseTimeChanged();
    emit displayStatusChanged();
    emit isRunningChanged();
}

QString HttpRequestResultViewModel::displayStatus() const noexcept
{
    if (m_isRunning) return "pending";
    if (m_hasResultTime) return "checked";

    return "emptybox";
}

void HttpRequestResultViewModel::setOutputFormat(const QString &outputFormat) noexcept
{
    if (m_outputFormat == outputFormat) return;

    m_outputFormat = outputFormat;
    emit outputFormatChanged();
}

void HttpRequestResultViewModel::copyHeadersToClipboard()
{
    if (m_headers.isEmpty()) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(m_headers.join("\n"));
}

void HttpRequestResultViewModel::copyBodyToClipboard()
{
    if (!m_bodyModel->isHasBody()) return;

    QClipboard *clipboard = QGuiApplication::clipboard();

    auto format = m_outputFormat;
    if (format == OutputFormatAuto) format = getFormatFromContentType();

    if (format == OutputFormatImage) {
        QImage image;
        image.loadFromData(m_bodyModel->getFullBodyArray());
        clipboard->setImage(image);
    } else {
        clipboard->setText(m_bodyModel->getFullBody());
    }
}

void HttpRequestResultViewModel::reformatBody()
{
    reformatting();
}

QString HttpRequestResultViewModel::getReadableSize(uint64_t size) const noexcept
{
    int order = 0;
    while (size >= 1024 && order < 4) {
        order++;
        size = size / 1024;
    }

    auto stringSize = QString::number(size);
    QString result;
    result.append(stringSize);
    result.append(" ");
    result.append(m_sizes[order]);
    return result;
}

QString HttpRequestResultViewModel::getFormatFromContentType() const noexcept
{
    QString contentTypeHeader = "";
    foreach (auto header, m_headers) {
        if (header.contains("content-type:", Qt::CaseInsensitive)) {
            contentTypeHeader = header.toLower();
            break;
        }
    }
    if (contentTypeHeader.isEmpty()) return "";

    auto isJson = contentTypeHeader.contains("application/json");
    if (isJson) return OutputFormatJson;

    auto isXml = contentTypeHeader.contains("application/xml") || contentTypeHeader.contains("application/xhtml+xml");
    if (isXml) return OutputFormatXml;

    auto isHtml = contentTypeHeader.contains("text/html");
    if (isHtml) return OutputFormatHtml;

    if (contentTypeHeader.contains("image/jpeg") || contentTypeHeader.contains("image/png") ||
        contentTypeHeader.contains("image/svg+xml") || contentTypeHeader.contains("image/webp") ||
        contentTypeHeader.contains("image/gif")) {
        return OutputFormatImage;
    }

    return "";
}
