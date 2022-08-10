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

#ifndef HTTPREQUESTMODEL_H
#define HTTPREQUESTMODEL_H

#include <QObject>
#include <QUuid>
#include "../ViewModels/httprequestviewmodel.h"
#include "../ViewModels/httprequestresultviewmodel.h"

class HttpRequestModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HttpRequestViewModel* requestModel READ requestModel NOTIFY requestModelChanged)
    Q_PROPERTY(HttpRequestResultViewModel* resultModel READ resultModel NOTIFY resultModelChanged)

private:
    QScopedPointer<HttpRequestViewModel> m_requestModel { new HttpRequestViewModel() };
    QScopedPointer<HttpRequestResultViewModel> m_resultModel { new HttpRequestResultViewModel() };
    QUuid m_requestId { QUuid::createUuid() };

public:
    explicit HttpRequestModel(QObject *parent = nullptr);

    HttpRequestViewModel* requestModel() const noexcept { return m_requestModel.get(); }

    HttpRequestResultViewModel* resultModel() const noexcept { return m_resultModel.get(); }

    QUuid requestId() const noexcept { return m_requestId; }

signals:
    void titleChanged();
    void requestModelChanged();
    void resultModelChanged();

};

#endif // HTTPREQUESTMODEL_H
