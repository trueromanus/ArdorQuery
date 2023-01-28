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
#include <QList>
#include <QHttpMultiPart>
#include "httprequestviewmodel.h"
#include "httprequestresultviewmodel.h"
#include "../Models/httprequestmodel.h"

class HttpPerformerViewModel : public QObject
{
    Q_OBJECT
private:
    QScopedPointer<QNetworkAccessManager> m_networkManager { new QNetworkAccessManager() };
    QScopedPointer<QMap<QString, QString>> m_rawHeaders { new QMap<QString, QString>() };
    QScopedPointer<QMap<QString, HttpRequestResultViewModel*>> m_runningRequests { new QMap<QString, HttpRequestResultViewModel*>() };
    QSharedPointer<QList<HttpRequestModel*>> m_requests { nullptr };

public:
    explicit HttpPerformerViewModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<HttpRequestModel*>> requests);

    void cancelRequest();

    void performOneRequest(HttpRequestModel * request);

    void performAllRequest();

private:
    QByteArray setupSimpleForm(QStringList&& parameters);
    QHttpMultiPart* setupMultiPartForm(QStringList&& files, QStringList&& parameters);
    bool adjustHeaders(QNetworkRequest& request, const HttpRequestViewModel* model) noexcept;
    void fillHeader(QNetworkRequest& request, const QString& name, const QString& value) noexcept;
    void startTrackRequest(QNetworkReply* reply, const QUuid& id, HttpRequestResultViewModel* resultModel) noexcept;
    void adjustOptions(QStringList options, QNetworkRequest& request);
    void performSingleRequest(HttpRequestModel* modelRequest);

private slots:
    void requestFinished(QNetworkReply *reply);

signals:
    void httpRequestChanged();
    void httpRequestResultChanged();
    void pushErrorMessage(const QString &title, const QString &message);

};

#endif // HTTPPERFORMERVIEWMODEL_H
