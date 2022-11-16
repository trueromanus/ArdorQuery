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

#ifndef OPENAPIROUTEMODEL_H
#define OPENAPIROUTEMODEL_H

#include <QString>
#include <QList>
#include "openapiparametermodel.h"

class OpenApiRouteModel
{
private:
    QString m_path { "" };
    QString m_method { "" };
    QString m_summary { "" };
    QList<OpenApiParameterModel*> m_parameters { QList<OpenApiParameterModel*>() };

public:
    OpenApiRouteModel();

    QString path() const noexcept { return m_path; }
    QString method() const noexcept { return m_method; }
    QString summary() const noexcept { return m_summary; }

    void setPath(const QString& path) noexcept { m_path = path; }
    void setMethod(const QString& method) noexcept { m_method = method; }
    void setSummary(const QString& summary) noexcept { m_summary = summary; }

    void addParameter(const OpenApiParameterModel* model) noexcept;
    void clearParameters() noexcept;

    const QList<OpenApiParameterModel*>& parameters() const;

};

#endif // OPENAPIROUTEMODEL_H
