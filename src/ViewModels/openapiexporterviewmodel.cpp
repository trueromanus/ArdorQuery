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

    if (rootObject.contains("paths")) {
        //TODO: show error openapi parameter not defined
        return;
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
