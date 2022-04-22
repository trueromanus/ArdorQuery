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

#ifndef HTTPREQUESTRESULTVIEWMODEL_H
#define HTTPREQUESTRESULTVIEWMODEL_H

#include <QObject>
#include <QDateTime>
#include <QTime>

class HttpRequestResultViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int statusCode READ statusCode WRITE setStatusCode NOTIFY statusCodeChanged)
    Q_PROPERTY(QString displayStatusCode READ displayStatusCode NOTIFY displayStatusCodeChanged)
    Q_PROPERTY(QStringList headers READ headers WRITE setHeaders NOTIFY headersChanged)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(QString responseSize READ responseSize NOTIFY responseSizeChanged)
    Q_PROPERTY(QString responseTime READ responseTime NOTIFY responseTimeChanged)
    Q_PROPERTY(QString networkError READ networkError WRITE setNetworkError NOTIFY networkErrorChanged)

private:
    int m_statusCode { 0 };
    QScopedPointer<QStringList> m_headers { new QStringList() };
    QString m_body { "" };
    QDateTime m_startResultTime;
    QDateTime m_endResultTime;
    uint64_t m_responseSize { 0 };
    QString m_networkError;

public:
    explicit HttpRequestResultViewModel(QObject *parent = nullptr);

    int statusCode() const noexcept { return m_statusCode; }
    void setStatusCode(const int statusCode) noexcept;

    QStringList* headers() const noexcept { return m_headers.get(); }
    void setHeaders(const QStringList& headers) noexcept;

    QString body() const noexcept { return m_body; }
    void setBody(const QString& body) noexcept;

    QString responseSize() const noexcept { return m_responseSize > 0 ? QString::number(m_responseSize) : " - "; }

    QString responseTime() const noexcept;

    QString displayStatusCode() const noexcept { return m_statusCode > 0 ? QString::number(m_statusCode) : " - "; }

    QString networkError() const noexcept { return m_networkError; }
    void setNetworkError(const QString& networkError) noexcept;

    void reset() noexcept;
    void trackRequestTime() noexcept;
    void untrackRequestTime() noexcept;

signals:
    void statusCodeChanged();
    void headersChanged();
    void bodyChanged();
    void responseSizeChanged();
    void responseTimeChanged();
    void networkErrorChanged();
    void displayStatusCodeChanged();

};

#endif // HTTPREQUESTRESULTVIEWMODEL_H
