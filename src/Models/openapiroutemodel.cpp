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

#include <QJsonArray>
#include "openapiroutemodel.h"

OpenApiRouteModel::OpenApiRouteModel()
{

}

void OpenApiRouteModel::addParameter(const OpenApiParameterModel *model) noexcept
{
    m_parameters.append(const_cast<OpenApiParameterModel*>(model));
}

void OpenApiRouteModel::addBodyType(const QString &type, const QString &body) noexcept
{
    m_bodyTypes.insert(type, body);
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
    m_bodyTypes.clear();

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

QJsonObject OpenApiRouteModel::toJsonObject() const noexcept
{
    QJsonObject object;

    object["path"] = m_path;
    object["method"] = m_method;
    object["summary"] = m_summary;
    object["identifier"] = m_identifier;

    if (!m_parameters.isEmpty()) {
        QJsonArray parameters;
        foreach (auto parameter, m_parameters) {
            QJsonObject parameterObject;
            parameterObject["name"] = parameter->name();
            parameterObject["required"] = parameter->required();
            parameterObject["in"] = parameter->in();

            parameters.append(parameterObject);
        }
        object[m_parametersField] = parameters;
    }

    if (!m_securities.isEmpty()) {
        QJsonObject securityObject;
        foreach (auto security, m_securities) {
            foreach (auto key, security.keys()) {
                QJsonArray permissionArray;
                auto permissions = security.values(key);
                foreach(auto permission, permissions) {
                    permissionArray.append(permission);
                }
                securityObject[key] = permissionArray;
            }
        }
        object[m_securitiesField] = securityObject;
    }

    if (!m_bodyTypes.isEmpty()) {
        QJsonObject bodiesObject;
        foreach (auto key, m_bodyTypes.keys()) {
            bodiesObject[key] = m_bodyTypes.value(key);
        }
        object[m_bodiesField] = bodiesObject;
    }

    return object;
}

void OpenApiRouteModel::fromJsonObject(const QJsonObject &object) noexcept
{
    m_path = object.value("path").toString();
    m_method = object.value("method").toString();
    m_summary = object.value("summary").toString();
    m_identifier = object.value("identifier").toInt();

    if (object.contains(m_parametersField)) {
        auto parametersArray = object[m_parametersField].toArray();
        foreach (auto parameter, parametersArray) {
            auto parameterObject = parameter.toObject();

            auto model = new OpenApiParameterModel();
            model->setName(parameterObject.value("name").toString());
            model->setRequired(parameterObject.value("required").toBool());
            model->setIn(parameterObject.value("int").toString());
            m_parameters.append(model);
        }
    }

    if (object.contains(m_bodiesField)) {
        auto bodiesObject = object[m_bodiesField].toObject();
        foreach(auto key, bodiesObject.keys()) {
            m_bodyTypes.insert(key, bodiesObject.value(key).toString());
        }
    }

    if (object.contains(m_securitiesField)) {
        auto securitiesObject = object[m_securitiesField].toObject();

        foreach (auto key, securitiesObject.keys()) {
            auto multiMap = QMultiMap<QString, QString>();
            auto permissions = securitiesObject.value(key).toArray();
            foreach(auto permission, permissions) {
                multiMap.insert(key, permission.toString());
            }
            if (permissions.isEmpty()) multiMap.insert(key, "");
            m_securities.append(multiMap);
        }
    }
}

const QList<OpenApiParameterModel *> &OpenApiRouteModel::parameters() const
{
    return m_parameters;
}
