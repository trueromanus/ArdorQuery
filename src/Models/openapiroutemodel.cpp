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

#include "openapiroutemodel.h"

OpenApiRouteModel::OpenApiRouteModel()
{

}

void OpenApiRouteModel::addParameter(const OpenApiParameterModel *model) noexcept
{
    m_parameters.append(const_cast<OpenApiParameterModel*>(model));
}

void OpenApiRouteModel::clear() noexcept
{
    setPath("");
    setSummary("");
    setMethod("");

    foreach (auto parameter, m_parameters) {
        parameter->setName("");
        parameter->setIn("");
        delete parameter;
    }
    m_parameters.clear();

    foreach (auto map, m_securities) {
        map.clear();
    }
    m_securities.clear();
}

void OpenApiRouteModel::addSecurityMap() noexcept
{
    QMultiMap<QString, QString> map;
    m_securities.append(map);
}

void OpenApiRouteModel::addSecurity(int index, const QString &key, const QString scope)
{
    m_securities[index].insert(key, scope);
}

QStringList OpenApiRouteModel::getFirstKeys() const noexcept
{
    QStringList result;
    if (m_securities.isEmpty()) return result;

    return m_securities.first().keys();
}

QStringList OpenApiRouteModel::getKeys(QStringList keys) const noexcept
{
    QStringList result;
    if (m_securities.isEmpty()) return result;

    foreach (auto security, m_securities) {
        foreach (auto securityKey, security.keys()) {
            if (keys.contains(securityKey)) result.append(securityKey);
        }
    }
    return result;
}

const QList<OpenApiParameterModel *> &OpenApiRouteModel::parameters() const
{
    return m_parameters;
}
