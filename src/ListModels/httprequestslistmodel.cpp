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
            return QVariant(request->title());
        }
        case ViewModelRole: {
            return QVariant::fromValue(request->requestModel());
        }
        case ResultViewModelRole: {
            return QVariant::fromValue(request->resultModel());
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
        }
    };
}

void HttpRequestsListModel::addItem(const HttpRequestModel* model) noexcept
{
    m_requests->append(const_cast<HttpRequestModel*>(model));
}

void HttpRequestsListModel::selectItem(const int index) noexcept
{
    if (index < 0) return;
    if (index > m_requests->count() - 1) return;

    m_selectedIndex = index;
    emit selectedItemChanged();
}

void HttpRequestsListModel::changeNameForSelectedItem(const QString &newName) noexcept
{
    selectedItem()->setTitle(newName);
    emit dataChanged(index(m_selectedIndex,0), index(m_selectedIndex,0));
}
