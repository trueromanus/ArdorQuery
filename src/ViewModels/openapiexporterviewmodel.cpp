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

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtConcurrent>
#include "openapiexporterviewmodel.h"
#include "../Models/openapiparametermodel.h"
#include "../globalhelpers.h"

OpenApiExporterViewModel::OpenApiExporterViewModel(QObject *parent)
    : QObject{parent}
{
    m_addressPalette->setup(m_addresses->getAddresses());

    m_tabs.append(Importer);
    m_tabs.append(SavedOptions);

    connect(m_networkManager, &QNetworkAccessManager::finished, this, &OpenApiExporterViewModel::requestFinished);
    connect(m_addresses, &OpenApiAddressesListModel::addressesChanged, this, &OpenApiExporterViewModel::addressListChanged);
    connect(m_addressPalette, &AddressesPaletteListModel::itemSelected, this, &OpenApiExporterViewModel::addressItemSelected);
    connect(m_schemeWatcher, &QFutureWatcher<int>::finished, this, &OpenApiExporterViewModel::parsingFinished);

    fillMappings();
    fillCommands();
    fillHelpShortcuts();
}

void OpenApiExporterViewModel::setBaseUrl(const QString &baseUrl) noexcept
{
    if (m_baseUrl == baseUrl) return;

    m_baseUrl = baseUrl;
    emit baseUrlChanged();
}

void OpenApiExporterViewModel::setHelpVisible(bool helpVisible) noexcept
{
    if (m_helpVisible == helpVisible) return;

    m_helpVisible = helpVisible;
    emit helpVisibleChanged();
}

void OpenApiExporterViewModel::setTitle(const QString &title) noexcept
{
    if (m_title == title) return;

    m_title = title;
    emit titleChanged();
}

void OpenApiExporterViewModel::setAuthMethod(const QString &authMethod) noexcept
{
    if (m_authMethod == authMethod) return;

    m_authMethod = authMethod;
    emit authMethodChanged();
}

void OpenApiExporterViewModel::setOpenedCommandPalette(bool openedCommandPalette) noexcept
{
    if (m_openedCommandPalette == openedCommandPalette) return;

    m_openedCommandPalette = openedCommandPalette;
    emit openedCommandPaletteChanged();
}

OpenApiRouteModel *OpenApiExporterViewModel::getRouteFromOpenApiByIndex(int index) const noexcept
{
    return m_routeList->getRouteByIndex(index);
}

OpenApiRoutesOptions *OpenApiExporterViewModel::getRoutesOptions()
{
    if (!m_routes.contains(m_url)) return nullptr;
    auto routeData = m_routes[m_url];
    return std::get<0>(routeData);
}

void OpenApiExporterViewModel::setSelectedTab(const QString &selectedTab) noexcept
{
    if (m_selectedTab == selectedTab) return;

    m_selectedTab = selectedTab;
    emit selectedTabChanged();
    emit exporterPageChanged();
    emit savedOptionsPageChanged();
}

void OpenApiExporterViewModel::setPrepareIdentifier(int prepareIdentifier) noexcept
{
    if (m_prepareIndetifier == prepareIdentifier) return;

    m_prepareIndetifier = prepareIdentifier;
    emit prepareIdentifierChanged();
}

void OpenApiExporterViewModel::setPrepareBodyType(const QString &prepareBodyType) noexcept
{
    if (m_prepareBodyType == prepareBodyType) return;

    m_prepareBodyType = prepareBodyType;
    emit prepareBodyTypeChanged();
}

void OpenApiExporterViewModel::cancelCurrentRequest() noexcept
{
    if (!m_loading) return;
    if (m_currentReply == nullptr) return;

    m_currentReply->abort();
    m_currentReply = nullptr;

    clearError();
}

void OpenApiExporterViewModel::clearError() noexcept
{
    m_loading = false;
    m_errorMessage = "";
    emit loadingChanged();
    emit errorMessageChanged();
}

void OpenApiExporterViewModel::setupError(const QString &message) noexcept
{
    m_loading = true;
    m_errorMessage = message;
    emit loadingChanged();
    emit errorMessageChanged();
}

void OpenApiExporterViewModel::loadOpenApiScheme() noexcept
{
    if (m_loading) return;

    clearErrorMessage();
    if (m_url.isEmpty()) {
        m_errorMessage = "To download the diagram, you must fill in the <b>Scheme URL</b> field!";
        emit errorMessageChanged();
        return;
    }

    if (m_url.startsWith("file://")) {
        clearError();
        auto schemaPath = removeFileProtocol(m_url);
        auto schemaFile = QFile(schemaPath);
        if (schemaFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            auto content = schemaFile.readAll();
            loadSchemeFromArray(content, m_JsonSchemeFormat);
        } else {
            setupError("Can't read schema file!");
        }
        return;
    }

    if (m_routes.contains(m_url)) {
        removeLoadedRoutes(m_url);
    }

    QNetworkRequest request(m_url);

    m_currentReply = m_networkManager->get(request);

    clearError();
    m_loading = true;
    emit loadingChanged();
}

void OpenApiExporterViewModel::setUrl(const QString &url) noexcept
{
    if (m_url == url) return;

    m_url = url;
    emit urlChanged();
    emit alreadyLoadedChanged();
}

bool OpenApiExporterViewModel::shortcutHandler(const QString &shortcut) noexcept
{
    if (!shortcut.startsWith("control") && m_openedCommandPalette) {
        m_openedCommandPalette = false;
        m_addressPalette->selectItem();
        emit openedCommandPaletteChanged();
    }

    if (!m_shortcutCommandMapping.contains(shortcut)) return false;

    auto command = m_shortcutCommandMapping.value(shortcut);

    if (command == m_loadSchemaCommand) {
        loadOpenApiScheme();
    } else if (command == m_cancelLoadSchemaCommand) {
        cancelCurrentRequest();
    } else if (command == m_saveSchemaCommand) {
        saveToAddressByTitle();
    } else if (command == m_addSchemaCommand) {
        addCurrentToAddresses();
    } else if (command == m_changeSelectedSchemaCommand) {
        if (!m_openedCommandPalette) {
            m_openedCommandPalette = true;
            m_addressPalette->refresh(true);
            emit openedCommandPaletteChanged();
        } else {
            m_addressPalette->selectNext();
        }
    } else if (command == m_helpCommand) {
        setHelpVisible(!m_helpVisible);
    } else if (command == m_closeWindowCommand) {
        emit needCloseWindow();
    } else if (command == m_deleteSelectedSchemaCommand) {
        deleteSelectedAddress();
    } else if(command == m_toggleTabsCommand) {
        togglePages();
    } else if(command == m_nextRouteCommand) {
        m_routeList->nextRoute();
    } else if(command == m_previousRouteCommand) {
        m_routeList->previousRoute();
    } else if(command == m_addAsNewQueryRouteCommand) {
        auto selectedRoute = m_routeList->getSelectedRoute();
        if (selectedRoute != nullptr) emit selectRouteFromList(selectedRoute->identifier(), false);
    } else if(command == m_addToCurrentQueryRouteCommand) {
        auto selectedRoute = m_routeList->getSelectedRoute();
        if (selectedRoute != nullptr) emit selectRouteFromList(selectedRoute->identifier(), true);
    }

    return true;
}

void OpenApiExporterViewModel::addCurrentToAddresses() noexcept
{
    m_addresses->addAddress(m_title.isEmpty() ? m_url : m_title, m_url, m_baseUrl, m_routeList->filter(), m_authMethod);
}

void OpenApiExporterViewModel::togglePages() noexcept
{
    if (m_selectedTab == Importer) {
        setSelectedTab(SavedOptions);
    } else {
        setSelectedTab(Importer);
    }
}

void OpenApiExporterViewModel::deleteSelectedAddress() noexcept
{
    clearErrorMessage();

    if (m_title.isEmpty()) {
        m_errorMessage = "To delete, you must fill in the Name field!";
        emit errorMessageChanged();
        return;
    }

    if (m_addresses->deleteItem(m_title)) {
        m_addressPalette->selectNext();
        m_addressPalette->selectItem();
    }
}

bool OpenApiExporterViewModel::isHasFewBodies(int identifier) noexcept
{
    auto route = m_routeList->getRouteByIndex(identifier);

    m_bodyTypes = route->bodyTypes();
    emit bodyTypesChanged();

    return route->isHasMoreThenOneBody();
}

void OpenApiExporterViewModel::clearErrorMessage() noexcept
{
    m_errorMessage = "";
    emit errorMessageChanged();
}

void OpenApiExporterViewModel::parseJsonSpecification(const QString& json) noexcept
{
    clearErrorMessage();

    QJsonParseError parseError;
    auto document = QJsonDocument::fromJson(json.toUtf8(), &parseError);
    if (parseError.error != 0) {
        m_errorMessage = "Unable to parse JSON response. Make sure the content contains valid JSON!";
        emit errorMessageChanged();
        return;
    }
    auto rootObject = document.object();
    if (!rootObject.contains("openapi")) {
        //TODO: show error openapi parameter not defined
        return;
    }
    auto openApiVersion = rootObject.value("openapi").toString();
    if (!openApiVersion.startsWith("3.")) {
        //TODO: show error openapi version not supported
        return;
    }

    if (!rootObject.contains("paths")) {
        //TODO: show error openapi parameter not defined
        return;
    }

    auto routes = parseRoutes(rootObject.value("paths").toObject(), rootObject);

    auto options = new OpenApiRoutesOptions();

    if (rootObject.contains("components")) {
        auto componentsObject = rootObject.value("components").toObject();
        if (componentsObject.contains("securitySchemes")) {
            auto securitySchemas = componentsObject.value("securitySchemes").toObject();
            parseSecuritySchemas(options, securitySchemas);
        }
    }

    if (rootObject.contains("security")) {
        parseSecurity(options, rootObject.value("security").toArray());
    }

    m_routes.insert(m_url, std::make_tuple(options, routes));
}

void OpenApiExporterViewModel::parseSecuritySchemas(OpenApiRoutesOptions *options, const QJsonObject &schemas) noexcept
{
    foreach (auto key, schemas.keys()) {
        auto schemaObject = schemas.value(key).toObject();
        options->addAuthorizationScheme(
            schemaObject.contains("type") ? schemaObject.value("type").toString() : "",
            schemaObject.contains("in") ? schemaObject.value("in").toString() : "",
            schemaObject.contains("name") ? schemaObject.value("name").toString() : "",
            schemaObject.contains("scheme") ? schemaObject.value("scheme").toString() : "",
            schemaObject.contains("openIdConnectUrl") ? schemaObject.value("openIdConnectUrl").toString() : "",
            key
        );
    }
}

void OpenApiExporterViewModel::parseSecurity(OpenApiRoutesOptions *options, const QJsonArray &securities) noexcept
{
    auto iterator = 0;
    foreach (auto security, securities) {
        options->addSecurityMap();
        auto itemObject = security.toObject();
        foreach (auto key, itemObject.keys()) {
            auto scopes = itemObject.value(key).toArray();
            if (scopes.count() == 0) { // case where `'key': []`
                options->addSecurity(iterator, key, "");
            } else { // case where `'key': ['write:pets', 'read:pets']`
                foreach (auto scope, scopes) {
                    options->addSecurity(iterator, key, scope.toString());
                }
            }
        }
        iterator++;
    }
}

QList<OpenApiRouteModel*> OpenApiExporterViewModel::parseRoutes(QJsonObject routeObject, QJsonObject rootObject) noexcept
{
    auto routePaths = routeObject.keys();
    QList<OpenApiRouteModel*> routes;
    int iterator = 0;
    QMap<QString, QString> completedTypes;

    foreach (auto routePath, routePaths) {
        auto route = routeObject.value(routePath);

        auto methodsObject = route.toObject();
        foreach (auto method, methodsObject.keys()) {
            auto model = new OpenApiRouteModel();
            model->setPath(routePath);
            model->setMethod(method);
            model->setIdentifier(iterator);

            auto methodObject = methodsObject.value(method).toObject();
            if (methodObject.contains("summary")) model->setSummary(methodObject.value("summary").toString());

            if (methodObject.contains("parameters")) parseParameters(model, methodObject.value("parameters").toArray());

            if (methodObject.contains("requestBody")) {
                auto requestBodyObject = methodObject.value("requestBody").toObject();
                if (requestBodyObject.contains("content")) {
                    auto mimeTypeObject = requestBodyObject.value("content").toObject();
                    foreach (auto mimeType, mimeTypeObject.keys()) {
                        auto mimeTypeContentObject = mimeTypeObject.value(mimeType).toObject();
                        if (mimeTypeContentObject.contains("schema")) {
                            auto schema = mimeTypeContentObject.value("schema").toObject();
                            auto isJson = mimeType.contains("json");
                            auto isXml = mimeType.contains("xml");

                            if (schema.contains("$ref")) {
                                auto reference = schema.value("$ref").toString() + (isJson ? ".json" : ".xml");
                                if (completedTypes.contains(reference)) {
                                    model->addBodyType(mimeType, completedTypes.value(reference));
                                    continue;
                                }
                            }

                            if (isJson) {
                                auto bodyModel = parseJsonBodyModel(schema, rootObject);
                                model->addBodyType(mimeType, bodyModel);
                                if (schema.contains("$ref")) completedTypes.insert(schema.value("$ref").toString() + ".json", bodyModel);
                                continue;
                            }
                            if (isXml) {
                                //TODO: implement xml generation
                            }
                        }
                    }
                }
            }

            if (methodObject.contains("security")) {
                auto securityArray = methodObject.value("security").toArray();
                auto iterator = 0;
                foreach (auto security, securityArray) {
                    model->addSecurityMap();
                    auto itemObject = security.toObject();
                    foreach (auto key, itemObject.keys()) {
                        auto scopes = itemObject.value(key).toArray();
                        if (scopes.count() == 0) { // case where `'key': []`
                            model->addSecurity(iterator, key, "");
                        } else { // case where `'key': ['write:pets', 'read:pets']`
                            foreach (auto scope, scopes) {
                                model->addSecurity(iterator, key, scope.toString());
                            }
                        }
                    }
                    iterator++;
                }
            }
            routes.append(model);
            iterator++;
        }
    }

    return routes;
}

QString OpenApiExporterViewModel::parseJsonBodyModel(const QJsonObject &schemaModel, const QJsonObject &rootObject)
{
    QSet<QString> usedModels;
    auto value = parseBodyModelLevel(schemaModel, rootObject, usedModels);

    QJsonDocument document;

    if (value.isObject()) {
        document = QJsonDocument(value.toObject());
    }

    if (value.isArray()) {
        document = QJsonDocument(value.toArray());
    }

    if (value.isBool() || value.isDouble() || value.isString() || value.isNull() || value.isUndefined()) {
        document = QJsonDocument::fromVariant(value.toVariant());
    }

    return document.toJson();
}

QJsonValue OpenApiExporterViewModel::parseBodyModelLevel(const QJsonObject &schemaModel, const QJsonObject &rootObject, QSet<QString> usedModels)
{
    QJsonObject actualSchemaObject;
    QString reference;
    if (schemaModel.contains("$ref")) {
        reference = schemaModel.value("$ref").toString();
        if (usedModels.contains(reference)) return QJsonValue("");
        actualSchemaObject = getModelByReference(reference, rootObject);
        usedModels.insert(reference);
    }

    if (schemaModel.contains("type")) actualSchemaObject = schemaModel;

    auto type = actualSchemaObject.value("type");
    if (type == "object") {
        QJsonObject object;
        if (actualSchemaObject.contains("properties")) {
            auto properties = actualSchemaObject.value("properties").toObject();
            foreach (auto propertyKey, properties.keys()) {
                object[propertyKey] = parseBodyModelLevel(properties[propertyKey].toObject(), rootObject, usedModels);
            }
        } else if (actualSchemaObject.contains("additionalProperties")) {
            auto additionalPropertiesObject = actualSchemaObject.value("additionalProperties").toObject();
            object["additionalProp"] = parseBodyModelLevel(additionalPropertiesObject, rootObject, usedModels);
        }
        return object;
    }
    if (type == "array") {
        QJsonArray array;
        if (schemaModel.contains("items")) {
            auto itemObject = parseBodyModelLevel(schemaModel.value("items").toObject(), rootObject, usedModels);
            array.append(itemObject);
        }
        return array;
    }
    if (type == "string") return QJsonValue("");
    if (type == "integer") return QJsonValue(0);
    if (type == "number") return QJsonValue(0.0f);
    if (type == "boolean") return QJsonValue(false);

    return QJsonValue();
}

QJsonObject OpenApiExporterViewModel::getModelByReference(const QString &reference, const QJsonObject &rootObject)
{
    if (reference == nullptr) return QJsonObject();
    if (reference.isEmpty()) return QJsonObject();
    if (!reference.startsWith("#/")) return QJsonObject();

    auto path = QString(reference);
    auto items = path.replace("#", "").split("/", Qt::SkipEmptyParts);

    auto currentObject = rootObject;
    auto countTakes = 0;

    foreach (auto item, items) {
        if (currentObject.contains(item)) {
            currentObject = currentObject.value(item).toObject();
            countTakes++;
        }
    }

    if (countTakes != items.count()) return QJsonObject();
    return currentObject;
}

void OpenApiExporterViewModel::parseParameters(OpenApiRouteModel* routeModel, const QJsonArray& parametersArray) noexcept
{
    foreach (auto parametersItem, parametersArray) {
        auto parametersObject = parametersItem.toObject();
        if (!parametersObject.contains("name") || !parametersObject.contains("in")) return;

        auto model = new OpenApiParameterModel();
        model->setName(parametersObject.value("name").toString());
        model->setIn(parametersObject.value("in").toString());

        if (parametersObject.contains("required")) {
            model->setRequired(parametersObject.value("required").toBool());
        }

        if (parametersObject.contains("deprecated")) {
            //TODO: implement in future
        }
        if (parametersObject.contains("schema")) {
            //TODO: implement in future
        }

        routeModel->addParameter(model);
    }
}

void OpenApiExporterViewModel::removeLoadedRoutes(const QString &url)
{
    auto routeData = m_routes.value(url);
    auto options = std::get<0>(routeData);
    auto routesList = std::get<1>(routeData);
    foreach (auto route, routesList) {
        route->clear();
        delete route;
    }

    routesList.clear();

    options->clearAuthorizationSchemes();
    options->clearSecurities();

    delete options;

    m_routes.remove(m_url);
}

void OpenApiExporterViewModel::loadRoutes(const QString &addressId)
{
    auto cachePath = getCachePath(addressId);
    if (QFile::exists(cachePath)) {
        //createIfNotExistsFile(cachePath, "[]");

    }
}

QList<OpenApiRouteModel*> OpenApiExporterViewModel::readCache(const QString& cacheFile)
{
    QList<OpenApiRouteModel*> result;

    auto file = QFile(getCachePath(cacheFile));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return result;

    auto content = file.readAll();

    file.close();

    auto document = QJsonDocument::fromJson(content);
    auto items = document.array();
    foreach (auto item, items) {
        if (!item.isObject()) continue;

        auto model = new OpenApiRouteModel();
        model->fromJsonObject(item.toObject());

        result.append(model);
    }

    return result;
}

void OpenApiExporterViewModel::writeCache(const QString& cacheFile, const QList<OpenApiRouteModel*>& routes)
{
    QJsonArray array;

    foreach (auto route, routes) {
        array.append(route->toJsonObject());
    }

    QFile file(getCachePath(cacheFile));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QJsonDocument document(array);
    file.write(document.toJson());
    file.close();
}

void OpenApiExporterViewModel::fillMappings()
{
    m_shortcutCommandMapping.insert("f5", m_loadSchemaCommand);

    m_shortcutCommandMapping.insert("f4", m_cancelLoadSchemaCommand);
    m_shortcutCommandMapping.insert("control-b", m_cancelLoadSchemaCommand);

    m_shortcutCommandMapping.insert("control-s", m_saveSchemaCommand);
    m_shortcutCommandMapping.insert("control-tab", m_changeSelectedSchemaCommand);
    m_shortcutCommandMapping.insert("control-backspace", m_changeSelectedSchemaCommand);
    m_shortcutCommandMapping.insert("control-h", m_helpCommand);
    m_shortcutCommandMapping.insert("f1", m_helpCommand);
    m_shortcutCommandMapping.insert("escape", m_closeWindowCommand);
    m_shortcutCommandMapping.insert("control-d", m_deleteSelectedSchemaCommand);
#ifndef Q_OS_MACOS
    m_shortcutCommandMapping.insert("control-delete", m_deleteSelectedSchemaCommand);
#endif
#ifdef Q_OS_MACOS
    m_shortcutCommandMapping.insert("control-f11", m_toggleTabsCommand);
#else
    m_shortcutCommandMapping.insert("f11", m_toggleTabsCommand);
#endif
    m_shortcutCommandMapping.insert("pageup", m_previousRouteCommand);
    m_shortcutCommandMapping.insert("pagedown", m_nextRouteCommand);
    m_shortcutCommandMapping.insert("control-enter", m_addAsNewQueryRouteCommand);
    m_shortcutCommandMapping.insert("shift-enter", m_addToCurrentQueryRouteCommand);
}

void OpenApiExporterViewModel::fillCommands()
{
    m_shortcutCommands.insert(m_loadSchemaCommand, "Load OpenAPI schema");
    m_shortcutCommands.insert(m_cancelLoadSchemaCommand, "Cancel OpenAPI schema loading if it is in progress");
    m_shortcutCommands.insert(m_saveSchemaCommand, "Edit scheme or add new scheme (identifier for scheme is title)");
    m_shortcutCommands.insert(m_changeSelectedSchemaCommand, "holding Ctrl/Command and then pressing Tab/Backspace to change OpenAPI schema");
    m_shortcutCommands.insert(m_helpCommand, "Show interactive help for shortcuts");
    m_shortcutCommands.insert(m_closeWindowCommand, "Close Export OpenAPI window");
    m_shortcutCommands.insert(m_deleteSelectedSchemaCommand, "Delete schema (identifier for scheme is title)");
    m_shortcutCommands.insert(m_toggleTabsCommand, "Toggle tabs betweens Exporter and Saved options");
    m_shortcutCommands.insert(m_previousRouteCommand, "Select previous route");
    m_shortcutCommands.insert(m_nextRouteCommand, "Select next route");
    m_shortcutCommands.insert(m_addAsNewQueryRouteCommand, "Add the selected route to a new query");
    m_shortcutCommands.insert(m_addToCurrentQueryRouteCommand, "Replace current query on selected route");
}

void OpenApiExporterViewModel::fillHelpShortcuts()
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
        auto joinedShortcuts = shortcuts.join(" or ");
        map["shortcuts"] = adjustShortcutsForDisplay(joinedShortcuts);

        m_shortcuts.append(map);
    }

    emit shortcutsChanged();
}

void OpenApiExporterViewModel::saveToAddressByTitle()
{
    clearErrorMessage();

    if (m_url.isEmpty() || m_baseUrl.isEmpty() || m_title.isEmpty()) {
        m_errorMessage = "For saving need fill Base URL, URL and Title";
        emit errorMessageChanged();
        return;
    }

    auto address = m_addressPalette->getSelectedAddressByTitle(m_title);

    if (address != nullptr) {
        address->setAddress(m_url);
        address->setBaseUrl(m_baseUrl);
        address->setSecurities(m_url);
        address->setFilter(m_routeList->filter());
        address->setSecurities(m_authMethod);

        m_addresses->refreshItems();
    } else {
        addCurrentToAddresses();
    }
}

void OpenApiExporterViewModel::loadSchemeFromArray(const QByteArray &array, int format)
{
    //TODO: Support YAML
    if (format == m_JsonSchemeFormat) {
        auto future = QtConcurrent::run(&OpenApiExporterViewModel::parseJsonSpecification, this, array);
        m_schemeWatcher->setFuture(future);
    }
}

void OpenApiExporterViewModel::requestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        m_loading = false;
        m_errorMessage = reply->errorString();
        emit loadingChanged();
        emit errorMessageChanged();
        return;
    }

    auto scheme = reply->readAll();

    loadSchemeFromArray(scheme, m_JsonSchemeFormat);
}

void OpenApiExporterViewModel::parsingFinished()
{
    m_loading = false;
    m_currentReply = nullptr;
    emit loadingChanged();
    emit alreadyLoadedChanged();

    if (m_errorMessage.isEmpty()) m_routeList->setupRoutes(std::get<1>(m_routes[m_url]));
}

void OpenApiExporterViewModel::addressListChanged()
{
    m_addressPalette->refresh(true);
}

void OpenApiExporterViewModel::addressItemSelected(const QUuid &id)
{
    auto selectedItem = m_addressPalette->getSelectedAddressById(id);
    if (selectedItem == nullptr) return;

    m_url = selectedItem->address();
    emit urlChanged();
    setBaseUrl(selectedItem->baseUrl());
    setTitle(selectedItem->title());
    setAuthMethod(selectedItem->securities());
    m_routeList->setFilter(selectedItem->filter());
    if (m_routes.contains(m_url)) {
        m_routeList->setupRoutes(std::get<1>(m_routes[m_url]));
        m_routeList->refresh();
    } else {
        loadOpenApiScheme();
    }
}
