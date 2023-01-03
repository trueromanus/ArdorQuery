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

bool BackendViewModel::keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept
{
    auto request = m_requests->selectedItem()->requestModel();
    auto result = m_requests->selectedItem()->resultModel();

    // ---------
    // Perform requests

    // Ctrl-Z or F5
    if (((nativeCode == 44 || key == Qt::Key_Z) && control) || (nativeCode == 63 || key == Qt::Key_F5)) {
        m_requestPerformer->performRequest();
        return true;
    }

    // Ctrl-B or F4
    if (((nativeCode == 48 || key == Qt::Key_B) && control) || (nativeCode == 62 || key == Qt::Key_F4)) {
        m_requestPerformer->cancelRequest();
        return true;
    }

    // ---------
    // Export

    // Ctrl-S or F10
    if (((nativeCode == 31 || key == Qt::Key_S) && control) || (nativeCode == 68 || key == Qt::Key_F10)) {
        m_requestExternal->copyToClipboard();
        return true;
    }

    // ---------
    // Import

    // Shift-Alt-L
    if ((nativeCode == 38 || key == Qt::Key_L) && shift && alt) {
        m_requestExternal->replaceFromClipboard();
        return true;
    }

    // Ctrl-L or F3
    if (((nativeCode == 38 || key == Qt::Key_L) && control) || (nativeCode == 61 || key == Qt::Key_F3)) {
        m_requestExternal->appendFromClipboard();
        return true;
    }

    // F6
    if (key == Qt::Key_F6) {
        emit needOpenApiExportWindow();
    }

    // ---------
    // Change selection

    // PgUp/Numpad PgUp or Ctrl-PgUp/Ctrl-Numpad PgUp
    if (nativeCode == 329 || nativeCode == 73 || key == Qt::Key_PageUp) {
        if (control) {
            request->selectFirstField();
        } else {
            request->selectUpField();
        }
        return true;
    }

    // PgDown/Numpad PgDown or Ctrl-PgDown/Ctrl-Numpad PgDown
    if (nativeCode == 337 || nativeCode == 81 || key == Qt::Key_PageDown) {
        if (control) {
            request->selectLastField();
        } else {
            request->selectDownField();
        }
        return true;
    }

    // ---------
    // Miscellanious

    // Ctrl-H or F1
#ifdef Q_OS_WIN
    if (((nativeCode == 35 || key == Qt::Key_H) && control && !shift && !alt) || (nativeCode == 59 || key == Qt::Key_F1)) {
        setHelpVisible(!m_helpVisible);
        return true;
    }
#else
    if ((key == Qt::Key_H && control) || key == Qt::Key_F1) {
        setHelpVisible(!m_helpVisible);
        return true;
    }

#endif

    // ---------
    // Fields

    // Ctrl-R
    if ((nativeCode == 19 || key == Qt::Key_R) && control) {
        request->clearFields();
        return true;
    }

    // Alt-Shift-R
    if ((nativeCode == 19 || key == Qt::Key_R) && alt && shift) {
        request->clearSelectedField();
        return true;
    }

    // Ctrl-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && control) {
        request->addItem(request->selectedItem() + 1);
        return true;
    }

    // Alt-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && alt) {
        request->addItem(request->selectedItem());
        return true;
    }

    // Shift-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && shift) {
        request->addItem(-1);
        return true;
    }


    // Ctrl-{
    if ((nativeCode == 26 || key == Qt::Key_BracketLeft) && control) {
        request->sortingFields(false);
        return true;
    }

    // Ctrl-}
    if ((nativeCode == 27 || key == Qt::Key_BracketRight) && control) {
        request->sortingFields(true);
        return true;
    }

    // F11
    if (key == Qt::Key_F11) {
        m_tabs->toggleTabs();
        return true;
    }

    // ---------
    // Queries

    // Ctrl-Tab
    if (key == Qt::Key_Tab && control) {
        if (!m_openedCommandPalette) {
            m_openedCommandPalette = true;
            m_requestsCommandPaletter->refresh(true);
            emit openedCommandPaletteChanged();
        } else {
            m_requestsCommandPaletter->selectNext();
        }
        return true;
    }

    // ---------
    // Body search

    // Ctrl-Alt-Down or Shift-Alt-Down
    if ((nativeCode == 336 || key == Qt::Key_Down) && (control || shift) && alt) {
        auto index = result->bodyModel()->nextFindedResult();
        if (index > -1) emit changedFindedIndex(index);
        return true;
    }

    // Ctrl-Alt-Up or Shift-Alt-Up
    if ((nativeCode == 328 || key == Qt::Key_Up) && (control || shift) && alt) {
        auto index = result->bodyModel()->previousFindedResult();
        if (index > -1) emit changedFindedIndex(index);
        return true;
    }

    // ---------
    // Queries

    // Ctrl-Insert
    if ((nativeCode == 338 || key == Qt::Key_Insert) && control) {
        addNewRequest();
    }

    // --------
    // Copying results

    // Shift-Alt-B
    if ((nativeCode == 48 || key == Qt::Key_B) && shift && alt) {
        result->copyBodyToClipboard();
        return true;
    }

    // Shift-Alt-H
    if ((nativeCode == 35 || key == Qt::Key_H) && shift && alt) {
        result->copyHeadersToClipboard();
        return true;
    }

    // Alt-=
    if (key == Qt::Key_Equal && alt) {
        emit needOpenFile();
        return true;
    }

    // Ctrl-=
    if (key == Qt::Key_Equal && control) {
        emit needSaveFile();
        return true;
    }

    // Ctrl-8
    if (key == Qt::Key_8 && control) {
        emit needGenerateImage();
        return true;
    }

    // Alt-8
    if (key == Qt::Key_8 && alt) {
        generateImageToClipboard();
        return true;
    }

    return false;
}

void BackendViewModel::keysReleased(int key) noexcept
{
    if (key == Qt::Key_Control && m_openedCommandPalette) {
        m_openedCommandPalette = false;
        m_requestsCommandPaletter->selectItem();
        emit openedCommandPaletteChanged();
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

    //TODO: check security in route
    //if (!route->security.isEmpty()) {
    //} else {

    auto authMethod = m_openApiExporter->authMethod();
    if (authMethod.isEmpty() && routeOptions->onlyOneSecurity()) {
        auto firstSecurity = routeOptions->getFirstSecurity();
        foreach (auto securityKey, firstSecurity) fillAuthorizationSecurity(securityKey, request, *routeOptions);
    }
    if (!authMethod.isEmpty()) {
        auto authKeys = authMethod.split(",", Qt::SkipEmptyParts);
        foreach (auto authKey, authKeys) fillAuthorizationSecurity(authKey.trimmed(), request, *routeOptions);
    }

    request->removeFirstItem();

    auto createdIndex = m_requests->addItem(model);
    m_requests->selectItem(createdIndex);
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
