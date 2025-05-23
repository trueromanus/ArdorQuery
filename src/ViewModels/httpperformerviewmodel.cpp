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
#include <QNetworkRequest>
#include <QJSEngine>
#include "../Models/postscriptresultmodel.h"
#include "../Models/postscriptresponsemodel.h"
#include "httpperformerviewmodel.h"
#include "../globalconstants.h"

HttpPerformerViewModel::HttpPerformerViewModel(QObject *parent)
    : QObject{parent}
{
    connect(m_networkManager.get(), &QNetworkAccessManager::finished, this, &HttpPerformerViewModel::requestFinished);

    m_rawHeaders->insert("authorization", "Authorization");
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

void HttpPerformerViewModel::setup(QSharedPointer<QList<HttpRequestModel *> > requests)
{
    m_requests = requests;
}

void HttpPerformerViewModel::cancelRequest()
{
    m_orderedRequests.clear();
    m_orderedRequestsIndex = 0;

    foreach (auto item, m_runningRequests.values()) {
        if (!item->isRunning()) continue; // in case if it already not run

        auto reply = item->currentReply();
        if (reply == nullptr) continue; // same case as above

        reply->abort();
        item->setCurrentReply(nullptr);
    }
}

void HttpPerformerViewModel::performOneRequest(HttpRequestModel *request)
{
    if (m_runningRequests.contains(request->requestId().toString())) return;
    if (!m_orderedRequests.isEmpty()) return;

    m_sessionObject = nullptr;
    m_countFinishedRequests = 0;
    m_countRequests = 0;
    m_countErrorRequests = 0;
    emit countFinishedRequestsChanged();
    emit countRequestsChanged();
    emit countErrorRequestsChanged();

    performSingleRequest(request);
}

void HttpPerformerViewModel::performAllRequest()
{
    if (!m_runningRequests.isEmpty() || !m_orderedRequests.isEmpty() || m_orderedRequestsIndex > 0) return;

    m_orderedRequests.clear();
    m_sessionObject = new PostScriptSessionModel(this);
    m_orderedRequestsIndex = 0;
    m_countErrorRequests = 0;
    m_countFinishedRequests = 0;
    emit countErrorRequestsChanged();
    emit countFinishedRequestsChanged();

    QSet<int> orders;
    foreach (auto request, *m_requests) {
        auto order = request->requestModel()->getOrder();
        orders.insert(order);
    }
    QList<int> orderList(orders.begin(), orders.end());
    std::sort(orderList.begin(), orderList.end(), [](int left, int right) { return left < right; });

    foreach (auto request, *m_requests) {
        auto order = request->requestModel()->getOrder();
        auto index = orderList.indexOf(order);
        m_orderedRequests.insert(index, request->requestId());
    }

    if (m_orderedRequests.isEmpty()) return;

    auto firstRequests = m_orderedRequests.values(m_orderedRequestsIndex);

    m_countRequests = 0;
    foreach (auto request, *m_requests) {
        auto id = request->requestId();
        if (firstRequests.contains(id) && performSingleRequest(request)) {
            m_countRequests = m_countRequests + 1;
        }
    }

    emit countRequestsChanged();
}

void HttpPerformerViewModel::performRequests(const QList<QUuid> &ids)
{
    foreach (auto request, *m_requests) {
        auto id = request->requestId();
        if (ids.contains(id) && performSingleRequest(request)) {
            m_countRequests = m_countRequests + 1;
        }
    }

    emit countRequestsChanged();
}

void HttpPerformerViewModel::setGlobalVariable(const GlobalVariablesListModel *globalVariable) noexcept
{
    if (m_globalVariable == globalVariable) return;

    m_globalVariable = const_cast<GlobalVariablesListModel*>( globalVariable );
    m_globalVariablePostScript = new GlobalVariablesPostScriptModel(this, m_globalVariable, m_requests);
    emit globalVariableChanged();
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

bool HttpPerformerViewModel::adjustHeaders(QNetworkRequest &request, const HttpRequestViewModel* model) noexcept
{
    QSet<QString> m_usedHeaders;
    auto headers = model->getHeaders();
    foreach (auto rawHeader, headers) {
        auto header = applyGlobalVariables(rawHeader);
        auto spaceIndex = header.indexOf(" ");
        if (spaceIndex == -1) {
            auto lowerHeader = header.toLower();
            if (m_usedHeaders.contains(lowerHeader)) {
                emit pushErrorMessage("Request perform", header + " header specified more then one time");
                return false;
            }
            m_usedHeaders.insert(lowerHeader);
            fillHeader(request, header, "");
            continue;
        }
        auto headerName = header.mid(0, spaceIndex);
        auto lowerHeaderName = headerName.toLower();
        if (m_usedHeaders.contains(lowerHeaderName)) {
            emit pushErrorMessage("Request perform", headerName + " header specified more then one time");
            return false;
        }
        m_usedHeaders.insert(lowerHeaderName);

        auto headerValue = header.mid(spaceIndex + 1);
        fillHeader(request, headerName, headerValue);
    }

    return true;
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
    } else {
        //custom header stays as it is
        request.setRawHeader(name.toUtf8(), value.toUtf8());
    }
}

void HttpPerformerViewModel::startTrackRequest(QNetworkReply *reply, const QUuid& id, HttpRequestResultViewModel* resultModel) noexcept
{
    if (resultModel->isRunning()) return; // already runned

    resultModel->trackRequestTime();
    resultModel->setCurrentReply(reply);

    reply->setProperty("id", id.toString());

    m_runningRequests.insert(id.toString(), resultModel);
}

void HttpPerformerViewModel::adjustOptions(QStringList options, QNetworkRequest &request)
{
    foreach (auto option, options) {
        option = applyGlobalVariables(option);
        auto loweredOption = option.trimmed().toLower();
        if (loweredOption == "noautoredirect") {
            request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
        }
        if (loweredOption == "autoredirect") {
            request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        }
        if (loweredOption == "weaksslcheck") {
            QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
            sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
            request.setSslConfiguration(sslConfiguration);
        }
        if (loweredOption == "noweaksslcheck") {
            QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
            sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfiguration);
        }
    }
}

bool HttpPerformerViewModel::performSingleRequest(HttpRequestModel *modelRequest)
{
    auto id = modelRequest->requestId();

    //if request already performing don't need make something
    if (m_runningRequests.contains(id.toString())) return false;

    auto resultModel = modelRequest->resultModel();
    auto requestModel = modelRequest->requestModel();
    resultModel->reset();

    auto url = requestModel->getUrl();
    url = applyGlobalVariables(url);
    auto requestUrl = QUrl(url);

    QNetworkRequest request(requestUrl);

    auto options = requestModel->getOptions();
    adjustOptions(options, request);

    auto headersValid = adjustHeaders(request, requestModel);
    if (!headersValid) return false;

    auto protocol = requestModel->getProtocol();
    protocol = applyGlobalVariables(protocol);
    if (protocol == "1.1") request.setAttribute(QNetworkRequest::Http2AllowedAttribute, false);

    auto postScript = requestModel->getPostScript();
    if (postScript.isEmpty()) {
        resultModel->clearPostScript();
    } else {
        resultModel->setPostScript(postScript);
    }

    auto timeout = requestModel->getTimeout();
    if (!timeout.isEmpty()) {
        timeout = applyGlobalVariables(timeout);
        auto timeoutInt = timeout.toInt();
        if (timeoutInt > 0) request.setTransferTimeout(timeoutInt);
    }

    auto method = requestModel->getMethod();
    method = applyGlobalVariables(method).toLower();
    if (method == "get") {
        auto getReply = m_networkManager->get(request);
        startTrackRequest(getReply, id, resultModel);
        return true;
    }
    if (method == "delete") {
        auto deleteReply = m_networkManager->deleteResource(request);
        startTrackRequest(deleteReply, id, resultModel);
        return true;
    }

    auto body = requestModel->getBody();
    auto forms = requestModel->getFormParameters();
    for (auto i = 0; i < forms.count(); i++) {
        auto item = forms[i];
        forms[i] = applyGlobalVariables(item);
    }
    auto files = requestModel->getFileParameters();
    for (auto i = 0; i < files.count(); i++) {
        auto fileItem = files[i];
        files[i] = applyGlobalVariables(fileItem);
    }

    auto isBodyAndFormEmpty = body.isEmpty() && forms.isEmpty() && files.empty();
    auto isSimpleForm = !forms.isEmpty() && files.isEmpty();
    auto isComplexForm = !files.isEmpty();
    auto isBodyFilled = !body.isEmpty();

    if (method == "post") {
        QNetworkReply* postReply = nullptr;
        if (isBodyAndFormEmpty) postReply = m_networkManager->post(request, "");
        if (isBodyFilled) postReply = m_networkManager->post(request, body.toUtf8());
        if (!isBodyFilled && isSimpleForm) postReply = m_networkManager->post(request, setupSimpleForm(std::move(forms)));
        if (!isBodyFilled && isComplexForm) postReply = m_networkManager->post(request, setupMultiPartForm(std::move(files), std::move(forms)));
        if (postReply != nullptr) {
            startTrackRequest(postReply, id, resultModel);
            return true;
        }
    }
    if (method == "put") {
        QNetworkReply* putReply = nullptr;
        if (isBodyAndFormEmpty) putReply = m_networkManager->put(request, "");
        if (isBodyFilled) putReply = m_networkManager->put(request, body.toUtf8());
        if (!isBodyFilled && isSimpleForm) putReply = m_networkManager->put(request, setupSimpleForm(std::move(forms)));
        if (!isBodyFilled && isComplexForm) putReply = m_networkManager->put(request, setupMultiPartForm(std::move(files), std::move(forms)));
        if (putReply != nullptr) {
            startTrackRequest(putReply, id, resultModel);
            return true;
        }
    }
    if (method == "patch") {
        QNetworkReply* patchReply = nullptr;
        if (isBodyAndFormEmpty) patchReply = m_networkManager->sendCustomRequest(request, "PATCH", "");
        if (isBodyFilled) patchReply = m_networkManager->sendCustomRequest(request, "PATCH", body.toUtf8());
        if (!isBodyFilled && isSimpleForm) patchReply = m_networkManager->sendCustomRequest(request, "PATCH", setupSimpleForm(std::move(forms)));
        if (!isBodyFilled && isComplexForm) patchReply = m_networkManager->sendCustomRequest(request, "PATCH", setupMultiPartForm(std::move(files), std::move(forms)));
        if (patchReply != nullptr) {
            startTrackRequest(patchReply, id, resultModel);
            return true;
        }
    }

    return false;
}

void HttpPerformerViewModel::addToCounter(int number) noexcept
{
    if (m_countRequests > 0 && m_countRequests == m_countFinishedRequests) {
        m_countRequests = 0;
        m_countFinishedRequests = 0;
    }

    m_countRequests += number;

    emit countRequestsChanged();
    emit countFinishedRequestsChanged();
}

void HttpPerformerViewModel::reduceFromCounter() noexcept
{
    m_countFinishedRequests += 1;

    emit countFinishedRequestsChanged();
}

void HttpPerformerViewModel::increaseErrorCounter() noexcept
{
    m_countErrorRequests += 1;

    emit countErrorRequestsChanged();
}

void HttpPerformerViewModel::runPostScript(const QString &script, QObject* properties, HttpRequestResultViewModel* result) noexcept
{
    QJSEngine resultEngine;

    QJSValue myScriptQObject = resultEngine.newQObject(properties);
    resultEngine.globalObject().setProperty("response", myScriptQObject);

    auto resultObject = new PostScriptResultModel(this);
    connect(resultObject, &PostScriptResultModel::needSaveToFile, result, &HttpRequestResultViewModel::requestOnSaveFile);
    QJSValue interactScriptQObject = resultEngine.newQObject(resultObject);
    resultEngine.globalObject().setProperty("result", interactScriptQObject);

    QJSValue globalVarsQObject = resultEngine.newQObject(m_globalVariablePostScript);
    resultEngine.globalObject().setProperty("globals", globalVarsQObject);

    if (m_sessionObject != nullptr) {
        QJSValue sessionQObject = resultEngine.newQObject(m_sessionObject);
        resultEngine.globalObject().setProperty("session", sessionQObject);
    }

    auto scriptResult = resultEngine.evaluate(script);
    if (scriptResult.isError()) {
        result->setCustomErrorResult(true, "Postscript error at line " + scriptResult.property("lineNumber").toString() + " " + scriptResult.toString());
        return;
    }

    auto hasErrors = resultObject->hasErrors();
    auto errorMessage = resultObject->errorMessage();
    result->setCustomErrorResult(hasErrors, errorMessage);
}

void HttpPerformerViewModel::runMaintainQueries()
{
    emit countErrorRequestsChanged();
    emit countFinishedRequestsChanged();

    if (m_runningRequests.isEmpty() && m_orderedRequests.contains(m_orderedRequestsIndex + 1) && m_countErrorRequests == 0) {
        m_orderedRequestsIndex += 1;
        auto nextRequests = m_orderedRequests.values(m_orderedRequestsIndex);
        performRequests(nextRequests);
    } else {
        m_orderedRequests.clear();
        m_orderedRequestsIndex = 0;
    }
}

QString HttpPerformerViewModel::applyGlobalVariables(const QString &value)
{
    auto remapValues = m_emptyRemapDictionary;
    if (m_sessionObject != nullptr) remapValues = m_sessionObject->getRemapValues();

    return m_globalVariable->replaceGlobalVariables(value, remapValues);
}

void HttpPerformerViewModel::requestFinished(QNetworkReply *reply)
{   
    auto id = reply->property("id").toString();
    if (!m_runningRequests.contains(id)) return;

    auto result = m_runningRequests.value(id);
    if (result == nullptr) return;

    result->untrackRequestTime();
    result->setCurrentReply(nullptr);
    m_runningRequests.remove(id);
    reduceFromCounter();

    result->setNetworkError("");
    result->setCustomErrorResult(false, "");

    if (reply->error() != QNetworkReply::NoError) {
        result->setCustomErrorResult(true, reply->errorString());
    }

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (status_code.isValid()) result->setStatusCode(status_code.toInt());

    QStringList responseHeaders;
    auto headers = reply->rawHeaderPairs();
    foreach (auto header, headers) {
        auto name = header.first;
        auto value = header.second;
        responseHeaders.append(name + " " + value);
    }
    result->setHeaders(responseHeaders);
    if (reply->isReadable()) result->setBody(reply->readAll());

    auto postScript = result->postScript();
    if (postScript.isEmpty()) {
        if (result->hasError()) increaseErrorCounter();

        runMaintainQueries();
        return;
    }

    auto response = new PostScriptResponseModel();
    response->setHeaders(responseHeaders);
    response->setStatusCode(status_code.toInt());
    response->setErrorMessage(result->networkError());
    response->setBodySize(result->originResponseSize());
    response->setRoute(reply->url().toString());

    runPostScript(postScript, response, result);
    if (result->hasError()) increaseErrorCounter();

    runMaintainQueries();
}
