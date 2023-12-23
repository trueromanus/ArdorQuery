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
#include <QMapIterator>
#include <QMultiMap>
#include "backendviewmodel.h"
#include "../globalhelpers.h"

BackendViewModel::BackendViewModel(QObject *parent)
    : QObject{parent}
{
    addNewRequest();

    m_requestsCommandPaletter->setup(m_requests->getList());
    m_requestPerformer->setup(m_requests->getList());

    connect(m_requestPerformer, &HttpPerformerViewModel::pushErrorMessage, this, &BackendViewModel::errorNotification);
    connect(m_requestsCommandPaletter, &RequestsCommandPaletteListModel::itemSelected, this, &BackendViewModel::requestsPaletterItemSelected);

    fillMappings();
    fillCommands();
    fillHelpShortcuts();
}

void BackendViewModel::setFocusedHelpTextField(bool focusedHelpTextField) noexcept
{
    if (m_focusedHelpTextField == focusedHelpTextField) return;

    m_focusedHelpTextField = focusedHelpTextField;
    emit focusedHelpTextFieldChanged();
}

void BackendViewModel::addNewRequest(bool forceSelectedAddedItem)
{
    auto model = new HttpRequestModel(this);

    auto request = model->requestModel();
    request->setTextAdvisor(m_textAdviser);
    request->setSelectedItem(0); // select first empty field for new request

    m_requests->addItem(model);

    if (forceSelectedAddedItem) {
        m_requestsCommandPaletter->refresh(true); // recreate history
        m_requestsCommandPaletter->forceSelectItem(model->requestId()); // force selection of added element
    }
}

bool BackendViewModel::shortcutHandler(const QString &shortcut) noexcept
{
    if (!shortcut.startsWith("control") && m_openedCommandPalette) {
        m_openedCommandPalette = false;
        m_requestsCommandPaletter->selectItem();
        emit openedCommandPaletteChanged();
    }

    if (!m_shortcutCommandMapping.contains(shortcut)) return false;

    auto command = m_shortcutCommandMapping[shortcut];

    if (command == m_changeSelectedQueryCommand) {
        if (!m_openedCommandPalette) {
            m_openedCommandPalette = true;
            m_requestsCommandPaletter->refresh(true);
            emit openedCommandPaletteChanged();
        } else {
            m_requestsCommandPaletter->selectNext();
        }
    } else if (command == m_performQueriesMultipleCommand) {
        m_requestPerformer->performAllRequest();
    } else if (command == m_performQueryCommand) {
        auto request = m_requests->getSelectedRequest();
        m_requestPerformer->performOneRequest(request);
    } else if (command == m_cancelQueryCommand) {
        m_requestPerformer->cancelRequest();
    } else if (command == m_saveToClipboardCommand) {
        m_requestExternal->copyToClipboard();
    } else if (command == m_helpCommand) {
        setHelpVisible(!m_helpVisible);
    } else if (command == m_replaceFromClipboardCommand) {
        m_requestExternal->replaceFromClipboard();
    } else if (command == m_appendFromClipboardCommand) {
        m_requestExternal->appendFromClipboard();
    } else if (command == m_globalVariablesCommand) {
        emit needGlobalVariablesWindow();
    } else if (command == m_opeApiExportCommand) {
        emit needOpenApiExportWindow();
    } else if (command == m_removeSelectedFieldCommand) {
        m_requests->selectedItem()->requestModel()->clearSelectedField();
    } else if (command == m_removeAllFieldCommand) {
        m_requests->selectedItem()->requestModel()->clearFields();
    } else if (command == m_addLineBelowCommand) {
        auto request = m_requests->selectedItem()->requestModel();
        request->addItem(request->selectedItem() + 1);
    } else if (command == m_addLineAboveCommand) {
        auto request = m_requests->selectedItem()->requestModel();
        request->addItem(request->selectedItem());
    } else if (command == m_addLineToEndCommand) {
        m_requests->selectedItem()->requestModel()->addItem(-1);
    } else if (command == m_sortAscendingCommand) {
        m_requests->selectedItem()->requestModel()->sortingFields(false);
    } else if (command == m_sortDescendingCommand) {
        m_requests->selectedItem()->requestModel()->sortingFields(true);
    } else if (command == m_toggleTabsCommand) {
        m_tabs->toggleTabs();
    } else if (command == m_addQueryCommand) {
        addNewRequest(true);
    } else if (command == m_deleteSelectedQueryCommand) {
        deleteCurrentRequest();
    } else if (command == m_selectLastFieldCommand) {
        m_requests->selectedItem()->requestModel()->selectLastField();
    } else if (command == m_selectNextFieldCommand) {
        m_requests->selectedItem()->requestModel()->selectDownField();
    } else if (command == m_selectTopFieldCommand) {
        m_requests->selectedItem()->requestModel()->selectFirstField();
    } else if (command == m_selectPreviousFieldCommand) {
        m_requests->selectedItem()->requestModel()->selectUpField();
    } else if (command == m_copyBodyToClipboardCommand) {
        m_requests->selectedItem()->resultModel()->copyBodyToClipboard();
    } else if (command == m_copyHeadersToClipboardCommand) {
        m_requests->selectedItem()->resultModel()->copyHeadersToClipboard();
    } else if (command == m_openFromFileCommand) {
        emit needOpenFile();
    } else if (command == m_saveToFileCommand) {
        emit needSaveFile();
    } else if (command == m_generateImageToFileCommand) {
        emit needGenerateImage();
    } else if (command == m_generateImageToClipboardCommand) {
        generateImageToClipboard();
    } else if (command == m_nextFindedResultCommand) {
        auto index = m_requests->selectedItem()->resultModel()->bodyModel()->nextFindedResult();
        if (index > -1) emit changedFindedIndex(index);
    } else if (command == m_previousFindedResultCommand) {
        auto index = m_requests->selectedItem()->resultModel()->bodyModel()->previousFindedResult();
        if (index > -1) emit changedFindedIndex(index);
    }

    return true;
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

void BackendViewModel::importFromOpenApi(int index, bool replaceCurrent) noexcept
{
    auto route = m_openApiExporter->getRouteFromOpenApiByIndex(index);
    if (route == nullptr) return;

    auto routeOptions = m_openApiExporter->getRoutesOptions();
    if (routeOptions == nullptr) return;

    HttpRequestModel* model = nullptr;
    if (replaceCurrent) {
        model = m_requests->selectedItem();
        model->requestModel()->clearFields();
    } else {
        model = new HttpRequestModel(this);
    }

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
        request->addItem(
            -1,
            HttpRequestViewModel::HttpRequestTypes::BodyType,
            route->bodyByType(prepareBody),
            prepareBody.contains("/json") ? JsonPrefix : ""
        );
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

    if (!replaceCurrent) {
        auto createdIndex = m_requests->addItem(model);
        m_requests->selectItem(createdIndex);
        m_requestsCommandPaletter->refresh(true);
        m_requestsCommandPaletter->forceSelectItem(model->requestId());
    }
}

void BackendViewModel::saveDownloadedFile(const QString &fileName) noexcept
{
    m_requests->selectedItem()->resultModel()->saveBodyToFile(removeProtocol(fileName));
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

void BackendViewModel::fillMappings()
{
    m_shortcutCommandMapping.insert("control-tab", m_changeSelectedQueryCommand);
    m_shortcutCommandMapping.insert("control-backspace", m_changeSelectedQueryCommand);
    m_shortcutCommandMapping.insert("control-m", m_performQueriesMultipleCommand);
    m_shortcutCommandMapping.insert("control-z", m_performQueryCommand);
    m_shortcutCommandMapping.insert("f5", m_performQueryCommand);
    m_shortcutCommandMapping.insert("control-b", m_cancelQueryCommand);
    m_shortcutCommandMapping.insert("f4", m_cancelQueryCommand);
    m_shortcutCommandMapping.insert("control-s", m_saveToClipboardCommand);
    m_shortcutCommandMapping.insert("f10", m_saveToClipboardCommand);
    m_shortcutCommandMapping.insert("control-h", m_helpCommand);
    m_shortcutCommandMapping.insert("f1", m_helpCommand);
    m_shortcutCommandMapping.insert("shift-alt-l", m_replaceFromClipboardCommand);
    m_shortcutCommandMapping.insert("control-l", m_appendFromClipboardCommand);
    m_shortcutCommandMapping.insert("f3", m_appendFromClipboardCommand);
    m_shortcutCommandMapping.insert("control-f6", m_globalVariablesCommand);
    m_shortcutCommandMapping.insert("control-g", m_globalVariablesCommand);
    m_shortcutCommandMapping.insert("f6", m_opeApiExportCommand);
    m_shortcutCommandMapping.insert("control-o", m_opeApiExportCommand);
    m_shortcutCommandMapping.insert("control-r", m_removeSelectedFieldCommand);
    m_shortcutCommandMapping.insert("shift-alt-r", m_removeAllFieldCommand);
    m_shortcutCommandMapping.insert("control-enter", m_addLineBelowCommand);
    m_shortcutCommandMapping.insert("alt-enter", m_addLineAboveCommand);
    m_shortcutCommandMapping.insert("shift-enter", m_addLineToEndCommand);
    m_shortcutCommandMapping.insert("control-{", m_sortAscendingCommand);
    m_shortcutCommandMapping.insert("control-}", m_sortDescendingCommand);
#ifdef Q_OS_MACOS
    m_shortcutCommandMapping.insert("control-f11", m_toggleTabsCommand);
#else
    m_shortcutCommandMapping.insert("f11", m_toggleTabsCommand);
#endif
    m_shortcutCommandMapping.insert("control-insert", m_addQueryCommand);
    m_shortcutCommandMapping.insert("control-d", m_deleteSelectedQueryCommand);
#ifndef Q_OS_MACOS
    m_shortcutCommandMapping.insert("control-delete", m_deleteSelectedQueryCommand);
#endif
    m_shortcutCommandMapping.insert("control-pagedown", m_selectLastFieldCommand);
    m_shortcutCommandMapping.insert("pagedown", m_selectNextFieldCommand);
    m_shortcutCommandMapping.insert("control-pageup", m_selectTopFieldCommand);
    m_shortcutCommandMapping.insert("pageup", m_selectPreviousFieldCommand);
    m_shortcutCommandMapping.insert("shift-alt-b", m_copyBodyToClipboardCommand);
    m_shortcutCommandMapping.insert("shift-alt-h", m_copyHeadersToClipboardCommand);
    m_shortcutCommandMapping.insert("alt-plus", m_openFromFileCommand);
    m_shortcutCommandMapping.insert("control-plus", m_saveToFileCommand);
    m_shortcutCommandMapping.insert("control-8", m_generateImageToFileCommand);
    m_shortcutCommandMapping.insert("alt-8", m_generateImageToClipboardCommand);
#ifdef Q_OS_LINUX
    m_shortcutCommandMapping.insert("control-down", m_nextFindedResultCommand);
    m_shortcutCommandMapping.insert("control-up", m_previousFindedResultCommand);
#else
    m_shortcutCommandMapping.insert("control-alt-down", m_nextFindedResultCommand);
    m_shortcutCommandMapping.insert("control-alt-up", m_previousFindedResultCommand);
#endif
}

void BackendViewModel::fillCommands()
{
    m_shortcutCommands.insert(m_changeSelectedQueryCommand, "holding Ctrl and further pressing Tab change opening query");
    m_shortcutCommands.insert(m_performQueriesMultipleCommand, "Run all requests");
    m_shortcutCommands.insert(m_performQueryCommand, "Run current request");
    m_shortcutCommands.insert(m_cancelQueryCommand, "Cancel request");
    m_shortcutCommands.insert(m_saveToClipboardCommand, "Save current query");
    m_shortcutCommands.insert(m_helpCommand, "Show interactive help for shortcuts");
    m_shortcutCommands.insert(m_replaceFromClipboardCommand, "Clear all fields and load fields from clipboard");
    m_shortcutCommands.insert(m_appendFromClipboardCommand, "Add fields from the clipboard below the currently selected field");
    m_shortcutCommands.insert(m_globalVariablesCommand, "Show global variables window");
    m_shortcutCommands.insert(m_opeApiExportCommand, "Show OpenAPI Export window");
    m_shortcutCommands.insert(m_removeSelectedFieldCommand, "Remove selected field");
    m_shortcutCommands.insert(m_removeAllFieldCommand, "Remove all fields");
    m_shortcutCommands.insert(m_addLineBelowCommand, "Adding a new empty field below the selected field");
    m_shortcutCommands.insert(m_addLineAboveCommand, "Adding a new empty field on top of the selected field");
    m_shortcutCommands.insert(m_addLineToEndCommand, "Adding a new empty field at the end of the fields");
    m_shortcutCommands.insert(m_sortAscendingCommand, "Sorting by type in ascending order");
    m_shortcutCommands.insert(m_sortDescendingCommand, "Sorting by type in descending order");
    m_shortcutCommands.insert(m_toggleTabsCommand, "Toggle tabs betweens Query and Result");
    m_shortcutCommands.insert(m_addQueryCommand, "Create a new empty query");
    m_shortcutCommands.insert(m_deleteSelectedQueryCommand, "Delete current query");
    m_shortcutCommands.insert(m_selectLastFieldCommand, "Selecting end a text field");
    m_shortcutCommands.insert(m_selectNextFieldCommand, "Select a text field below the currently selected field");
    m_shortcutCommands.insert(m_selectTopFieldCommand, "Selecting start a text field");
    m_shortcutCommands.insert(m_selectPreviousFieldCommand, "Select a text field above the currently selected field");
    m_shortcutCommands.insert(m_copyHeadersToClipboardCommand, "Copy result headers to clipboard");
    m_shortcutCommands.insert(m_copyBodyToClipboardCommand, "Copy result body to clipboard");
    m_shortcutCommands.insert(m_openFromFileCommand, "Import fields from file");
    m_shortcutCommands.insert(m_saveToFileCommand, "Save fields to file");
    m_shortcutCommands.insert(m_generateImageToFileCommand, "Generate image contains query fields, response summary and headers and save to file");
    m_shortcutCommands.insert(m_generateImageToClipboardCommand, "Generate image contains query fields, response summary and headers and save to clipboard");
    m_shortcutCommands.insert(m_nextFindedResultCommand, "Next founded result");
    m_shortcutCommands.insert(m_previousFindedResultCommand, "Previous founded result");
}

void BackendViewModel::fillHelpShortcuts()
{
    m_shortcuts.clear();

    auto mappingIterator = QMapIterator(m_shortcutCommandMapping);
    QMultiMap<QString, QString> commandWithShortcuts;
    while (mappingIterator.hasNext()) {
        mappingIterator.next();
        commandWithShortcuts.insert(mappingIterator.value(), mappingIterator.key());
    }

    auto iterator = QMapIterator(m_shortcutCommands);

    while (iterator.hasNext()) {
        iterator.next();

        auto key = iterator.key();
        auto value = iterator.value();
        QVariantMap map;
        map["description"] = value;
        auto shortcuts = commandWithShortcuts.values(key);
        auto allShortcuts = shortcuts.join(" or ");
        map["shortcuts"] = adjustShortcutsForDisplay(allShortcuts);
        m_shortcuts.append(map);
    }

    emit shortcutsChanged();
}

void BackendViewModel::errorNotification(const QString &title, const QString &message)
{
    m_notifications->pushErrorMessage(title, message);
}

void BackendViewModel::requestsPaletterItemSelected(const QUuid &id)
{
    m_requests->selectItemById(id);
}
