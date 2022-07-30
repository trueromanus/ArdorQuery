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

#include "requestscommandpalettelistmodel.h"

RequestsCommandPaletteListModel::RequestsCommandPaletteListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void RequestsCommandPaletteListModel::setup(QSharedPointer<QList<HttpRequestModel *> > requests)
{
    m_requests = requests;
    refresh(true);
}

int RequestsCommandPaletteListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_requests->size();
}

QVariant RequestsCommandPaletteListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = index.row();
    auto requestId = m_history.value(itemIndex);
    auto request = m_requestsMap.value(requestId);

    switch (role) {
        case IdentifierRole: {
            return QVariant(itemIndex);
        }
        case TitleRole: {
            return QVariant(request->title());
        }
        case IsSelectedRole: {
            return QVariant(m_selected == itemIndex);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> RequestsCommandPaletteListModel::roleNames() const
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

void RequestsCommandPaletteListModel::selectItem()
{
    auto id = m_history.value(m_selected);
    m_history.removeOne(id);
    m_history.insert(0, id);
    m_selected = 0;
    emit itemSelected(id);
}

void RequestsCommandPaletteListModel::selectNext()
{
    if (m_requests->count() < 2) return;

    m_selected += 1;
    if (m_selected >= m_requests->count()) m_selected = 0;

    refresh();
}

void RequestsCommandPaletteListModel::refresh(bool needRecreateHistory)
{
    beginResetModel();

    if (needRecreateHistory) recreateHistory();

    endResetModel();
}

void RequestsCommandPaletteListModel::recreateHistory()
{
    m_requestsMap.clear();
    foreach (auto item, *m_requests) {
         auto id = item->requestId();
         m_requestsMap.insert(id, item);
         if (!m_history.contains(id)) m_history.append(id);
    }

    QList<QUuid> m_removedIds;

    foreach (auto historyItem, m_history) {
        if (!m_requestsMap.contains(historyItem)) m_removedIds.append(historyItem);
    }

    foreach (auto id, m_removedIds) {
        m_history.removeOne(id);
    }
}
