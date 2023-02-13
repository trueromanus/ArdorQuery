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

#include "httprequestslistmodel.h"

HttpRequestsListModel::HttpRequestsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

int HttpRequestsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_requests->size();
}

QVariant HttpRequestsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto request = m_requests->at(index.row());

    switch (role) {
        case RequestTitleRole: {
            return QVariant(request->requestModel()->title());
        }
        case ViewModelRole: {
            return QVariant::fromValue(request->requestModel());
        }
        case ResultViewModelRole: {
            return QVariant::fromValue(request->resultModel());
        }
        case IsSelectedRole: {
            return QVariant(index.row() == m_selectedIndex);
        }
        case IndexRole: {
            return QVariant(index.row());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> HttpRequestsListModel::roleNames() const
{
    return {
        {
            RequestTitleRole,
            "title"
        },
        {
            ViewModelRole,
            "viewModel"
        },
        {
            ResultViewModelRole,
            "resultViewModel"
        },
        {
            IsSelectedRole,
            "isSelected"
        },
        {
            IndexRole,
            "currentIndex"
        }
    };
}

int HttpRequestsListModel::addItem(const HttpRequestModel* model) noexcept
{
    beginResetModel();

    m_requests->append(const_cast<HttpRequestModel*>(model));

    endResetModel();

    return m_requests->count() - 1;
}

bool HttpRequestsListModel::singleRequest() const noexcept
{
    return m_requests->count() == 1;
}

void HttpRequestsListModel::deleteSelectedItem() noexcept
{
    auto selectedIndex = m_selectedIndex;
    m_selectedIndex -= 1;
    if (m_selectedIndex < 0) m_selectedIndex = 0;

    beginResetModel();

    auto request = m_requests->at(selectedIndex);

    request->clear();

    m_requests->removeAt(selectedIndex);

    endResetModel();

    selectItem(m_selectedIndex);
}

QSharedPointer<QList<HttpRequestModel *> > HttpRequestsListModel::getList() const noexcept
{
    return m_requests;
}

HttpRequestModel *HttpRequestsListModel::getSelectedRequest() const noexcept
{
    return m_requests->value(m_selectedIndex);
}

void HttpRequestsListModel::selectItem(const int newIndex) noexcept
{
    if (newIndex < 0) return;
    if (newIndex > m_requests->count() - 1) return;

    auto oldSelectedIndex = m_selectedIndex;
    m_selectedIndex = newIndex;
    emit dataChanged(index(m_selectedIndex, 0), index(m_selectedIndex, 0));
    emit dataChanged(index(oldSelectedIndex, 0), index(oldSelectedIndex, 0));
    emit selectedItemChanged();
}

void HttpRequestsListModel::selectItemById(const QUuid &id) noexcept
{
    auto iterator = std::find_if(
        m_requests->cbegin(),
        m_requests->cend(),
        [id](HttpRequestModel* item) {
           return item->requestId() == id;
        }
    );
    if (iterator == m_requests->cend()) return;

    auto item = *iterator;
    selectItem(m_requests->indexOf(item));
}
