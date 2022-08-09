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

    Q_INVOKABLE void addNewRequest(const QString& name);
    Q_INVOKABLE bool keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept;
    Q_INVOKABLE void keysReleased(int key) noexcept;
    Q_INVOKABLE void refreshFindedIndex() noexcept;

    bool helpVisible() const noexcept { return m_helpVisible; }
    void setHelpVisible(const bool helpVisible) noexcept;

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

private slots:
    void errorNotification(const QString& message, const QString& title);
    void requestsPaletterItemSelected(const QUuid& id);

};

#endif // BACKENDVIEWMODEL_H
