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

#ifndef REQUESTEXTERNALVIEWMODEL_H
#define REQUESTEXTERNALVIEWMODEL_H

#include <QObject>
#include "../globalconstants.h"
#include "../ViewModels/httprequestviewmodel.h"

class RequestExternalViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HttpRequestViewModel* httpRequest READ httpRequest WRITE setHttpRequest NOTIFY httpRequestChanged)
    Q_PROPERTY(TextAdvisorViewModel* textAdvisor READ textAdvisor WRITE setTextAdvisor NOTIFY textAdvisorChanged)

private:
    HttpRequestViewModel* m_httpRequest { nullptr };
    TextAdvisorViewModel* m_textAdvisor { nullptr };

public:
    explicit RequestExternalViewModel(QObject *parent = nullptr);

    void parseFromString(const QString& input) noexcept;

    HttpRequestViewModel* httpRequest() const noexcept { return m_httpRequest; }
    void setHttpRequest(const HttpRequestViewModel* httpRequest) noexcept;

    TextAdvisorViewModel* textAdvisor() const noexcept { return m_textAdvisor; }
    void setTextAdvisor(const TextAdvisorViewModel* textAdvisor) noexcept;

    Q_INVOKABLE void appendFromClipboard();
    Q_INVOKABLE void replaceFromClipboard();

    void copyToClipboard();

private:
    QString getTextFromClipboard() const noexcept;

signals:
    void httpRequestChanged();
    void textAdvisorChanged();

};

#endif // REQUESTEXTERNALVIEWMODEL_H
