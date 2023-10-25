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

#include <QFile>
#include "backendviewmodel.h"

BackendViewModel::BackendViewModel(QObject *parent)
    : QObject{parent}
{
    addNewRequest();

    m_requestsCommandPaletter->setup(m_requests->getList());
    m_requestPerformer->setup(m_requests->getList());

    connect(m_requestPerformer, &HttpPerformerViewModel::pushErrorMessage, this, &BackendViewModel::errorNotification);
    connect(m_requestsCommandPaletter, &RequestsCommandPaletteListModel::itemSelected, this, &BackendViewModel::requestsPaletterItemSelected);
}

void BackendViewModel::addNewRequest()
{
    auto model = new HttpRequestModel(this);

    auto request = model->requestModel();
    request->setTextAdvisor(m_textAdviser);

    m_requests->addItem(model);
}

void BackendViewModel::shortcutHandler(const QString &shortcut) noexcept
{
    if (!shortcut.startsWith("control") && m_openedCommandPalette) {
        m_openedCommandPalette = false;
        m_requestsCommandPaletter->selectItem();
        emit openedCommandPaletteChanged();
        return;
    }

    if (shortcut == "control-tab") {
        if (!m_openedCommandPalette) {
            m_openedCommandPalette = true;
            m_requestsCommandPaletter->refresh(true);
            emit openedCommandPaletteChanged();
        } else {
            m_requestsCommandPaletter->selectNext();
        }
        return;
    }

    if (shortcut == "control-m") {
        m_requestPerformer->performAllRequest();
        return;
    }

    if (shortcut == "control-z" || shortcut == "f5") {
        m_requestPerformer->performAllRequest();
        auto request = m_requests->getSelectedRequest();
        m_requestPerformer->performOneRequest(request);
        return;
    }

    if (shortcut == "control-b" || shortcut == "f4") {
        m_requestPerformer->cancelRequest();
        return;
    }

    if (shortcut == "f10" || shortcut == "control-s") {
        m_requestExternal->copyToClipboard();
        return;
    }

    if (shortcut == "f1" || shortcut == "control-h") {
        setHelpVisible(!m_helpVisible);
        return;
    }

    if (shortcut == "shift-alt-l") {
        m_requestExternal->replaceFromClipboard();
        return;
    }

    if (shortcut == "control-l" || shortcut == "f3") {
        m_requestExternal->appendFromClipboard();
        return;
    }

    if (shortcut == "control-f6") {
        emit needGlobalVariablesWindow();
        return;
    }

    if (shortcut == "f6") {
        emit needOpenApiExportWindow();
        return;
    }

    if (shortcut == "control-f6") {
        m_requests->selectedItem()->requestModel()->clearFields();
        return;
    }

    if (shortcut == "shift-alt-r") {
        m_requests->selectedItem()->requestModel()->clearSelectedField();
        return;
    }

    if (shortcut == "control-enter") {
        auto request = m_requests->selectedItem()->requestModel();
        request->addItem(request->selectedItem() + 1);
        return;
    }

    if (shortcut == "alt-enter") {
        auto request = m_requests->selectedItem()->requestModel();
        request->addItem(request->selectedItem());
        return;
    }

    if (shortcut == "shift-enter") {
        m_requests->selectedItem()->requestModel()->addItem(-1);
        return;
    }

    if (shortcut == "control-{") {
        m_requests->selectedItem()->requestModel()->sortingFields(false);
        return;
    }

    if (shortcut == "control-}") {
        m_requests->selectedItem()->requestModel()->sortingFields(true);
        return;
    }

    if (shortcut == "f11") {
        m_tabs->toggleTabs();
        return;
    }

    if (shortcut == "control-insert") {
        addNewRequest();
        return;
    }

    if (shortcut == "control-delete") {
        deleteCurrentRequest();
        return;
    }

    if (shortcut == "control-pagedown") {
        m_requests->selectedItem()->requestModel()->selectLastField();
        return;
    }

    if (shortcut == "pagedown") {
        m_requests->selectedItem()->requestModel()->selectDownField();
        return;
    }

    if (shortcut == "control-pageup") {
        m_requests->selectedItem()->requestModel()->selectFirstField();
        return;
    }

    if (shortcut == "pageup") {
        m_requests->selectedItem()->requestModel()->selectUpField();
        return;
    }

    if (shortcut == "shift-alt-b") {
        m_requests->selectedItem()->resultModel()->copyBodyToClipboard();
        return;
    }

    if (shortcut == "shift-alt-h") {
        m_requests->selectedItem()->resultModel()->copyHeadersToClipboard();
        return;
    }

    if (shortcut == "alt-plus") {
        emit needOpenFile();
        return;
    }

    if (shortcut == "control-plus") {
        emit needSaveFile();
        return;
    }

    if (shortcut == "control-8") {
        emit needGenerateImage();
        return;
    }

    if (shortcut == "alt-8") {
        generateImageToClipboard();
        return;
    }

    if (shortcut == "control-alt-down") {
        auto index = m_requests->selectedItem()->resultModel()->bodyModel()->nextFindedResult();
        if (index > -1) emit changedFindedIndex(index);
        return;
    }

    if (shortcut == "control-alt-up") {
        auto index = m_requests->selectedItem()->resultModel()->bodyModel()->previousFindedResult();
        if (index > -1) emit changedFindedIndex(index);
        return;
    }
}

void BackendViewModel::refreshFindedIndex() noexcept
{
    auto bodyModel = m_requests->selectedItem()->resultModel()->bodyModel();
    auto findedLine = bodyModel->getCurrentFindedLine();
    if (findedLine > -1) emit changedFindedIndex(findedLine);
}

void BackendViewModel::openedFile(const QString &filePath) noexcept
{
    auto fileName = removeProtocol(filePath);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorNotification("Open file error", file.errorString());
        return;
    }
    auto content = file.readAll();
    file.close();

    m_requests->selectedItem()->requestModel()->clearFields();
    m_requestExternal->parseFromString(content);
    m_requestExternal->removeFirstItemIfNeeded();
}

void BackendViewModel::savedFile(const QString &filePath) noexcept
{
    auto fileName = removeProtocol(filePath);
    auto fields = m_requests->selectedItem()->requestModel()->getAllFields();
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorNotification("Open file error", file.errorString());
        return;
    }

    file.write(fields.toUtf8());
    file.close();
}

void BackendViewModel::generateImage(const QString &filePath) noexcept
{
    auto request = m_requests->selectedItem()->requestModel();
    auto result = m_requests->selectedItem()->resultModel();

    auto fields = request->getAllFieldsAsList();
    result->generateImage(fields, removeProtocol(filePath), false);
}

void BackendViewModel::generateImageToClipboard() noexcept
{
    auto request = m_requests->selectedItem()->requestModel();
    auto result = m_requests->selectedItem()->resultModel();

    auto fields = request->getAllFieldsAsList();
    result->generateImage(fields, "", true);
}

void BackendViewModel::importFromOpenApi(int index) noexcept
{
    auto route = m_openApiExporter->getRouteFromOpenApiByIndex(index);
    if (route == nullptr) return;

    auto routeOptions = m_openApiExporter->getRoutesOptions();
    if (routeOptions == nullptr) return;

    auto model = new HttpRequestModel(this);

    auto request = model->requestModel();
    request->setTextAdvisor(m_textAdviser);

    auto summary = route->summary();
    if (!summary.isEmpty()) request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::TitleType, route->summary());

    auto baseUrl = m_openApiExporter->baseUrl();
    auto routePath = route->path();
    if (!baseUrl.isEmpty()) routePath = baseUrl + route->path();
    request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::UrlType, routePath);

    request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::MethodType, route->method().toUpper());

    auto parameters = route->parameters();
    foreach (auto parameter, parameters) {
        if (parameter->isCookie()) {
            request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::PastryType, parameter->name() + "=");
        }
        if (parameter->isHeader()) {
            request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::HeaderType, parameter->name() + " ");
        }
        if (parameter->isQuery()) {
            request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::ParamType, parameter->name() + "=");
        }
        if (parameter->isPath()) {
            request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::RouteType, parameter->name() + "=");
        }
    }

    auto prepareBody = m_openApiExporter->prepareBodyType();
    if (!prepareBody.isEmpty()) {
        request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::BodyType, route->bodyByType(prepareBody));
        m_openApiExporter->setPrepareBodyType("");
    }

    auto authMethod = m_openApiExporter->authMethod();

    if (route->hasSecurity()) {
        QStringList routeSecurityKeys = authMethod.isEmpty() ? route->getFirstKeys() : route->getKeys(authMethod.split(",", Qt::SkipEmptyParts));
        foreach (auto routeSecurityKey, routeSecurityKeys) fillAuthorizationSecurity(routeSecurityKey.trimmed(), request, *routeOptions);
    } else {
        if (authMethod.isEmpty() && routeOptions->onlyOneSecurity()) {
            auto firstSecurity = routeOptions->getFirstSecurity();
            foreach (auto securityKey, firstSecurity) fillAuthorizationSecurity(securityKey, request, *routeOptions);
        }
        if (!authMethod.isEmpty()) {
            auto authKeys = authMethod.split(",", Qt::SkipEmptyParts);
            foreach (auto authKey, authKeys) fillAuthorizationSecurity(authKey.trimmed(), request, *routeOptions);
        }
    }

    request->removeFirstItem();

    auto createdIndex = m_requests->addItem(model);
    m_requests->selectItem(createdIndex);
}

void BackendViewModel::deleteCurrentRequest() noexcept
{
    if (m_requests->singleRequest()) addNewRequest();

    m_requests->deleteSelectedItem();
}

void BackendViewModel::setHelpVisible(const bool helpVisible) noexcept
{
    if (m_helpVisible == helpVisible) return;

    m_helpVisible = helpVisible;
    emit helpVisibleChanged();
}

QString BackendViewModel::removeProtocol(const QString& filePath) noexcept
{
    auto path = filePath;
    return path.replace("file:///", "").replace("file://", "");
}

void BackendViewModel::fillAuthorizationSecurity(const QString &key, HttpRequestViewModel* request, const OpenApiRoutesOptions &options)
{
    auto scheme = options.getScheme(key);
    if (scheme == nullptr) return;

    auto type = scheme->type().toLower();
    if (type == "http" && !scheme->httpScheme().isEmpty()) {
        request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::HeaderType, "Authorization " + scheme->httpScheme() + " ");
    }
    if (type == "apikey") {
        auto in = scheme->in().toLower();
        if (in == "query") request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::ParamType, scheme->name() + "=");
        if (in == "cookie") request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::PastryType, scheme->name() + "=");
        if (in == "header") request->addItem(-1, HttpRequestViewModel::HttpRequestTypes::HeaderType, scheme->name() + " ");
    }
}

void BackendViewModel::errorNotification(const QString &title, const QString &message)
{
    m_notifications->pushErrorMessage(title, message);
}

void BackendViewModel::requestsPaletterItemSelected(const QUuid &id)
{
    m_requests->selectItemById(id);
}
