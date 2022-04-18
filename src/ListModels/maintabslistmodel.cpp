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

#include "maintabslistmodel.h"

MainTabsListModel::MainTabsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_tabs->append("Request");
    m_tabs->append("Result");
}

int MainTabsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_tabs->size();
}

QVariant MainTabsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

        auto tabIndex = index.row();
        auto tab = m_tabs->at(tabIndex);

        switch (role) {
            case TitleRole: {
                return QVariant(tab);
            }
            case IsActiveRole: {
                return QVariant(m_activatedTab == tabIndex);
            }
            case IdentifierRole: {
                return QVariant(index.row());
            }
            case IsRequestsRole: {
                return QVariant(index.row() == 0);
            }
        }

        return QVariant();
}

QHash<int, QByteArray> MainTabsListModel::roleNames() const
{
    return {
        {
            TitleRole,
            "title"
        },
        {
            IsActiveRole,
            "isActived"
        },
        {
            IdentifierRole,
            "identifier"
        },
        {
            IsRequestsRole,
            "isRequests"
        }
    };
}

void MainTabsListModel::addTab(const QString &name) noexcept
{
    m_tabs->append(name);
}

void MainTabsListModel::removeTab(const int index) noexcept
{
    if (index >= m_tabs->length()) return;

    beginResetModel();

    m_tabs->removeAt(index);

    endResetModel();
}

void MainTabsListModel::activateTab(const int newIndex)
{
    if (newIndex >= m_tabs->count()) return;

    auto oldIndex = m_activatedTab;
    m_activatedTab = newIndex;

    emit dataChanged(index(oldIndex), index(oldIndex));
    emit dataChanged(index(m_activatedTab), index(m_activatedTab));
}


