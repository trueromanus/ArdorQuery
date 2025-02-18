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

#include "globalvariablespostscriptmodel.h"

GlobalVariablesPostScriptModel::GlobalVariablesPostScriptModel(QObject *parent, GlobalVariablesListModel* globalVariables,
    QSharedPointer<QList<HttpRequestModel*>> requests)
    : QObject{parent}
{
    m_globalVariables = globalVariables;
    m_requests = requests;
}

bool GlobalVariablesPostScriptModel::has(const QString &name)
{
    return m_globalVariables->hasVariableName(name);
}

QString GlobalVariablesPostScriptModel::get(const QString &name)
{
    if (!m_globalVariables->hasVariableName(name)) return "";

    return m_globalVariables->getVariable(name);
}

QVariantMap GlobalVariablesPostScriptModel::getQueryStatus(const QString &unique)
{
    QVariantMap map;

    auto iterator = std::find_if(
        m_requests->begin(),
        m_requests->end(),
        [unique](HttpRequestModel* item) {
            auto id = item->requestModel()->getIdentifier();
            return id == unique;
        }
    );
    if (iterator == m_requests->end()) return map;

    auto item = *iterator;
    map["hasErrors"] = item->resultModel()->hasError();
    map["errorMessage"] = item->resultModel()->networkError();
    map["status"] = item->resultModel()->statusCode();
    map["responseSize"] = static_cast<unsigned long long>(item->resultModel()->originResponseSize());

    return map;
}

QList<QString> GlobalVariablesPostScriptModel::getQueryHeaders(const QString &unique)
{
    QList<QString> result;

    auto iterator = std::find_if(
        m_requests->begin(),
        m_requests->end(),
        [unique](HttpRequestModel* item) {
            auto id = item->requestModel()->getIdentifier();
            return id == unique;
        }
        );
    if (iterator == m_requests->end()) return result;

    auto item = *iterator;

    result.append(item->resultModel()->rawHeaders());
    return result;
}
