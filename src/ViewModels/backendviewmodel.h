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

#ifndef BACKENDVIEWMODEL_H
#define BACKENDVIEWMODEL_H

#include <QObject>
#include "../ViewModels/httpperformerviewmodel.h"
#include "../ViewModels/textadvisorviewmodel.h"
#include "../ListModels/httprequestslistmodel.h"
#include "../ListModels/maintabslistmodel.h"
#include "../ViewModels/requestexternalviewmodel.h"
#include "../ListModels/notificationlistmodel.h"
#include "../ListModels/outputformatslistmodel.h"
#include "../ListModels/requestscommandpalettelistmodel.h"
#include "../ViewModels/openapiexporterviewmodel.h"
#include "../ListModels/globalvariableslistmodel.h"

class BackendViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HttpPerformerViewModel* requestPerformer READ requestPerformer NOTIFY requestPerformerChanged)
    Q_PROPERTY(TextAdvisorViewModel* textAdviser READ textAdviser NOTIFY textAdviserChanged)
    Q_PROPERTY(HttpRequestsListModel* requests READ requests NOTIFY requestsChanged)
    Q_PROPERTY(MainTabsListModel* tabs READ tabs NOTIFY tabsChanged)
    Q_PROPERTY(RequestExternalViewModel* requestExternal READ requestExternal NOTIFY requestExternalChanged)
    Q_PROPERTY(NotificationListModel* notifications READ notifications NOTIFY notificationsChanged)
    Q_PROPERTY(bool helpVisible READ helpVisible WRITE setHelpVisible NOTIFY helpVisibleChanged)
    Q_PROPERTY(OutputFormatsListModel* outputFormats READ outputFormats NOTIFY outputFormatsChanged)
    Q_PROPERTY(RequestsCommandPaletteListModel* requestsCommandPaletter READ requestsCommandPaletter NOTIFY requestsCommandPaletterChanged)
    Q_PROPERTY(bool openedCommandPalette READ openedCommandPalette NOTIFY openedCommandPaletteChanged)
    Q_PROPERTY(OpenApiExporterViewModel* openApiExporter READ openApiExporter NOTIFY openApiExporterChanged)
    Q_PROPERTY(GlobalVariablesListModel* globalVariables READ globalVariables NOTIFY globalVariablesChanged)
    Q_PROPERTY(QList<QVariantMap> shortcuts READ shortcuts NOTIFY shortcutsChanged)
    Q_PROPERTY(bool focusedHelpTextField READ focusedHelpTextField WRITE setFocusedHelpTextField NOTIFY focusedHelpTextFieldChanged FINAL)
    Q_PROPERTY(bool showGlobalVariablesPopup READ showGlobalVariablesPopup NOTIFY showGlobalVariablesPopupChanged FINAL)
    Q_PROPERTY(QString selectedGlobalVariable READ selectedGlobalVariable NOTIFY selectedGlobalVariableChanged FINAL)

private:
    HttpPerformerViewModel* m_requestPerformer { new HttpPerformerViewModel(this) };
    QSharedPointer<TextAdvisorViewModel> m_textAdviser { new TextAdvisorViewModel() };
    QScopedPointer<RequestExternalViewModel> m_requestExternal { new RequestExternalViewModel() };
    HttpRequestsListModel* m_requests { new HttpRequestsListModel(this) };
    MainTabsListModel* m_tabs { new MainTabsListModel(this) };
    NotificationListModel* m_notifications { new NotificationListModel(this) };
    OutputFormatsListModel* m_outputFormats { new OutputFormatsListModel(this) };
    bool m_helpVisible { false };
    RequestsCommandPaletteListModel* m_requestsCommandPaletter { new RequestsCommandPaletteListModel(this) };
    bool m_openedCommandPalette { false };
    OpenApiExporterViewModel* m_openApiExporter { new OpenApiExporterViewModel(this) };
    GlobalVariablesListModel* m_globalVariables { new GlobalVariablesListModel(this) };
    bool m_openApiHelpVisible { false };
    QList<QVariantMap> m_shortcuts { QList<QVariantMap>() };
    QMap<QString, QString> m_shortcutCommandMapping { QMap<QString, QString>() };
    QMap<QString, QString> m_shortcutCommands { QMap<QString, QString>() };
    bool m_focusedHelpTextField { false };
    bool m_showGlobalVariablesPopup { false };
    int m_selectedGlobalVariableIndex { -1 };
    QString m_selectedGlobalVariable { "" };
    const QString m_changeSelectedQueryCommand { "changeselectedquery" };
    const QString m_performQueriesMultipleCommand { "performqueriesmultiple" };
    const QString m_performQueryCommand { "performquery" };
    const QString m_cancelQueryCommand { "cancelmquery" };
    const QString m_saveToClipboardCommand { "savetoclipboard" };
    const QString m_helpCommand { "help" };
    const QString m_replaceFromClipboardCommand { "replacefromclipboard" };
    const QString m_appendFromClipboardCommand { "appendfromclipboard" };
    const QString m_globalVariablesCommand { "globalvariables" };
    const QString m_addGlobalVariablesCommand { "addglobalvariable" };
    const QString m_opeApiExportCommand { "openApiExport" };
    const QString m_removeSelectedFieldCommand { "removeselectedfield" };
    const QString m_removeAllFieldCommand { "removeallfield" };
    const QString m_addLineBelowCommand { "addLineBelow" };
    const QString m_addLineAboveCommand { "addLineAbove" };
    const QString m_addLineToEndCommand { "addLineToTop" };
    const QString m_sortAscendingCommand { "sortAscending" };
    const QString m_sortDescendingCommand { "sortDescending" };
    const QString m_toggleTabsCommand { "toggletabs" };
    const QString m_addQueryCommand { "addquery" };
    const QString m_deleteSelectedQueryCommand { "deleteselectedquery" };
    const QString m_selectLastFieldCommand { "selectlastfield" };
    const QString m_selectNextFieldCommand { "selectnextfield" };
    const QString m_selectTopFieldCommand { "selecttopfield" };
    const QString m_selectPreviousFieldCommand { "selectpreviousfield" };
    const QString m_copyHeadersToClipboardCommand { "copyheaderstoclipboard" };
    const QString m_copyBodyToClipboardCommand { "copybodytoclipboard" };
    const QString m_openFromFileCommand { "openfromfile" };
    const QString m_saveToFileCommand { "savetofile" };
    const QString m_generateImageToFileCommand { "generateimagetofile" };
    const QString m_generateImageToClipboardCommand { "generateimagetoclipboard" };
    const QString m_nextFindedResultCommand { "nextfindedresult" };
    const QString m_previousFindedResultCommand { "previousfindedresult" };

public:
    explicit BackendViewModel(QObject *parent = nullptr);

    HttpPerformerViewModel* requestPerformer() const noexcept { return m_requestPerformer; }
    TextAdvisorViewModel* textAdviser() const noexcept { return m_textAdviser.get(); }
    HttpRequestsListModel* requests() const noexcept { return m_requests; }
    MainTabsListModel* tabs() const noexcept { return m_tabs; }
    RequestExternalViewModel* requestExternal() const noexcept { return m_requestExternal.get(); }
    NotificationListModel* notifications() const noexcept { return m_notifications; }
    OutputFormatsListModel* outputFormats() const noexcept { return m_outputFormats; }
    RequestsCommandPaletteListModel* requestsCommandPaletter() const noexcept { return m_requestsCommandPaletter; }
    bool openedCommandPalette() const noexcept { return m_openedCommandPalette; }
    OpenApiExporterViewModel* openApiExporter() const noexcept { return m_openApiExporter; }
    GlobalVariablesListModel* globalVariables() const noexcept { return m_globalVariables; }
    QList<QVariantMap> shortcuts() const noexcept { return m_shortcuts; }

    bool focusedHelpTextField() const noexcept { return m_focusedHelpTextField; }
    void setFocusedHelpTextField(bool focusedHelpTextField) noexcept;

    bool showGlobalVariablesPopup() const noexcept { return m_showGlobalVariablesPopup; }
    QString selectedGlobalVariable() const noexcept { return m_selectedGlobalVariable; }

    Q_INVOKABLE void addNewRequest(bool forceSelectedAddedItem = false);
    Q_INVOKABLE bool shortcutHandler(const QString& shortcut) noexcept;
    Q_INVOKABLE void refreshFindedIndex() noexcept;
    Q_INVOKABLE void openedFile(const QString& filePath) noexcept;
    Q_INVOKABLE void savedFile(const QString& filePath) noexcept;
    Q_INVOKABLE void generateImage(const QString& filePath) noexcept;
    Q_INVOKABLE void generateImageToClipboard() noexcept;
    Q_INVOKABLE void importFromOpenApi(int index, bool replaceCurrent) noexcept;
    Q_INVOKABLE void saveDownloadedFile(const QString& fileName) noexcept;
    Q_INVOKABLE void closeGlobalVariables() noexcept;

    void deleteCurrentRequest() noexcept;

    bool helpVisible() const noexcept { return m_helpVisible; }
    void setHelpVisible(const bool helpVisible) noexcept;

    bool openApiHelpVisible() const noexcept { return m_openApiHelpVisible; }

private:
    QString removeProtocol(const QString& filePath) noexcept;
    void fillAuthorizationSecurity(const QString& key, HttpRequestViewModel* request, const OpenApiRoutesOptions& options);
    void fillMappings();
    void fillCommands();
    void fillHelpShortcuts();

signals:
    void requestPerformerChanged();
    void textAdviserChanged();
    void requestsChanged();
    void tabsChanged();
    void requestExternalChanged();
    void helpVisibleChanged();
    void notificationsChanged();
    void outputFormatsChanged();
    void requestsCommandPaletterChanged();
    void openedCommandPaletteChanged();
    void changedFindedIndex(int findedLine);
    void needOpenFile();
    void needSaveFile();
    void needGenerateImage();
    void needOpenApiExportWindow();
    void openApiExporterChanged();
    void openApiHelpVisibleChanged();
    void globalVariablesChanged();
    void needGlobalVariablesWindow();
    void shortcutsChanged();
    void focusedHelpTextFieldChanged();
    void showGlobalVariablesPopupChanged();
    void selectedGlobalVariableChanged();
    void globalVariableSelected(int lastPosition);

private slots:
    void errorNotification(const QString& message, const QString& title);
    void requestsPaletterItemSelected(const QUuid& id);

};

#endif // BACKENDVIEWMODEL_H
