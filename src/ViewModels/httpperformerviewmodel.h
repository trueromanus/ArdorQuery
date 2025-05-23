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
#include <QScopedPointer>
#include <QSharedPointer>
#include "httprequestviewmodel.h"
#include "httprequestresultviewmodel.h"
#include "../Models/httprequestmodel.h"
#include "../ListModels/globalvariableslistmodel.h"
#include "../Models/globalvariablespostscriptmodel.h"
#include "../Models/postscriptsessionmodel.h"

class HttpPerformerViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int countRequests READ countRequests NOTIFY countRequestsChanged)
    Q_PROPERTY(int countFinishedRequests READ countFinishedRequests NOTIFY countFinishedRequestsChanged)
    Q_PROPERTY(int countErrorRequests READ countErrorRequests NOTIFY countErrorRequestsChanged)
    Q_PROPERTY(GlobalVariablesListModel* globalVariable READ globalVariable WRITE setGlobalVariable NOTIFY globalVariableChanged)

private:
    QScopedPointer<QNetworkAccessManager> m_networkManager { new QNetworkAccessManager() };
    QScopedPointer<QMap<QString, QString>> m_rawHeaders { new QMap<QString, QString>() };
    QMap<QString, HttpRequestResultViewModel*> m_runningRequests { QMap<QString, HttpRequestResultViewModel*>() };
    QSharedPointer<QList<HttpRequestModel*>> m_requests { nullptr };
    QMultiMap<int, QUuid> m_orderedRequests { QMultiMap<int, QUuid>() };
    int m_orderedRequestsIndex = 0;
    int m_countRequests { 0 };
    int m_countFinishedRequests { 0 };
    int m_countErrorRequests { 0 };
    GlobalVariablesListModel* m_globalVariable { nullptr };
    GlobalVariablesPostScriptModel* m_globalVariablePostScript { nullptr };
    PostScriptSessionModel* m_sessionObject { nullptr };
    QMap<QString, QString> m_emptyRemapDictionary { QMap<QString, QString>() };

public:
    explicit HttpPerformerViewModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<HttpRequestModel*>> requests);

    void cancelRequest();

    void performOneRequest(HttpRequestModel * request);

    void performAllRequest();

    void performRequests(const QList<QUuid>& ids);

    int countRequests() const noexcept { return m_countRequests; }

    int countFinishedRequests() const noexcept { return m_countFinishedRequests; }

    int countErrorRequests() const noexcept { return m_countErrorRequests; }

    GlobalVariablesListModel* globalVariable() const noexcept { return m_globalVariable; }
    void setGlobalVariable(const GlobalVariablesListModel* globalVariable) noexcept;

private:
    QByteArray setupSimpleForm(QStringList&& parameters);
    QHttpMultiPart* setupMultiPartForm(QStringList&& files, QStringList&& parameters);
    bool adjustHeaders(QNetworkRequest& request, const HttpRequestViewModel* model) noexcept;
    void fillHeader(QNetworkRequest& request, const QString& name, const QString& value) noexcept;
    void startTrackRequest(QNetworkReply* reply, const QUuid& id, HttpRequestResultViewModel* resultModel) noexcept;
    void adjustOptions(QStringList options, QNetworkRequest& request);
    bool performSingleRequest(HttpRequestModel* modelRequest);
    void addToCounter(int number) noexcept;
    void reduceFromCounter() noexcept;
    void increaseErrorCounter() noexcept;
    void runPostScript(const QString& script, QObject* properties, HttpRequestResultViewModel* result) noexcept;
    void runMaintainQueries();
    QString applyGlobalVariables(const QString& value);

private slots:
    void requestFinished(QNetworkReply *reply);

signals:
    void httpRequestChanged();
    void httpRequestResultChanged();
    void pushErrorMessage(const QString &title, const QString &message);
    void countRequestsChanged();
    void countFinishedRequestsChanged();
    void globalVariableChanged();
    void countErrorRequestsChanged();

};

#endif // HTTPPERFORMERVIEWMODEL_H
