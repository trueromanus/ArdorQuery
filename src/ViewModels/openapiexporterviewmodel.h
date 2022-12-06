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
#include "../ListModels/addressespalettelistmodel.h"

class OpenApiExporterViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OpenApiAddressesListModel* addresses READ addresses NOTIFY addressesChanged)
    Q_PROPERTY(QString url READ url NOTIFY urlChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(bool alreadyLoaded READ alreadyLoaded NOTIFY alreadyLoadedChanged)
    Q_PROPERTY(OpenApiRoutesListModel* routeList READ routeList NOTIFY routeListChanged)
    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(bool helpVisible READ helpVisible WRITE setHelpVisible NOTIFY helpVisibleChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool openedCommandPalette READ openedCommandPalette WRITE setOpenedCommandPalette NOTIFY openedCommandPaletteChanged)
    Q_PROPERTY(AddressesPaletteListModel* addressPalette READ addressPalette NOTIFY addressPaletteChanged)
    Q_PROPERTY(QStringList tabs READ tabs NOTIFY tabsChanged)
    Q_PROPERTY(QString selectedTab READ selectedTab WRITE setSelectedTab NOTIFY selectedTabChanged)
    Q_PROPERTY(bool exporterPage READ exporterPage NOTIFY exporterPageChanged)
    Q_PROPERTY(bool savedOptionsPage READ savedOptionsPage NOTIFY savedOptionsPageChanged)

private:
    OpenApiAddressesListModel* m_addresses { new OpenApiAddressesListModel(this) };
    QMap<QString, QList<OpenApiRouteModel*>> m_routes { QMap<QString, QList<OpenApiRouteModel*>>() };
    OpenApiRoutesListModel* m_routeList { new OpenApiRoutesListModel(this) };
    AddressesPaletteListModel* m_addressPalette { new AddressesPaletteListModel(this) };
    QString m_url { "" };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QString m_baseUrl { "" };
    QString m_title { "" };
    bool m_loading { false };
    bool m_helpVisible { false };
    QStringList m_tabs { QStringList() };
    bool m_openedCommandPalette { false };
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
    const QString Exporter { "Exporter" };
    const QString SavedOptions { "Saved options" };
    QString m_selectedTab { Exporter };

public:
    explicit OpenApiExporterViewModel(QObject *parent = nullptr);

    OpenApiAddressesListModel* addresses() const noexcept { return m_addresses; }

    OpenApiRoutesListModel* routeList() const noexcept { return m_routeList; }

    AddressesPaletteListModel* addressPalette() const noexcept { return m_addressPalette; }

    QString url() const noexcept { return m_url; }

    bool loading() const noexcept { return m_loading; }

    bool alreadyLoaded() const noexcept { return m_routes.contains(m_url); }

    QString baseUrl () const noexcept { return m_baseUrl; }
    void setBaseUrl(const QString& baseUrl) noexcept;

    bool helpVisible() const noexcept { return m_helpVisible; }
    void setHelpVisible(bool helpVisible) noexcept;

    QString title () const noexcept { return m_title; }
    void setTitle(const QString& title) noexcept;

    bool openedCommandPalette() const noexcept { return m_openedCommandPalette; }
    void setOpenedCommandPalette(bool openedCommandPalette) noexcept;

    OpenApiRouteModel* getRouteFromOpenApiByIndex(int index) const noexcept;

    QStringList tabs() const noexcept { return m_tabs; }

    QString selectedTab() const noexcept { return m_selectedTab; }
    void setSelectedTab(const QString& selectedTab) noexcept;

    bool exporterPage() const noexcept { return m_selectedTab == Exporter; }
    bool savedOptionsPage() const noexcept { return m_selectedTab == SavedOptions; }

    Q_INVOKABLE void loadOpenApiScheme() noexcept;
    Q_INVOKABLE void setUrl(const QString& url) noexcept;
    Q_INVOKABLE bool keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept;
    Q_INVOKABLE void keysReleased(int key) noexcept;
    Q_INVOKABLE void addCurrentToAddresses() noexcept;
    Q_INVOKABLE void togglePages() noexcept;
    Q_INVOKABLE void editInSelectedAddress() noexcept;

private:
    void parseJsonSpecification(const QString& json) noexcept;
    void parseRoutes(QJsonObject routeObject) noexcept;
    void parseParameters(OpenApiRouteModel* routeModel, const QJsonArray& parametersArray) noexcept;
    void removeLoadedRoutes(const QString& url);

private slots:
    void requestFinished(QNetworkReply *reply);
    void addressListChanged();
    void addressItemSelected(const QUuid& id);

signals:
    void addressesChanged();
    void urlChanged();
    void loadingChanged();
    void alreadyLoadedChanged();
    void routeListChanged();
    void baseUrlChanged();
    void helpVisibleChanged();
    void titleChanged();
    void openedCommandPaletteChanged();
    void addressPaletteChanged();
    void tabsChanged();
    void selectedTabChanged();
    void exporterPageChanged();
    void savedOptionsPageChanged();

};

#endif // OPENAPIEXPORTERVIEWMODEL_H
