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

#ifndef OPENAPIPARAMETERMODEL_H
#define OPENAPIPARAMETERMODEL_H

#include <QString>

class OpenApiParameterModel
{
private:
    QString m_name { "" };
    bool m_required { false };
    QString m_in { "" };

public:
    OpenApiParameterModel();

    QString name() const noexcept { return m_name; }
    bool required() const noexcept { return m_required; }
    QString in() const noexcept { return m_in; }
    bool isQuery() const noexcept { return m_in == "query"; }
    bool isPath() const noexcept { return m_in == "path"; }
    bool isHeader() const noexcept { return m_in == "header"; }
    bool isCookie() const noexcept { return m_in == "cookie"; }

    void setName(QString name) noexcept { m_name = name; }
    void setRequired(bool required) noexcept { m_required = required; }
    void setIn(const QString& in) noexcept { m_in = in; }
};

#endif // OPENAPIPARAMETERMODEL_H
