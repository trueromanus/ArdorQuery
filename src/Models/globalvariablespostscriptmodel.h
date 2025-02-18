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

#ifndef GLOBALVARIABLESPOSTSCRIPTMODEL_H
#define GLOBALVARIABLESPOSTSCRIPTMODEL_H

#include <QObject>
#include <QSharedPointer>
#include "../ListModels/globalvariableslistmodel.h"
#include "../Models/httprequestmodel.h"

class GlobalVariablesPostScriptModel : public QObject
{
    Q_OBJECT

private:
    GlobalVariablesListModel* m_globalVariables { nullptr };
    QSharedPointer<QList<HttpRequestModel*>> m_requests { nullptr };

public:
    explicit GlobalVariablesPostScriptModel(QObject *parent, GlobalVariablesListModel* globalVariables, QSharedPointer<QList<HttpRequestModel*>> requests);

    Q_INVOKABLE bool has(const QString& name);
    Q_INVOKABLE QString get(const QString& name);
    Q_INVOKABLE QVariantMap getQueryStatus(const QString& unique);
    Q_INVOKABLE QList<QString> getQueryHeaders(const QString& unique);

signals:
};

#endif // GLOBALVARIABLESPOSTSCRIPTMODEL_H
