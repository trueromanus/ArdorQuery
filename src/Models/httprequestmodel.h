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
#include "../ViewModels/httprequestviewmodel.h"
#include "../ViewModels/httprequestresultviewmodel.h"

class HttpRequestModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(HttpRequestViewModel* requestModel READ requestModel NOTIFY requestModelChanged)
    Q_PROPERTY(HttpRequestResultViewModel* resultModel READ resultModel NOTIFY resultModelChanged)

private:
    QString m_title { "" };
    QScopedPointer<HttpRequestViewModel> m_requestModel { new HttpRequestViewModel(this) };
    QScopedPointer<HttpRequestResultViewModel> m_resultModel { new HttpRequestResultViewModel(this) };

public:
    explicit HttpRequestModel(QObject *parent = nullptr);

    QString title() const noexcept { return m_title; }
    void setTitle(const QString& title) noexcept;

    HttpRequestViewModel* requestModel() const noexcept { return m_requestModel.get(); }

    HttpRequestResultViewModel* resultModel() const noexcept { return m_resultModel.get(); }

signals:
    void titleChanged();
    void requestModelChanged();
    void resultModelChanged();

};

#endif // HTTPREQUESTMODEL_H
