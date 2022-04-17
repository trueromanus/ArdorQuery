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

#include "httprequestviewmodel.h"

HttpRequestViewModel::HttpRequestViewModel(QObject *parent)
    : QAbstractListModel{parent}
{
    auto zeroItem = new HttpRequestItem();
    m_items->append(zeroItem);
}

int HttpRequestViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items->count();
}

QVariant HttpRequestViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

        auto rowIndex = index.row();
        auto item = m_items->at(rowIndex);

        switch (role) {
            case TypeRole: {
                return QVariant(item->type());
            }
            case TextRole: {
                return QVariant(item->text());
            }
            case IsActiveRole: {
                return QVariant(m_selectedItem == rowIndex);
            }
            case IndexRole: {
                return QVariant(rowIndex);
            }
            case TypeColor: {
                return QVariant(getTypeColor(item->type()));
            }
        }

        return QVariant();
}

QHash<int, QByteArray> HttpRequestViewModel::roleNames() const
{

    return {
        {
            TypeRole,
            "lineType"
        },
        {
            TextRole,
            "textContent"
        },
        {
            IsActiveRole,
            "isActive"
        },
        {
            IndexRole,
            "currentIndex"
        },
        {
            TypeColor,
            "typeColor"
        }
    };
}

void HttpRequestViewModel::setSelectedItem(const int selectedItem) noexcept
{
    if (m_selectedItem == selectedItem) return;

    auto oldIndex = m_selectedItem;

    m_selectedItem = selectedItem;
    emit selectedItemChanged();
    emit dataChanged(index(selectedItem, 0), index(selectedItem, 0));
    emit dataChanged(index(oldIndex, 0), index(oldIndex, 0));
}

void HttpRequestViewModel::setTextAdvisor(const QSharedPointer<TextAdvisorViewModel> textAdviser) noexcept
{
    m_textAdvisor = textAdviser;
}

void HttpRequestViewModel::addItem(const int position)
{
    int actualPosition = position;

    beginResetModel();

    auto item = new HttpRequestItem();

    if (actualPosition == -1) {
        m_items->append(item);
        actualPosition = m_items->count() - 1;
    } else {
        if (actualPosition >= m_items->count()) actualPosition = m_items->count();
        m_items->insert(actualPosition, item);
    }

    endResetModel();

    setSelectedItem(actualPosition);
}

void HttpRequestViewModel::refreshItem(const int position, const QString &content)
{
    auto item = m_items->value(position);
    item->setText(content);

    emit dataChanged(index(position, 0), index(position, 0));
}

void HttpRequestViewModel::setItemContent(const int position, const QString &content)
{
    auto item = m_items->value(position);
    item->setText(content);

    auto lowerContent = content.toLower();
    auto itemType = static_cast<HttpRequestTypes>(item->type());
    bool needRefresh = false;

    if (lowerContent.startsWith("url ") && itemType != HttpRequestTypes::UrlType) {
        item->setType(static_cast<int>(HttpRequestTypes::UrlType));
        needRefresh = true;
    }
    if (lowerContent.startsWith("method ") && itemType != HttpRequestTypes::MethodType) {
        item->setType(static_cast<int>(HttpRequestTypes::MethodType));
        needRefresh = true;
    }

    if (lowerContent.startsWith("body ") && itemType != HttpRequestTypes::BodyType) {
        item->setType(static_cast<int>(HttpRequestTypes::BodyType));
        needRefresh = true;
    }

    if (lowerContent.startsWith("form ") && itemType != HttpRequestTypes::FormItemType) {
        item->setType(static_cast<int>(HttpRequestTypes::FormItemType));
        needRefresh = true;
    }

    if (itemType != HttpRequestTypes::UnknownType && content.isEmpty()) {
        item->setType(static_cast<int>(HttpRequestTypes::UnknownType));
        needRefresh = true;
    }

    if (itemType == HttpRequestTypes::UnknownType && m_textAdvisor->isContainsHeader(content)) {
        item->setType(static_cast<int>(HttpRequestTypes::HeaderType));
        needRefresh = true;
    }

    if (needRefresh) emit dataChanged(index(position, 0), index(position, 0));
}

QString HttpRequestViewModel::getTypeColor(int type) const
{
    auto requestType = static_cast<HttpRequestTypes>(type);

    switch (requestType) {
        case HttpRequestTypes::UnknownType:
            return "#CDCDB4";
        case HttpRequestTypes::UrlType:
            return  "#FDD12D";
        case HttpRequestTypes::MethodType:
            return "#F68989";
        case HttpRequestTypes::HeaderType:
            return "#8FBDD3";
        case HttpRequestTypes::BodyType:
            return "#78938A";
        case HttpRequestTypes::FormItemType:
            return "#9FC088";
        default:
            return "#CDCDB4";
    }
}
