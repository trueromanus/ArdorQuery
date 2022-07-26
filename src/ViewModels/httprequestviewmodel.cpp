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

#include <QClipboard>
#include <QGuiApplication>
#include "httprequestviewmodel.h"
#include "../globalconstants.h"

HttpRequestViewModel::HttpRequestViewModel(QObject *parent)
    : QAbstractListModel{parent}
{
    auto zeroItem = new HttpRequestItem();
    m_items->append(zeroItem);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::UrlType), 0);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::MethodType), 1);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::HeaderType), 2);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::FormItemType), 3);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::FormFileType), 4);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::HttpProtocolType), 5);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::BodyType), 6);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::UnknownType), 10);
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
            case IsFocusedRole: {
                return QVariant(rowIndex == m_selectedItem);
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
        },
        {
            IsFocusedRole,
            "isNeedFocused"
        }
    };
}

void HttpRequestViewModel::setSelectedItem(const int selectedItem) noexcept
{
    if (m_selectedItem == selectedItem) return;

    auto oldIndex = m_selectedItem;

    m_selectedItem = selectedItem;
    emit selectedItemChanged();

    if (oldIndex != selectedItem) emit dataChanged(index(oldIndex, 0), index(oldIndex, 0));
    emit dataChanged(index(selectedItem, 0), index(selectedItem, 0));
}

void HttpRequestViewModel::setTextAdvisor(const QSharedPointer<TextAdvisorViewModel> textAdviser) noexcept
{
    m_textAdvisor = textAdviser;
}

void HttpRequestViewModel::addItem(const int position, const HttpRequestViewModel::HttpRequestTypes itemType, const QString initialValue)
{
    int actualPosition = position;

    beginResetModel();

    auto item = new HttpRequestItem();
    if (itemType != HttpRequestTypes::UnknownType) item->setType(static_cast<int>(itemType));
    if (!initialValue.isEmpty()) item->setText(initialValue);

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

void HttpRequestViewModel::removeFirstItem()
{
    beginResetModel();

    m_items->removeFirst();

    endResetModel();

    if (m_selectedItem >= m_items->count()) setSelectedItem(m_items->count() - 1);
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

    if (lowerContent.startsWith(UrlPrefix) && itemType != HttpRequestTypes::UrlType) {
        item->setType(static_cast<int>(HttpRequestTypes::UrlType));
        needRefresh = true;
    }
    if (lowerContent.startsWith(MethodPrefix) && itemType != HttpRequestTypes::MethodType) {
        item->setType(static_cast<int>(HttpRequestTypes::MethodType));
        needRefresh = true;
    }

    if (lowerContent.startsWith(BodyPrefix) && itemType != HttpRequestTypes::BodyType) {
        item->setType(static_cast<int>(HttpRequestTypes::BodyType));
        needRefresh = true;
    }

    if (lowerContent.startsWith(FormPrefix) && itemType != HttpRequestTypes::FormItemType) {
        item->setType(static_cast<int>(HttpRequestTypes::FormItemType));
        needRefresh = true;
    }

    if (lowerContent.startsWith(FilePrefix) && itemType != HttpRequestTypes::FormFileType) {
        item->setType(static_cast<int>(HttpRequestTypes::FormFileType));
        needRefresh = true;
    }

    if (lowerContent.startsWith(ProtocolPrefix) && itemType != HttpRequestTypes::HttpProtocolType) {
        item->setType(static_cast<int>(HttpRequestTypes::HttpProtocolType));
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

void HttpRequestViewModel::selectUpField()
{
    if (m_selectedItem == 0) return;

    setSelectedItem(m_selectedItem - 1);
}

void HttpRequestViewModel::selectDownField()
{
    if (m_selectedItem == m_items->length() - 1) return;

    setSelectedItem(m_selectedItem + 1);
}

void HttpRequestViewModel::selectFirstField()
{
    setSelectedItem(0);
}

void HttpRequestViewModel::selectLastField()
{
    setSelectedItem(m_items->length() - 1);
}

void HttpRequestViewModel::clearFields()
{
    beginResetModel();

    m_items->clear();
    addItem(-1);

    endResetModel();

    setSelectedItem(0);
}

void HttpRequestViewModel::clearSelectedField()
{
    if (m_items->count() == 1) {
        clearFields();
        return;
    }

    beginResetModel();

    auto oldSelectedItem = m_selectedItem;
    m_items->removeAt(oldSelectedItem);

    endResetModel();

    if (oldSelectedItem > 0) setSelectedItem(oldSelectedItem - 1);
}

QString HttpRequestViewModel::getMethod() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::MethodType;
        }
    );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text();
        return text.replace(MethodPrefix, "", Qt::CaseInsensitive);
    }

    return "get";
}

QString HttpRequestViewModel::getProtocol() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::HttpProtocolType;
        }
    );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text();
        return text.replace(ProtocolPrefix, "", Qt::CaseInsensitive).toLower();
    }

    return "";
}

QString HttpRequestViewModel::getUrl() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::UrlType;
        }
    );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text();
        return text.replace("url ", "", Qt::CaseInsensitive);
    }

    return "";
}

QString HttpRequestViewModel::getBody() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::BodyType;
        }
    );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text();
        return text.mid(5);
    }

    return "";
}

QStringList HttpRequestViewModel::getFormParameters() const noexcept
{
    QStringList parameters;
    foreach (auto item, *m_items) {
        auto type = static_cast<HttpRequestTypes>(item->type());
        if (type == HttpRequestTypes::FormItemType) {
            parameters.append(item->text());
        }
    }
    return parameters;
}

QStringList HttpRequestViewModel::getFileParameters() const noexcept
{
    QStringList parameters;
    foreach (auto item, *m_items) {
        auto type = static_cast<HttpRequestTypes>(item->type());
        if (type == HttpRequestTypes::FormFileType) {
            parameters.append(item->text());
        }
    }
    return parameters;
}

QStringList HttpRequestViewModel::getHeaders() const noexcept
{
    QStringList headers;
    foreach (auto item, *m_items) {
        auto type = static_cast<HttpRequestTypes>(item->type());
        if (type == HttpRequestTypes::HeaderType) headers.append(item->text());
    }
    return headers;
}

bool HttpRequestViewModel::isOnlyEmptyFirstItem() const noexcept
{
    return m_items->count() == 1 && m_items->value(0)->text().isEmpty();
}

int HttpRequestViewModel::countItems() const noexcept
{
    return m_items->count();
}

void HttpRequestViewModel::sortingFields(const bool descending) noexcept
{
    beginResetModel();

    auto weigths = m_sortWeight.get();
    std::sort(
        m_items->begin(),
        m_items->end(),
        [weigths, descending](HttpRequestItem* left, HttpRequestItem* right) {
            return descending ? weigths->value(left->type()) > weigths->value(right->type()) : weigths->value(left->type()) < weigths->value(right->type());
        }
    );

    endResetModel();
}

QString HttpRequestViewModel::getAllFields() const noexcept
{
    QList<HttpRequestItem*> sortedList(m_items->count());

    auto weigths = m_sortWeight.get();
    std::partial_sort_copy(
        m_items->begin(),
        m_items->end(),
        sortedList.begin(),
        sortedList.end(),
        [weigths](HttpRequestItem* left, HttpRequestItem* right) {
            return weigths->value(left->type()) < weigths->value(right->type());
        }
    );

    QStringList lines(m_items->size());
    int iterator = 0;

    foreach(auto item, sortedList) {
        auto text = item->text();
        auto type = static_cast<HttpRequestTypes>(item->type());
        lines[iterator] = type != HttpRequestTypes::BodyType ? text.replace("\n", "") : text;
        iterator++;
    }

    return lines.join("\n");
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
        case HttpRequestTypes::FormFileType:
            return "#FFD9C0";
        case HttpRequestTypes::HttpProtocolType:
            return "#A5BECC";
        default:
            return "#CDCDB4";
    }
}
