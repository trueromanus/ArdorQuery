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
#include <QPainter>
#include <QFile>
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
    m_displayHeaders.clear();
    foreach (auto header, headers) {
        m_headers.append(header);
        auto spaceIndex = header.indexOf(" ");
        auto mid = StartHeaderTag + header.mid(0, spaceIndex) + EndHeaderTag;
        m_displayHeaders.append(mid + header.mid(spaceIndex + 1));
    }

    emit headersChanged();
}

void HttpRequestResultViewModel::setBody(const QByteArray &body) noexcept
{
    if (body.isEmpty()) {
        m_bodyModel->setBody("", "");
        m_isFormatting = false;
        m_showImage = false;
        m_showDownloadFile = false;
        emit isFormattingChanged();
        return;
    }

    auto outputFormat = m_outputFormat;
    m_actualFormat.clear();
    if (m_outputFormat == OutputFormatAuto) {
        outputFormat = getFormatFromContentType();
        m_actualFormat = outputFormat;
    }

    m_isFormatting = !outputFormat.isEmpty() && outputFormat != OutputFormatPlainText;
    m_showImage = outputFormat == OutputFormatImage;
    m_showDownloadFile = outputFormat == OutputNeedDownloaded;

    m_responseSize = body.size();
    if (m_responseSize > 0) {
        setResponseReadableSize(getReadableSize(m_responseSize));
    } else {
        setResponseReadableSize("");
    }
    emit showImageChanged();
    emit showDownloadFileChanged();
    emit responseSizeChanged();
    emit isFormattingChanged();
    emit actualFormatChanged();

    m_bodyModel->setBody(body, outputFormat);
}

void HttpRequestResultViewModel::reformatting() noexcept
{
    auto outputFormat = m_outputFormat;
    if (outputFormat == OutputFormatAuto) {
        outputFormat = getFormatFromContentType();
        m_actualFormat = outputFormat;
    }

    m_isFormatting = !outputFormat.isEmpty() && outputFormat != OutputFormatPlainText;
    m_showImage = outputFormat == OutputFormatImage;
    m_showDownloadFile = outputFormat == OutputNeedDownloaded;

    m_bodyModel->reformatBody(outputFormat);

    emit isFormattingChanged();
    emit showImageChanged();
    emit showDownloadFileChanged();
    emit actualFormatChanged();
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
    emit hasErrorChanged();
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
    m_responseReadableSize.clear();
    emit responseTimeChanged();
    emit responseSizeChanged();
    emit responseReadableSizeChanged();
}

void HttpRequestResultViewModel::trackRequestTime() noexcept
{
    m_isRunning = true;
    m_customErrorResult = false;
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
    if (m_customErrorResult || !m_networkError.isEmpty()) return "cancel";
    if (m_hasResultTime) return "checked";

    return "emptybox";
}

void HttpRequestResultViewModel::setOutputFormat(const QString &outputFormat) noexcept
{
    if (m_outputFormat == outputFormat) return;

    m_outputFormat = outputFormat;
    emit outputFormatChanged();
}

void HttpRequestResultViewModel::generateImage(const QStringList& fields, const QString& path, bool saveToClipboard) noexcept
{
    auto headers = getHeaderLines();
    int countOfLines = 6 + headers.count() + fields.count();
    int lineHeight = 20;
    int currentLine = 0;
    QImage image(800, countOfLines * lineHeight, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);

    paintText(painter, image, currentLine, lineHeight, "Query", true);

    foreach (auto field, fields) {
        paintText(painter, image, currentLine, lineHeight, field, false);
    }

    paintText(painter, image, currentLine, lineHeight, "Response data", true);
    paintText(painter, image, currentLine, lineHeight, "Status: " + displayStatusCode() + networkError(), false);
    paintText(painter, image, currentLine, lineHeight, "Response time: " + responseTime(), false);
    paintText(painter, image, currentLine, lineHeight, "Response size: " + responseReadableSize() + " " + responseSize(), false);

    paintText(painter, image, currentLine, lineHeight, "Headers", true);

    foreach (auto header, headers) {
        paintText(painter, image, currentLine, lineHeight, header, false);
    }

    if (saveToClipboard) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setImage(image);
    } else {
        auto saveResult = image.save(path);
        if (!saveResult) emit errorSavingGeneratedFile();
    }
}

void HttpRequestResultViewModel::setCustomErrorResult(bool hasErrors, const QString& errorMessage)
{
    m_customErrorResult = hasErrors;
    if (!errorMessage.isEmpty()) m_networkError = errorMessage;
    emit hasErrorChanged();
    emit networkErrorChanged();
    emit displayStatusChanged();
}

void HttpRequestResultViewModel::setPostScript(const QString &script)
{
    m_postScript = script;
}

void HttpRequestResultViewModel::clearPostScript()
{
    m_postScript = "";
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

void HttpRequestResultViewModel::saveBodyToFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return;

    auto content = m_bodyModel->getFullBodyArray();
    file.write(content);

    file.close();
}

void HttpRequestResultViewModel::reformatBody()
{
    if (m_outputFormat != OutputFormatAuto) {
        m_actualFormat.clear();
        emit actualFormatChanged();
    }
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

QString HttpRequestResultViewModel::getFormatFromContentType() noexcept
{
    QString contentTypeHeader = "";
    QString contentDisposition = "";
    foreach (auto header, m_headers) {
        if (header.startsWith("content-type ", Qt::CaseInsensitive)) {
            contentTypeHeader = header.toLower();
            continue;
        }
        if (header.startsWith("content-disposition ", Qt::CaseInsensitive)) {
            contentDisposition = header.toLower();
            continue;
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

    if (!contentDisposition.isEmpty()) {
        m_defaultDownloadFile = "";
        auto value = contentDisposition
            .replace(StartHeaderTag, "")
            .replace(EndHeaderTag, "")
            .replace("content-disposition: ", "");
        if (!value.contains("attachment", Qt::CaseInsensitive)) return "";
        auto parts = value.split(";");
        foreach (auto part, parts) {
            auto nameParts = part.split("=");
            if (nameParts.length() != 2) continue;

            if (nameParts[0].trimmed() != "filename") continue;
            m_defaultDownloadFile = nameParts[1];
        }
        return OutputNeedDownloaded;
    }

    //all non text unsupported types we can only download
    if (contentTypeHeader.contains("application/") ||
        contentTypeHeader.contains("audio/") ||
        contentTypeHeader.contains("video/") ||
        contentTypeHeader.contains("font/")) {
        return OutputNeedDownloaded;
    }

    if (contentTypeHeader.contains("text/")) return OutputFormatPlainText;

    return "";
}

void HttpRequestResultViewModel::setBoldTextToPainter(QPainter &painter) noexcept
{
    auto font = painter.font();
    font.setBold(true);
    painter.setFont(font);
}

void HttpRequestResultViewModel::setNormalTextToPainter(QPainter &painter) noexcept
{
    auto font = painter.font();
    font.setBold(false);
    painter.setFont(font);
}

void HttpRequestResultViewModel::paintText(QPainter &painter, const QImage &image, int &currentLine, int &lineHeight, const QString &text, bool bold) noexcept
{
    if (bold && !painter.font().bold()) setBoldTextToPainter(painter);
    if (!bold && painter.font().bold()) setNormalTextToPainter(painter);

    painter.drawText(3, currentLine * lineHeight, image.rect().width() - 3, lineHeight, Qt::AlignVCenter, text);
    currentLine++;
}

QStringList HttpRequestResultViewModel::getHeaderLines()
{
    const uint64_t lineCount = 110;
    QStringList lines;
    foreach (auto header, m_headers) {
        auto clearedHeader = QString(header)
            .replace("\n", "")
            .replace("\r", "");
        if (clearedHeader.size() > lineCount) {
            int position = 0;
            QString content;
            bool isFull = false;
            do {
                content.clear();
                auto content = clearedHeader.mid(position, lineCount);
                if (content.isEmpty()) break;
                lines.append(content);
                position += lineCount;
                isFull = content.size() == lineCount;
            } while (isFull);
        } else {
            lines.append(clearedHeader);
        }
    }

    return lines;
}

