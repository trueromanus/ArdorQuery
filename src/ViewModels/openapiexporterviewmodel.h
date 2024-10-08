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
#include <QFutureWatcher>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include "../ListModels/openapiaddresseslistmodel.h"
#include "../Models/openapiroutemodel.h"
#include "../ListModels/openapirouteslistmodel.h"
#include "../ListModels/addressespalettelistmodel.h"
#include "../Models/openapiroutesoptions.h"

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
    Q_PROPERTY(QString authMethod READ authMethod WRITE setAuthMethod NOTIFY authMethodChanged)
    Q_PROPERTY(bool openedCommandPalette READ openedCommandPalette WRITE setOpenedCommandPalette NOTIFY openedCommandPaletteChanged)
    Q_PROPERTY(AddressesPaletteListModel* addressPalette READ addressPalette NOTIFY addressPaletteChanged)
    Q_PROPERTY(QStringList tabs READ tabs NOTIFY tabsChanged)
    Q_PROPERTY(QString selectedTab READ selectedTab WRITE setSelectedTab NOTIFY selectedTabChanged)
    Q_PROPERTY(bool exporterPage READ exporterPage NOTIFY exporterPageChanged)
    Q_PROPERTY(bool savedOptionsPage READ savedOptionsPage NOTIFY savedOptionsPageChanged)
    Q_PROPERTY(int prepareIdentifier READ prepareIdentifier WRITE setPrepareIdentifier NOTIFY prepareIdentifierChanged)
    Q_PROPERTY(QString prepareBodyType READ prepareBodyType WRITE setPrepareBodyType NOTIFY prepareBodyTypeChanged)
    Q_PROPERTY(QStringList bodyTypes READ bodyTypes NOTIFY bodyTypesChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QList<QVariantMap> shortcuts READ shortcuts NOTIFY shortcutsChanged)

private:
    OpenApiAddressesListModel* m_addresses { new OpenApiAddressesListModel(this) };
    QMap<QString, std::tuple<OpenApiRoutesOptions*,QList<OpenApiRouteModel*>>> m_routes { QMap<QString, std::tuple<OpenApiRoutesOptions*,QList<OpenApiRouteModel*>>>() };
    OpenApiRoutesListModel* m_routeList { new OpenApiRoutesListModel(this) };
    AddressesPaletteListModel* m_addressPalette { new AddressesPaletteListModel(this) };
    QString m_url { "" };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QNetworkReply* m_currentReply { nullptr };
    QString m_baseUrl { "" };
    QString m_title { "" };
    QString m_authMethod  { "" };
    bool m_loading { false };
    bool m_helpVisible { false };
    QStringList m_tabs { QStringList() };
    QFutureWatcher<void>* m_schemeWatcher { new QFutureWatcher<void>(this) };
    bool m_openedCommandPalette { false };
    int m_prepareIndetifier { -1 };
    QString m_prepareBodyType { "" };
    QList<QVariantMap> m_shortcuts { QList<QVariantMap>() };
    QMap<QString, QString> m_shortcutCommandMapping { QMap<QString, QString>() };
    QMap<QString, QString> m_shortcutCommands { QMap<QString, QString>() };
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
    const QString Importer { "Importer" };
    const QString SavedOptions { "Saved options" };
    const QString m_helpCommand { "exportopenapihelp" };
    const QString m_closeWindowCommand { "exportopenapiclosewindow" };
    const QString m_changeSelectedSchemaCommand { "exportopenapichangeselectedschema" };
    const QString m_addSchemaCommand { "exportopenapiaddschema" };
    const QString m_saveSchemaCommand { "exportopenapisaveschema" };
    const QString m_loadSchemaCommand { "exportopenapiloadschema" };
    const QString m_cancelLoadSchemaCommand { "exportopenapicancelloadschema" };
    const QString m_deleteSelectedSchemaCommand { "exportopenapicanceldeleteselectedschema" };
    const QString m_toggleTabsCommand { "exportopenapitoggletabs" };
    const QString m_nextRouteCommand { "exportopenapinextroute" };
    const QString m_previousRouteCommand { "exportopenapipreviousroute" };
    const QString m_addAsNewQueryRouteCommand { "exportopenapiaddasnewquery" };
    const QString m_addToCurrentQueryRouteCommand { "exportopenapiaddtocurrentquery" };
    const int m_JsonSchemeFormat { 1 };
    QString m_selectedTab { Importer };
    QStringList m_bodyTypes { QStringList() };
    QString m_errorMessage { "" };

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

    QString authMethod() const noexcept { return m_authMethod; }
    void setAuthMethod(const QString& authMethod) noexcept;

    bool openedCommandPalette() const noexcept { return m_openedCommandPalette; }
    void setOpenedCommandPalette(bool openedCommandPalette) noexcept;

    OpenApiRouteModel* getRouteFromOpenApiByIndex(int index) const noexcept;
    OpenApiRoutesOptions* getRoutesOptions();

    QStringList tabs() const noexcept { return m_tabs; }

    QString selectedTab() const noexcept { return m_selectedTab; }
    void setSelectedTab(const QString& selectedTab) noexcept;

    bool exporterPage() const noexcept { return m_selectedTab == Importer; }
    bool savedOptionsPage() const noexcept { return m_selectedTab == SavedOptions; }

    int prepareIdentifier() const noexcept { return m_prepareIndetifier; }
    void setPrepareIdentifier(int prepareIdentifier) noexcept;

    QString prepareBodyType() const noexcept { return m_prepareBodyType; }
    void setPrepareBodyType(const QString& prepareBodyType) noexcept;

    QStringList bodyTypes() const noexcept { return m_bodyTypes; }

    QString errorMessage() const noexcept { return m_errorMessage; }

    QList<QVariantMap> shortcuts() const noexcept { return m_shortcuts; }

    void cancelCurrentRequest() noexcept;
    void clearError() noexcept;
    void setupError(const QString& message) noexcept;
    Q_INVOKABLE void loadOpenApiScheme() noexcept;
    Q_INVOKABLE void setUrl(const QString& url) noexcept;
    Q_INVOKABLE bool shortcutHandler(const QString& shortcut) noexcept;
    Q_INVOKABLE void addCurrentToAddresses() noexcept;
    Q_INVOKABLE void togglePages() noexcept;
    Q_INVOKABLE void deleteSelectedAddress() noexcept;
    Q_INVOKABLE bool isHasFewBodies(int identifier) noexcept;
    Q_INVOKABLE void clearErrorMessage() noexcept;

private:
    void parseJsonSpecification(const QString& json) noexcept;
    void parseSecuritySchemas(OpenApiRoutesOptions* options, const QJsonObject& schemas) noexcept;
    void parseSecurity(OpenApiRoutesOptions* options, const QJsonArray& securities) noexcept;
    QList<OpenApiRouteModel*> parseRoutes(QJsonObject routeObject, QJsonObject rootObject) noexcept;
    QString parseJsonBodyModel(const QJsonObject& schemaModel, const QJsonObject& rootObject);
    QJsonValue parseBodyModelLevel(const QJsonObject& schemaModel, const QJsonObject& rootObject, QSet<QString> usedModels);
    QJsonObject getModelByReference(const QString& reference, const QJsonObject& rootObject);
    void parseParameters(OpenApiRouteModel* routeModel, const QJsonArray& parametersArray) noexcept;
    void removeLoadedRoutes(const QString& url);
    void loadRoutes(const QString& addressId);
    QList<OpenApiRouteModel*> readCache(const QString& cacheFile);
    void writeCache(const QString& cacheFile, const QList<OpenApiRouteModel*>& routes);
    void fillMappings();
    void fillCommands();
    void fillHelpShortcuts();
    void saveToAddressByTitle();
    void loadSchemeFromArray(const QByteArray& array, int format);

private slots:
    void requestFinished(QNetworkReply *reply);
    void parsingFinished();
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
    void authMethodChanged();
    void prepareIdentifierChanged();
    void bodyTypesChanged();
    void prepareBodyTypeChanged();
    void errorMessageChanged();
    void needCloseWindow();
    void shortcutsChanged();
    void selectRouteFromList(int identifier, bool replaceCurrent);

};

#endif // OPENAPIEXPORTERVIEWMODEL_H
