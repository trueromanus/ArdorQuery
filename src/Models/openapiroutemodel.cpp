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

void OpenApiRouteModel::clearParameters() noexcept
{
    foreach (auto parameter, m_parameters) {
        delete parameter;
    }

    m_parameters.clear();
}

const QList<OpenApiParameterModel *> &OpenApiRouteModel::parameters() const
{
    return m_parameters;
}
