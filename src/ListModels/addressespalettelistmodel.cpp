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

#include "addressespalettelistmodel.h"

AddressesPaletteListModel::AddressesPaletteListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void AddressesPaletteListModel::setup(QSharedPointer<QList<OpenApiAddressModel*>> addresses)
{
    m_addresses = addresses;
    refresh(true);
}

int AddressesPaletteListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_addresses->size();
}

QVariant AddressesPaletteListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = index.row();
    auto requestId = m_history.value(itemIndex);
    auto address = m_addressesMap.value(requestId);

    switch (role) {
        case IdentifierRole: {
            return QVariant(itemIndex);
        }
        case TitleRole: {
            return QVariant(address->title());
        }
        case IsSelectedRole: {
            return QVariant(m_selected == itemIndex);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> AddressesPaletteListModel::roleNames() const
{
    return {
        {
            IdentifierRole,
            "identifier"
        },
        {
            TitleRole,
            "title"
        },
        {
            IsSelectedRole,
            "isSelected"
        }
    };
}

void AddressesPaletteListModel::selectItem()
{
    auto id = m_history.value(m_selected);
    m_history.removeOne(id);
    m_history.insert(0, id);
    m_selected = 0;
    emit itemSelected(id);
}

void AddressesPaletteListModel::selectNext()
{
    if (m_addresses->count() < 2) return;

    m_selected += 1;
    if (m_selected >= m_addresses->count()) m_selected = 0;

    refresh();
}

void AddressesPaletteListModel::selectItemByAddressIndex(int index)
{
    auto address = m_addresses->value(index);
    QMapIterator mapIterator(m_addressesMap);
    QUuid key;
    while (mapIterator.hasNext()) {
        mapIterator.next();

        if (mapIterator.value() == address) key = mapIterator.key();
    }
    m_selected = m_history.indexOf(key);

    selectItem();
}

void AddressesPaletteListModel::refresh(bool needRecreateHistory)
{
    beginResetModel();

    if (needRecreateHistory) recreateHistory();

    endResetModel();
}

void AddressesPaletteListModel::recreateHistory()
{
    m_addressesMap.clear();
    foreach (auto item, *m_addresses) {
         auto id = item->addressId();
         m_addressesMap.insert(id, item);
         if (!m_history.contains(id)) m_history.insert(0, id);
    }

    QList<QUuid> m_removedIds;

    foreach (auto historyItem, m_history) {
        if (!m_addressesMap.contains(historyItem)) m_removedIds.append(historyItem);
    }

    foreach (auto id, m_removedIds) {
        m_history.removeOne(id);
    }
}

OpenApiAddressModel *AddressesPaletteListModel::getSelectedAddressById(const QUuid& id)
{
    return m_addressesMap.value(id);
}

int AddressesPaletteListModel::getSelectedAddressIndex()
{
    auto id = m_history.value(m_selected);
    auto address = m_addressesMap.value(id);
    return m_addresses->indexOf(address);
}
