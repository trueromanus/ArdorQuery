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

#ifndef OPENAPIROUTESAUTHORIZATIONMODEL_H
#define OPENAPIROUTESAUTHORIZATIONMODEL_H

#include <QString>

class OpenApiRoutesAuthorizationModel
{
private:
    QString m_type { "" }; // "apiKey", "http", "mutualTLS", "oauth2", "openIdConnect"
    QString m_in { "" }; // "query", "header" or "cookie"
    QString m_name { "" }; // name of query, header or cookie
    QString m_httpScheme { "" }; // used in http authorization header like 'Authorization: <auth-scheme> <authorization-parameters>'
    // QString m_flows { "" }; // TODO: support oAuth flows
    QString m_openIdConnectUrl { "" }; //
    QString m_displayName { "" };

public:
    OpenApiRoutesAuthorizationModel();

    QString type() const noexcept { return m_type; }
    void setType(const QString& type) noexcept { m_type = type; }

    QString in() const noexcept { return m_in; }
    void setIn(const QString& in) noexcept { m_in = in; }

    QString name() const noexcept { return m_name; }
    void setName(const QString& name) noexcept { m_name = name; }

    QString httpScheme() const noexcept { return m_httpScheme; }
    void setHttpScheme(const QString& httpScheme) noexcept { m_httpScheme = httpScheme; }

    QString openIdConnectUrl() const noexcept { return m_openIdConnectUrl; }
    void setOpenIdConnectUrl(const QString& openIdConnectUrl) noexcept { m_openIdConnectUrl = openIdConnectUrl; }

    QString displayName() const noexcept { return m_displayName; }
    void setDisplayName (const QString& displayName) noexcept { m_displayName = displayName; }

};

#endif // OPENAPIROUTESAUTHORIZATIONMODEL_H
