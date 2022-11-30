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
#include "openapiexporterviewmodel.h"

OpenApiExporterViewModel::OpenApiExporterViewModel(QObject *parent)
    : QObject{parent}
{
    m_addressPalette->setup(m_addresses->getAddresses());

    m_tabs.append(Exporter);
    m_tabs.append(SavedOptions);

    connect(m_networkManager, &QNetworkAccessManager::finished, this, &OpenApiExporterViewModel::requestFinished);
    connect(m_addresses, &OpenApiAddressesListModel::addressesChanged, this, &OpenApiExporterViewModel::addressListChanged);
    connect(m_addressPalette, &AddressesPaletteListModel::itemSelected, this, &OpenApiExporterViewModel::addressItemSelected);
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

void OpenApiExporterViewModel::setSelectedTab(const QString &selectedTab) noexcept
{
    if (m_selectedTab == selectedTab) return;

    m_selectedTab = selectedTab;
    emit selectedTabChanged();
    emit exporterPageChanged();
    emit savedOptionsPageChanged();
}

void OpenApiExporterViewModel::loadOpenApiScheme() noexcept
{
    if (m_loading) return;
    if (m_routes.contains(m_url)) removeLoadedRoutes(m_url);

    QNetworkRequest request(m_url);

    m_networkManager->get(request);

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

bool OpenApiExporterViewModel::keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept
{
    if (shift || alt) {

    }

    // F5 or Ctrl-Z
    if (((nativeCode == 44 || key == Qt::Key_Z) && control) || (nativeCode == 63 || key == Qt::Key_F5)) {
        loadOpenApiScheme();
        return true;
    }

    // Ctrl-Insert
    if (key == Qt::Key_Insert && control) {
        addCurrentToAddresses();
        return true;
    }

    // Ctrl-Tab
    if (key == Qt::Key_Tab && control) {
        if (!m_openedCommandPalette) {
            m_openedCommandPalette = true;
            m_addressPalette->refresh(true);
            emit openedCommandPaletteChanged();
        } else {
            m_addressPalette->selectNext();
        }
        return true;
    }

    // Ctrl-H or F1
#ifdef Q_OS_WIN
    if (((nativeCode == 35 || key == Qt::Key_H) && control && !shift && !alt) || key == Qt::Key_F1) {
        setHelpVisible(!m_helpVisible);
        return true;
    }
#else
    if ((key == Qt::Key_H && control) || key == Qt::Key_F1) {
        setHelpVisible(!m_helpVisible);
        return true;
    }

#endif

    return false;
}

void OpenApiExporterViewModel::keysReleased(int key) noexcept
{
    if (key == Qt::Key_Control && m_openedCommandPalette) {
        m_openedCommandPalette = false;
        m_addressPalette->selectItem();
        emit openedCommandPaletteChanged();
    }
}

void OpenApiExporterViewModel::addCurrentToAddresses() noexcept
{
    if (m_url.isEmpty() && m_baseUrl.isEmpty()) return;

    m_addresses->addAddress(m_title.isEmpty() ? m_url : m_title, m_url, m_baseUrl, m_routeList->filter());
}

void OpenApiExporterViewModel::parseJsonSpecification(const QString& json) noexcept
{
    auto document = QJsonDocument::fromJson(json.toUtf8());
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

    parseRoutes(rootObject.value("paths").toObject());
}

void OpenApiExporterViewModel::parseRoutes(QJsonObject routeObject) noexcept
{
    auto routePaths = routeObject.keys();
    QList<OpenApiRouteModel*> routes;

    foreach (auto routePath, routePaths) {
        auto route = routeObject.value(routePath);

        auto methodsObject = route.toObject();
        foreach (auto method, methodsObject.keys()) {
            auto model = new OpenApiRouteModel();
            model->setPath(routePath);
            model->setMethod(method);

            auto methodObject = methodsObject.value(method).toObject();
            if (methodObject.contains("summary")) model->setSummary(methodObject.value("summary").toString());

            if (methodObject.contains("parameters")) parseParameters(model, methodObject.value("parameters").toArray());

            if (methodObject.contains("requestBody")) {
                //TODO: body scheme
            }
            routes.append(model);
        }
    }

    m_routes.insert(m_url, routes);
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
    auto routesList = m_routes.value(url);
    foreach (auto route, routesList) {
        route->clearParameters();
        delete route;
    }

    routesList.clear();

    m_routes.remove(m_url);
}

void OpenApiExporterViewModel::requestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        //TODO: show error to user reply->errorString());
        return;
    }

    //TODO: Support YAML

    auto scheme = reply->readAll();
    parseJsonSpecification(scheme);

    m_loading = false;
    emit loadingChanged();
    emit alreadyLoadedChanged();

    m_routeList->setupRoutes(m_routes[m_url]);
}

void OpenApiExporterViewModel::addressListChanged()
{
    m_addressPalette->refresh(true);
}

void OpenApiExporterViewModel::addressItemSelected(const QUuid &id)
{
    auto selectedItem = m_addressPalette->getSelectedAddressById(id);
    m_url = selectedItem->address();
    emit urlChanged();
    setBaseUrl(selectedItem->baseUrl());
    setTitle(selectedItem->title());
    m_routeList->setFilter(selectedItem->filter());
    if (m_routes.contains(m_url)) {
        m_routeList->setupRoutes(m_routes[m_url]);
        m_routeList->refresh();
    } else {
        loadOpenApiScheme();
    }
}
