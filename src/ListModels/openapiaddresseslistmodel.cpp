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

#include "openapiaddresseslistmodel.h"

OpenApiAddressesListModel::OpenApiAddressesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int OpenApiAddressesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_usedAddresses->size();
}

QVariant OpenApiAddressesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();
    auto item = m_usedAddresses->value(currentIndex);

    switch (role) {
        case IdentfierRole: {
            return QVariant(currentIndex);
        }
        case TitleRole: {
            return QVariant(item->title());
        }
        case IsSelectedRole: {
            return QVariant();
        }
        case RouteRole: {
            return QVariant(item->address());
        }
        case BaseUrlRole: {
            return QVariant(item->baseUrl());
        }
        case FilterRole: {
            return QVariant(item->filter());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> OpenApiAddressesListModel::roleNames() const
{
    return {
        {
            IdentfierRole,
            "identifier"
        },
        {
            TitleRole,
            "title"
        },
        {
            IsSelectedRole,
            "isSelected"
        },
        {
            RouteRole,
            "route"
        },
        {
            BaseUrlRole,
            "baseRoute"
        },
        {
            FilterRole,
            "filter"
        }
    };

}

void OpenApiAddressesListModel::addAddress(const QString& title, const QString& route, const QString& baseUrl, const QString& filter) noexcept
{
    beginResetModel();

    auto model = new OpenApiAddressModel();
    model->setAddress(route);
    model->setTitle(title);
    model->setBaseUrl(baseUrl);
    model->setFilter(filter);
    m_usedAddresses->append(model);

    endResetModel();

    emit addressesChanged();
    emit hasItemsChanged();
}

QSharedPointer<QList<OpenApiAddressModel*>> OpenApiAddressesListModel::getAddresses() noexcept
{
    return m_usedAddresses;
}

void OpenApiAddressesListModel::selectItem(int index) noexcept
{
    if (index >= m_usedAddresses->count()) return;

    beginResetModel();

    m_selectedIndex = index;

    endResetModel();
}

void OpenApiAddressesListModel::deleteItem(int index) noexcept
{
    if (index >= m_usedAddresses->count()) return;

    beginResetModel();

    if (m_selectedIndex == index) m_selectedIndex = 0;

    m_usedAddresses->removeAt(index);

    endResetModel();

    emit hasItemsChanged();
}
