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

#ifndef HTTPPERFORMERVIEWMODEL_H
#define HTTPPERFORMERVIEWMODEL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMap>
#include "httprequestviewmodel.h"
#include "httprequestresultviewmodel.h"

class HttpPerformerViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HttpRequestViewModel* httpRequest READ httpRequest WRITE setHttpRequest NOTIFY httpRequestChanged)
    Q_PROPERTY(HttpRequestResultViewModel* httpRequestResult READ httpRequestResult WRITE setHttpRequestResult NOTIFY httpRequestResultChanged)

private:
    HttpRequestViewModel* m_httpRequest;
    HttpRequestResultViewModel* m_httpRequestResult;
    QScopedPointer<QNetworkAccessManager> m_networkManager { new QNetworkAccessManager() };
    QScopedPointer<QMap<QString, QString>> m_rawHeaders { new QMap<QString, QString>() };
    QScopedPointer<QMap<QString, HttpRequestResultViewModel*>> m_runningRequests { new QMap<QString, HttpRequestResultViewModel*>() };

public:
    explicit HttpPerformerViewModel(QObject *parent = nullptr);

    HttpRequestViewModel* httpRequest() const noexcept { return m_httpRequest; }
    void setHttpRequest(const HttpRequestViewModel* viewModel) noexcept;

    HttpRequestResultViewModel* httpRequestResult() const noexcept { return m_httpRequestResult; }
    void setHttpRequestResult(const HttpRequestResultViewModel* httpRequestResult) noexcept;

    Q_INVOKABLE void performRequest();

private:
    QByteArray setupSimpleForm(QStringList&& parameters);
    void adjustHeaders(QNetworkRequest& request) noexcept;
    void fillHeader(QNetworkRequest& request, const QString& name, const QString& value) noexcept;
    void startTrackRequest(QNetworkReply* reply) noexcept;

private slots:
    void requestFinished(QNetworkReply *reply);

signals:
    void httpRequestChanged();
    void httpRequestResultChanged();

};

#endif // HTTPPERFORMERVIEWMODEL_H
