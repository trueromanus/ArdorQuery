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

#ifndef OPENAPIEXPORTERVIEWMODEL_H
#define OPENAPIEXPORTERVIEWMODEL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include "../ListModels/openapiaddresseslistmodel.h"
#include "../Models/openapiroutemodel.h"
#include "../Models/openapiparametermodel.h"
#include "../ListModels/openapirouteslistmodel.h"

class OpenApiExporterViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OpenApiAddressesListModel* addresses READ addresses NOTIFY addressesChanged)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(bool alreadyLoaded READ alreadyLoaded NOTIFY alreadyLoadedChanged)
    Q_PROPERTY(OpenApiRoutesListModel* routeList READ routeList NOTIFY routeListChanged)
    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)

private:
    OpenApiAddressesListModel* m_addresses { new OpenApiAddressesListModel(this) };
    QMap<QString, QList<OpenApiRouteModel*>> m_routes { QMap<QString, QList<OpenApiRouteModel*>>() };
    OpenApiRoutesListModel* m_routeList { new OpenApiRoutesListModel(this) };
    QString m_url { "" };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QString m_baseUrl { "" };
    bool m_loading { false };
    const QString IntegerType { "integer" };
    const QString DoubleType { "number" };
    const QString StringType { "string" };
    const QString BooleanType { "boolean" };
    const QString ArrayType { "array" };
    const QString ObjectType { "object" };
    const QString RefField { "$ref" };
    const QString QueryIn { "query" };
    const QString HeaderIn { "header" };
    const QString PathIn { "path" };
    const QString CookieIn { "cookie" };

public:
    explicit OpenApiExporterViewModel(QObject *parent = nullptr);

    OpenApiAddressesListModel* addresses() const noexcept { return m_addresses; }

    OpenApiRoutesListModel* routeList() const noexcept { return m_routeList; }

    QString url() const noexcept { return m_url; }

    bool loading() const noexcept { return m_loading; }

    bool alreadyLoaded() const noexcept { return m_routes.contains(m_url); }

    QString baseUrl () const noexcept { return m_baseUrl; }
    void setBaseUrl(const QString& baseUrl) noexcept;

    OpenApiRouteModel* getRouteFromOpenApiByIndex(int index) const noexcept;

    Q_INVOKABLE void loadOpenApiScheme() noexcept;
    Q_INVOKABLE void setUrl(const QString& url) noexcept;

private:
    void parseJsonSpecification(const QString& json) noexcept;
    void parseRoutes(QJsonObject routeObject) noexcept;
    void parseParameters(OpenApiRouteModel* routeModel, const QJsonArray& parametersArray) noexcept;
    void removeLoadedRoutes(const QString& url);

private slots:
    void requestFinished(QNetworkReply *reply);

signals:
    void addressesChanged();
    void urlChanged();
    void loadingChanged();
    void alreadyLoadedChanged();
    void routeListChanged();
    void baseUrlChanged();

};

#endif // OPENAPIEXPORTERVIEWMODEL_H
