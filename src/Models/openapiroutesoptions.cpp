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

#include "openapiroutesoptions.h"

OpenApiRoutesOptions::OpenApiRoutesOptions()
{

}

void OpenApiRoutesOptions::clearAuthorizationSchemes() noexcept
{
    foreach (auto scheme, m_authorizationSchemes) {
        scheme->setDisplayName("");
        scheme->setName("");
        scheme->setHttpScheme("");
        scheme->setIn("");
        scheme->setOpenIdConnectUrl("");
        scheme->setType("");
        delete scheme;
    }

    m_authorizationSchemes.clear();
}

void OpenApiRoutesOptions::addAuthorizationScheme(const QString &type, const QString &in, const QString &name, const QString &httpScheme, const QString &openIdConnect, const QString& displayName)
{
    auto model = new OpenApiRoutesAuthorizationModel();
    if (!type.isEmpty()) model->setType(type);
    if (!in.isEmpty()) model->setIn(in);
    if (!name.isEmpty()) model->setName(name);
    if (!httpScheme.isEmpty()) model->setHttpScheme(httpScheme);
    if (!openIdConnect.isEmpty()) model->setOpenIdConnectUrl(openIdConnect);
    if (!displayName.isEmpty()) model->setDisplayName(displayName);

    m_authorizationSchemes.append(model);
}

void OpenApiRoutesOptions::clearSecurities() noexcept
{
    foreach (auto innerMap, m_securities) {
        innerMap.clear();
    }

    m_securities.clear();
}

void OpenApiRoutesOptions::addSecurityMap()
{
    QMultiMap<QString, QString> map;
    m_securities.append(map);
}

void OpenApiRoutesOptions::addSecurity(int index, const QString &security, const QString &scope)
{
    m_securities[index].insert(security, scope);
}

QStringList OpenApiRoutesOptions::getFirstSecurity() const noexcept
{
    QStringList result;
    auto first = m_securities.first();
    return first.keys();
}

OpenApiRoutesAuthorizationModel *OpenApiRoutesOptions::getScheme(const QString &key) const noexcept
{
    auto iterator = std::find_if(
        m_authorizationSchemes.cbegin(),
        m_authorizationSchemes.cend(),
        [key](OpenApiRoutesAuthorizationModel* item) {
            return item->displayName() == key;
        }
    );

    if (iterator == m_authorizationSchemes.cend()) return nullptr;

    return *iterator;
}
