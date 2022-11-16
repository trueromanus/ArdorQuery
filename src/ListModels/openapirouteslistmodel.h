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

#ifndef OPENAPIROUTESLISTMODEL_H
#define OPENAPIROUTESLISTMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QList>
#include "../Models/openapiroutemodel.h"

class OpenApiRoutesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QString orderField READ orderField WRITE setOrderField NOTIFY orderFieldChanged)

private:
    QList<OpenApiRouteModel*> m_filteredRoutes { QList<OpenApiRouteModel*>() };
    QList<int> m_filteredIds { QList<int>() };
    QList<OpenApiRouteModel*> m_allRoutes { QList<OpenApiRouteModel*>() };
    enum OpenApiRoutesRoles {
        IdentfierRole = Qt::UserRole + 1,
        MethodRole,
        RouteRole,
        DescriptionRole,
        MethodColorRole,
    };
    QString m_filter { "" };
    QString m_orderField { "" };

public:
    explicit OpenApiRoutesListModel(QObject *parent = nullptr);

    void setupRoutes(const QList<OpenApiRouteModel*>& routes);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString filter() const noexcept { return m_filter; }
    void setFilter(const QString& filter) noexcept;

    QString orderField() const noexcept { return m_orderField; }
    void setOrderField(const QString& orderField) noexcept;

    void refresh();

    OpenApiRouteModel* getRouteByIndex(int index) const noexcept;

private:
    QString getMethodColor(const OpenApiRouteModel* route) const noexcept;

signals:
    void filterChanged();
    void orderFieldChanged();

};

#endif // OPENAPIROUTESLISTMODEL_H
