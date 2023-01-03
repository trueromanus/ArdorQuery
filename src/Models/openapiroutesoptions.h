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

#ifndef OPENAPIROUTESOPTIONS_H
#define OPENAPIROUTESOPTIONS_H

#include <QList>
#include <QMultiMap>
#include "openapiroutesauthorizationmodel.h"

class OpenApiRoutesOptions
{
private:
    QList<OpenApiRoutesAuthorizationModel*> m_authorizationSchemes { QList<OpenApiRoutesAuthorizationModel*>() };
    QList<QMultiMap<QString, QString>> m_securities { QList<QMultiMap<QString, QString>>() };

public:
    OpenApiRoutesOptions();

    void clearAuthorizationSchemes() noexcept;
    void addAuthorizationScheme(const QString& type, const QString& in, const QString& name, const QString& httpScheme, const QString& openIdConnect, const QString& displayName);

    void clearSecurities() noexcept;
    void addSecurityMap();
    void addSecurity(int index, const QString& security, const QString& scope);

    bool onlyOneSecurity() const noexcept { return m_securities.count() == 1; }
    QStringList getFirstSecurity() const noexcept;

    OpenApiRoutesAuthorizationModel* getScheme(const QString& key) const noexcept;
};

#endif // OPENAPIROUTESOPTIONS_H
