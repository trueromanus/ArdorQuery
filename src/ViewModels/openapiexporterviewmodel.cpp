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
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &OpenApiExporterViewModel::requestFinished);
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

bool OpenApiExporterViewModel::checkIsLoading(const QString &url) const noexcept
{
    return m_routes.contains(m_url);
}

void OpenApiExporterViewModel::setUrl(const QString &url) noexcept
{
    //TODO: validating url
    m_url = url;
    //TODO: save to addresses
    emit urlChanged();
    emit alreadyLoadedChanged();
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
}
