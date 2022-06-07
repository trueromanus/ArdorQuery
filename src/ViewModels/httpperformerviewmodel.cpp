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

#include <QUrlQuery>
#include <QUuid>
#include <QFile>
#include <QMimeDatabase>
#include <QFileInfo>
#include "httpperformerviewmodel.h"
#include "../globalconstants.h"

HttpPerformerViewModel::HttpPerformerViewModel(QObject *parent)
    : QObject{parent}
{
    connect(m_networkManager.get(), &QNetworkAccessManager::finished, this, &HttpPerformerViewModel::requestFinished);

    m_rawHeaders->insert("accept", "Accept");
    m_rawHeaders->insert("accept-charset", "Accept-Charset");
    m_rawHeaders->insert("accept-encoding", "Accept-Encoding");
    m_rawHeaders->insert("accept-language", "Accept-Language");
    m_rawHeaders->insert("accept-datetime", "Accept-Datetime");
    m_rawHeaders->insert("access-control-request-method", "Access-Control-Request-Method");
    m_rawHeaders->insert("access-control-request-headers", "Access-Control-Request-Headers");
    m_rawHeaders->insert("cache-control", "Cache-Control");
    m_rawHeaders->insert("connection", "Connection");
    m_rawHeaders->insert("if-range", "If-Range");
    m_rawHeaders->insert("if-unmodified-since", "If-Unmodified-Since");
    m_rawHeaders->insert("date", "Date");
    m_rawHeaders->insert("expect", "Expect");
    m_rawHeaders->insert("forwarded", "Forwarded");
    m_rawHeaders->insert("from", "From");
    m_rawHeaders->insert("host", "Host");
    m_rawHeaders->insert("mandatory", "Mandatory");
    m_rawHeaders->insert("max-forwards", "Max-Forwards");
    m_rawHeaders->insert("http2-settings", "HTTP2-Settings");
    m_rawHeaders->insert("origin", "Origin");
    m_rawHeaders->insert("pragma", "Pragma");
    m_rawHeaders->insert("prefer", "Prefer");
    m_rawHeaders->insert("proxy-authorization", "Proxy-Authorization");
    m_rawHeaders->insert("range", "Range");
    m_rawHeaders->insert("referer", "Referer");
    m_rawHeaders->insert("trailer", "Trailer");
    m_rawHeaders->insert("transfer-encoding", "Transfer-Encoding");
    m_rawHeaders->insert("referer", "Referer");
    m_rawHeaders->insert("upgrade", "Upgrade");
    m_rawHeaders->insert("via", "Via");
    m_rawHeaders->insert("warning", "Warning");
}

void HttpPerformerViewModel::setHttpRequest(const HttpRequestViewModel *viewModel) noexcept
{
    if (m_httpRequest == viewModel) return;

    m_httpRequest = const_cast<HttpRequestViewModel*>(viewModel);
    emit httpRequestChanged();
}

void HttpPerformerViewModel::setHttpRequestResult(const HttpRequestResultViewModel *httpRequestResult) noexcept
{
    if (m_httpRequestResult == httpRequestResult) return;

    m_httpRequestResult = const_cast<HttpRequestResultViewModel*>(httpRequestResult);
    emit httpRequestResultChanged();
}

void HttpPerformerViewModel::cancelRequest()
{
    //TODO: cancel request
}

void HttpPerformerViewModel::performRequest()
{
    if (m_httpRequest == nullptr) return;
    if (m_httpRequestResult == nullptr) return;

    auto url = m_httpRequest->getUrl();
    if (url.isEmpty()) {
        //TODO: push error message for user
        return;
    }

    auto requestUrl = QUrl(url);
    if(!requestUrl.isValid()) {
        //TODO: push error message for user
        return;
    }

    m_httpRequestResult->reset();

    QNetworkRequest request(requestUrl);

    adjustHeaders(request);

    auto method = m_httpRequest->getMethod().toLower();
    if (method == "get") {
        auto getReply = m_networkManager->get(request);
        startTrackRequest(getReply);
        return;
    }
    if (method == "delete") {
        auto deleteReply = m_networkManager->deleteResource(request);
        startTrackRequest(deleteReply);
        return;
    }

    auto body = m_httpRequest->getBody();
    auto forms = m_httpRequest->getFormParameters();
    auto files = m_httpRequest->getFileParameters();
    if (body.isEmpty() && forms.isEmpty() && files.isEmpty()) {
        emit pushErrorMessage("Request perform", "You need specified body, form or files");
        return;
    }

    if (!body.isEmpty() && !forms.isEmpty()) {
        emit pushErrorMessage("Request perform", "You have filled in both the body and the fields of the form, you need to leave only one of them");
        return;
    }

    if (method == "post") {
        QNetworkReply* postReply = nullptr;
        if (!body.isEmpty()) {
            postReply = m_networkManager->post(request, body.toUtf8());
        }

        if (!forms.isEmpty() && files.isEmpty()) {
            postReply = m_networkManager->post(request, setupSimpleForm(std::move(forms)));
        }
        if (!files.isEmpty() && !forms.isEmpty()) {
            postReply = m_networkManager->post(request, setupMultiPartForm(std::move(files), std::move(forms)));
        }
        if (postReply != nullptr) startTrackRequest(postReply);
    }
    if (method == "put") {
        QNetworkReply* putReply = nullptr;
        if (!body.isEmpty()) putReply = m_networkManager->put(request, body.toUtf8());
        if (!forms.isEmpty()) putReply = m_networkManager->put(request, setupSimpleForm(std::move(forms)));

        if (putReply != nullptr) startTrackRequest(putReply);
    }
}

QByteArray HttpPerformerViewModel::setupSimpleForm(QStringList&& parameters)
{
    QUrlQuery postData;

    foreach (auto parameter, parameters) {
        auto pair = parameter.replace("form ", "", Qt::CaseInsensitive);

        auto parts = pair.split("=");
        if (parts.count() < 2) continue;

        postData.addQueryItem(parts[0], parts[1]);
    }

    return postData.toString(QUrl::FullyEncoded).toUtf8();
}

QHttpMultiPart* HttpPerformerViewModel::setupMultiPartForm(QStringList&& files, QStringList &&parameters)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    foreach (auto parameter, parameters) {
        auto pair = parameter.replace(FormPrefix, "", Qt::CaseInsensitive);

        auto parts = pair.split("=");
        if (parts.count() < 2) continue;

        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + parts[0] + "\""));
        textPart.setBody(parts[1].toUtf8());

        multiPart->append(textPart);
    }

    QMimeDatabase mimeDatabase;

    foreach (auto fileItem, files) {
        auto pair = fileItem.replace(FilePrefix, "", Qt::CaseInsensitive);

        auto parts = pair.split("=");
        if (parts.count() < 2) continue;

        auto fileName = parts[1];

        if (!QFile::exists(fileName)) continue;

        QHttpPart filePart;

        auto type = mimeDatabase.mimeTypeForFile(fileName);
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(type.name()));

        QFileInfo fileInfo(fileName);
        auto onlyFileName = fileInfo.fileName();

        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + parts[0] + "\"; filename=\"" + onlyFileName + "\""));

        auto file = new QFile(parts[1]);
        file->open(QIODevice::ReadOnly);
        filePart.setBodyDevice(file);
        file->setParent(multiPart);

        multiPart->append(filePart);
    }

    return multiPart;
}

void HttpPerformerViewModel::adjustHeaders(QNetworkRequest &request) noexcept
{
    auto headers = m_httpRequest->getHeaders();
    foreach (auto header, headers) {
        auto parts = header.split(" ");
        if (parts.length() == 1) {
            fillHeader(request, header, "");
            continue;
        }
        fillHeader(request, parts[0], parts[1]);
    }
}

void HttpPerformerViewModel::fillHeader(QNetworkRequest &request, const QString &name, const QString &value) noexcept
{
    auto lowerName = name.toLower();

    if (lowerName == "content-type") {
        request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, QVariant(value));
        return;
    }
    if (lowerName == "content-encoding") {
        request.setRawHeader("Content-Encoding", value.toUtf8());
        return;
    }
    if (lowerName == "content-length") {
        request.setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, QVariant(value));
        return;
    }
    if (lowerName == "cookie") {
        request.setHeader(QNetworkRequest::KnownHeaders::CookieHeader, QVariant(value));
        return;
    }
    if (lowerName == "if-match") {
        request.setHeader(QNetworkRequest::KnownHeaders::IfMatchHeader, QVariant(value));
        return;
    }
    if (lowerName == "if-none-match") {
        request.setHeader(QNetworkRequest::KnownHeaders::IfNoneMatchHeader, QVariant(value));
        return;
    }
    if (lowerName == "if-modified-since") {
        request.setHeader(QNetworkRequest::KnownHeaders::IfModifiedSinceHeader, QVariant(value));
        return;
    }
    if (lowerName == "user-agent") {
        request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, QVariant(value));
        return;
    }

    if (m_rawHeaders->contains(lowerName)) {
        auto rawHeader = m_rawHeaders->value(lowerName);
        request.setRawHeader(rawHeader.toUtf8(), value.toUtf8());
    }
}

void HttpPerformerViewModel::startTrackRequest(QNetworkReply *reply) noexcept
{
    if (m_httpRequestResult->isRunning()) return; // already runned

    m_httpRequestResult->trackRequestTime();

    auto uuid = QUuid::createUuid().toString();

    reply->setProperty("id", uuid);

    m_runningRequests->insert(uuid, m_httpRequestResult);
}

void HttpPerformerViewModel::requestFinished(QNetworkReply *reply)
{
    auto id = reply->property("id").toString();
    if (!m_runningRequests->contains(id)) return;

    auto result = m_runningRequests->value(id);
    result->untrackRequestTime();
    m_runningRequests->remove(id);

    if (reply->error() != QNetworkReply::NoError) {
        result->setNetworkError(reply->errorString());
        return;
    }

    result->setNetworkError("");

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) result->setStatusCode(status_code.toInt());

    QStringList responseHeaders;
    auto headers = reply->rawHeaderPairs();
    foreach (auto header, headers) {
        auto name = std::get<0>(header);
        auto value = std::get<1>(header);
        responseHeaders.append(name + ": " + value);
    }
    result->setHeaders(responseHeaders);

    result->setBody(reply->readAll());
}
