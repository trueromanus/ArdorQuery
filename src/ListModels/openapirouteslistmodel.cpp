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

#include "openapirouteslistmodel.h"

OpenApiRoutesListModel::OpenApiRoutesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void OpenApiRoutesListModel::setupRoutes(const QList<OpenApiRouteModel *> &routes)
{
    m_allRoutes.append(routes);

    refresh();
}

int OpenApiRoutesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filteredRoutes.size();
}

QVariant OpenApiRoutesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();
    auto item = m_filteredRoutes.value(currentIndex);

    switch (role) {
        case IdentfierRole: {
            return QVariant(currentIndex);
        }
        case MethodRole: {
            return QVariant(item->method().toUpper());
        }
        case RouteRole: {
            return QVariant(item->path());
        }
        case DescriptionRole: {
            return QVariant(item->summary());
        }
        case MethodColorRole: {
            return QVariant(getMethodColor(item));
        }
    }

    return QVariant();
}

QHash<int, QByteArray> OpenApiRoutesListModel::roleNames() const
{
    return {
        {
            IdentfierRole,
            "identifier"
        },
        {
            MethodRole,
            "method"
        },
        {
            RouteRole,
            "route"
        },
        {
            DescriptionRole,
            "description"
        },
        {
            MethodColorRole,
            "methodColor"
        }
    };
}

void OpenApiRoutesListModel::setFilter(const QString &filter) noexcept
{
    if (m_filter == filter) return;

    m_filter = filter;
    emit filterChanged();
}

void OpenApiRoutesListModel::setOrderField(const QString &orderField) noexcept
{
    if (m_orderField == orderField) return;

    m_orderField = orderField;
    emit orderFieldChanged();
}

void OpenApiRoutesListModel::refresh()
{
    beginResetModel();

    m_filteredRoutes.clear();

    if (m_filter.isEmpty() && m_orderField.isEmpty()) {
        m_filteredRoutes.append(m_allRoutes);
        endResetModel();
        return;
    }

    if (!m_filter.isEmpty()) {
        foreach (auto route, m_allRoutes) {
            auto isRoute = route->path().toLower().contains(m_filter.toLower());
            auto isMethod = route->method().toLower().contains(m_filter.toLower());
            auto isDescription = route->summary().toLower().contains(m_filter.toLower());

            if (isRoute || isMethod || isDescription) m_filteredRoutes.append(route);
        }
    }

    endResetModel();

}

QString OpenApiRoutesListModel::getMethodColor(const OpenApiRouteModel *route) const noexcept
{
    auto methodLowered = route->method().toLower();
    if (methodLowered == "get") return "#61AFFE";
    if (methodLowered == "post") return "#49CC90";
    if (methodLowered == "put") return "#FCA130";
    if (methodLowered == "delete") return "#F93E3E";
    if (methodLowered == "patch") return "#50E3C2";

    return "black";
}
