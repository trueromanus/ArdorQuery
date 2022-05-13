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

class BackendViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HttpPerformerViewModel* requestPerformer READ requestPerformer NOTIFY requestPerformerChanged)
    Q_PROPERTY(TextAdvisorViewModel* textAdviser READ textAdviser NOTIFY textAdviserChanged)
    Q_PROPERTY(HttpRequestsListModel* requests READ requests NOTIFY requestsChanged)
    Q_PROPERTY(MainTabsListModel* tabs READ tabs NOTIFY tabsChanged)
    Q_PROPERTY(RequestExternalViewModel* requestExternal READ requestExternal NOTIFY requestExternalChanged)

private:
    HttpPerformerViewModel* m_requestPerformer { new HttpPerformerViewModel(this) };
    QSharedPointer<TextAdvisorViewModel> m_textAdviser { new TextAdvisorViewModel() };
    QScopedPointer<RequestExternalViewModel> m_requestExternal { new RequestExternalViewModel() };
    HttpRequestsListModel* m_requests { new HttpRequestsListModel(this) };
    MainTabsListModel* m_tabs { new MainTabsListModel(this) };

public:
    explicit BackendViewModel(QObject *parent = nullptr);

    HttpPerformerViewModel* requestPerformer() const noexcept { return m_requestPerformer; }
    TextAdvisorViewModel* textAdviser() const noexcept { return m_textAdviser.get(); }
    HttpRequestsListModel* requests() const noexcept { return m_requests; }
    MainTabsListModel* tabs() const noexcept { return m_tabs; }
    RequestExternalViewModel* requestExternal() const noexcept { return m_requestExternal.get(); }

    Q_INVOKABLE void addNewRequest(const QString& name);

signals:
    void requestPerformerChanged();
    void textAdviserChanged();
    void requestsChanged();
    void tabsChanged();
    void requestExternalChanged();

};

#endif // BACKENDVIEWMODEL_H
