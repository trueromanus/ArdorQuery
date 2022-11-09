#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "openapiexporterviewmodel.h"
#include "../Models/openapiroutemodel.h"

OpenApiExporterViewModel::OpenApiExporterViewModel(QObject *parent)
    : QObject{parent}
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &OpenApiExporterViewModel::requestFinished);
}

void OpenApiExporterViewModel::loadOpenApiScheme() noexcept
{
    QNetworkRequest request(m_url);

    m_networkManager->get(request);

    m_loading = true;
    emit loadingChanged();
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

    foreach (auto routePath, routePaths) {
        auto route = routeObject.value(routePath);

        auto methodsObject = route.toObject();
        foreach (auto method, methodsObject.keys()) {
            OpenApiRouteModel model;
            model.setPath(routePath);
            model.setMethod(method);

            auto methodObject = methodsObject.value(method).toObject();
            if (methodObject.contains("summary")) model.setSummary(methodObject.value("summary").toString());

            if (methodObject.contains("parameters")) {
                //TODO: read parameters
            }
            if (methodObject.contains("requestBody")) {
                //TODO: body scheme
            }
        }
    }
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
}
