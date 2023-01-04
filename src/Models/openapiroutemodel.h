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
#include <QMultiMap>
#include "openapiparametermodel.h"

class OpenApiRouteModel
{
private:
    int m_identifier { 0 };
    QString m_path { "" };
    QString m_method { "" };
    QString m_summary { "" };
    QList<OpenApiParameterModel*> m_parameters { QList<OpenApiParameterModel*>() };
    QList<QMultiMap<QString, QString>> m_securities { QList<QMultiMap<QString, QString>>() };

public:
    OpenApiRouteModel();

    QString path() const noexcept { return m_path; }
    QString method() const noexcept { return m_method; }
    QString summary() const noexcept { return m_summary; }
    int identifier() const noexcept { return m_identifier; }

    void setPath(const QString& path) noexcept { m_path = path; }
    void setMethod(const QString& method) noexcept { m_method = method; }
    void setSummary(const QString& summary) noexcept { m_summary = summary; }
    void setIdentifier(int identifier) noexcept { m_identifier = identifier; }

    void addParameter(const OpenApiParameterModel* model) noexcept;

    void clear() noexcept;

    void addSecurityMap() noexcept;
    void addSecurity(int index, const QString& key, const QString scope);
    bool hasSecurity() const noexcept { return !m_securities.isEmpty(); }
    QStringList getFirstKeys() const noexcept;
    QStringList getKeys(QStringList keys) const noexcept;

    const QList<OpenApiParameterModel*>& parameters() const;

};

#endif // OPENAPIROUTEMODEL_H
