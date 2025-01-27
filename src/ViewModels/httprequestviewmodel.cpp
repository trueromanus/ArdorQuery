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
#include <QMap>
#include "httprequestviewmodel.h"
#include "../globalconstants.h"

HttpRequestViewModel::HttpRequestViewModel(QObject *parent)
    : QAbstractListModel{parent}
{
    auto zeroItem = new HttpRequestItem();
    m_items->append(zeroItem);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::TitleType), 1);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::UrlType), 10);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::RouteType), 11);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::ParamType), 12);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::MethodType), 20);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::HeaderType), 30);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::HttpProtocolType), 31);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::BearerType), 32);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::PastryType), 33);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::FormItemType), 40);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::FormFileType), 41);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::OptionsType), 50);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::PostScriptType), 51);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::TimeoutType), 52);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::OrderType), 53);

    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::BodyType), 60);
    m_sortWeight->insert(static_cast<int>(HttpRequestTypes::UnknownType), 1000);

    fillPrefixMappings();
}

int HttpRequestViewModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_hideItems ? 0 : m_items->count();
}

QVariant HttpRequestViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (m_hideItems) return QVariant();

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
        case LastCursorPositionRole: {
            return QVariant(item->cursor());
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
        },
        {
            LastCursorPositionRole,
            "lastCursorPosition"
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

void HttpRequestViewModel::redrawAllItems() noexcept
{
    beginResetModel();

    endResetModel();
}

void HttpRequestViewModel::addItem(const int position, const HttpRequestViewModel::HttpRequestTypes itemType, const QString initialValue, const QString& alias)
{
    int actualPosition = position;

    beginResetModel();

    auto item = new HttpRequestItem();
    if (itemType != HttpRequestTypes::UnknownType) item->setType(static_cast<int>(itemType));
    if (!initialValue.isEmpty()) {
        auto prefix = alias.isEmpty() ? getItemPrefix(itemType, initialValue) : alias;
        item->setText(prefix + initialValue);
    }

    if (actualPosition == -1) {
        m_items->append(item);
        actualPosition = m_items->count() - 1;
    } else {
        if (actualPosition >= m_items->count()) actualPosition = m_items->count();
        m_items->insert(actualPosition, item);
    }

    endResetModel();

    if (itemType == HttpRequestTypes::TitleType) emit titleChanged();

    setSelectedItem(actualPosition);
}

void HttpRequestViewModel::addRawLine(const QString line)
{
    auto item = new HttpRequestItem();
    m_items->append(item);

    setItemContent(m_items->size() - 1, line);
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

    auto itemType = static_cast<HttpRequestTypes>(item->type());
    bool needRefresh = false;

    auto contentType = tryGetRequestType(content);

    if (itemType != contentType) {
        item->setType(static_cast<int>(contentType));
        needRefresh = true;
    }

    if (contentType == HttpRequestTypes::TitleType) emit titleChanged();

    if (needRefresh) emit dataChanged(index(position, 0), index(position, 0));
}

void HttpRequestViewModel::setItemCursor(int position, int cursor)
{
    auto item = m_items->value(position);
    item->setCursor(cursor);
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
        return addGetParameters(text.replace("url ", "", Qt::CaseInsensitive));
    }

    return "";
}

QString HttpRequestViewModel::addGetParameters(const QString &url) const noexcept
{
    QStringList parameters;
    QMap<QString, QString> routes;
    foreach (auto item, *m_items) {
        auto type = static_cast<HttpRequestTypes>(item->type());
        if (type == HttpRequestTypes::ParamType) {
            parameters.append(item->text().mid(6));
        }
        if (type == HttpRequestTypes::RouteType) {
            auto itemData = item->text().split(" ", Qt::SkipEmptyParts);
            if (itemData.count() < 2) continue;

            auto routeData = itemData.last().split("=");
            routes.insert(routeData.first(), routeData.last());
        }
    }

    if (parameters.isEmpty() && routes.isEmpty()) return url;

    auto parametersUrl = QString(url);

    if (!parameters.isEmpty()) {
        auto allParameters = parameters.join("&");
        parametersUrl = url.contains("?") ? url + allParameters : url + "?" + allParameters;
    }

    if (!routes.isEmpty()) {
        foreach (auto route, routes.keys()) {
            parametersUrl = parametersUrl.replace("{" + route + "}", routes.value(route));
        }
    }

    return parametersUrl;
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
    QStringList cookieValues;
    foreach (auto item, *m_items) {
        auto type = static_cast<HttpRequestTypes>(item->type());
        if (type == HttpRequestTypes::HeaderType) {
            auto text = item->text();
            auto loweredText = text.toLower();
            if (loweredText.startsWith(HeaderPrefix)) {
                headers.append(text.mid(HeaderPrefix.length()));
            } else {
                headers.append(text);
            }
        }
        if (type == HttpRequestTypes::BearerType) headers.append(item->text().replace(BearerPrefix, "Authorization Bearer "));
        if (type == HttpRequestTypes::BodyType && item->text().startsWith(JsonPrefix)) {
            headers.append("Content-Type application/json");
            headers.append("Accept application/json");
        }
        if (type == HttpRequestTypes::BodyType && item->text().startsWith(XmlPrefix)) {
            headers.append("Content-Type application/xml");
            headers.append("Accept text/xml, application/xml");
        }
        if (type == HttpRequestTypes::PastryType) {
            auto text = item->text();
            if (text.contains("=")) cookieValues.append(item->text().replace(PastryPrefix, ""));
        }
    }
    if (!cookieValues.isEmpty()) headers.append("Cookie " + cookieValues.join("; "));
    return headers;
}

QStringList HttpRequestViewModel::getOptions() const noexcept
{
    QStringList options;
    foreach (auto item, *m_items) {
        auto type = static_cast<HttpRequestTypes>(item->type());
        if (type == HttpRequestTypes::OptionsType) {
            options.append(item->text().replace(OptionsPrefix, "").split(",", Qt::SkipEmptyParts));
        }
    }
    return options;
}

QString HttpRequestViewModel::getTitle() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::TitleType;
        }
    );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text();
        auto title = text.replace("title ", "", Qt::CaseInsensitive);
        return title.trimmed().isEmpty() ? m_unnamed : title;
    }

    return m_unnamed;
}

QString HttpRequestViewModel::getPostScript() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::PostScriptType;
        }
        );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text();
        return text.replace(PostScriptPrefix, "", Qt::CaseInsensitive);
    }

    return "";
}

QString HttpRequestViewModel::getTimeout() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::TimeoutType;
        }
        );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text().replace(TimeoutPrefix, "", Qt::CaseInsensitive);
        return text;
    }

    return "";
}

int HttpRequestViewModel::getOrder() const noexcept
{
    auto iterator = std::find_if(
        m_items->begin(),
        m_items->end(),
        [](const HttpRequestItem* item) {
            auto type = static_cast<HttpRequestTypes>(item->type());
            return type == HttpRequestTypes::OrderType;
        }
        );
    if (iterator != m_items->end()) {
        auto item = *iterator;
        auto text = item->text().replace(OrderPrefix, "", Qt::CaseInsensitive);
        return text.toInt();
    }

    return 0;
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

    QStringList lines;
    lines.resize(m_items->size());
    int iterator = 0;

    foreach(auto item, sortedList) {
        auto text = item->text();
        auto type = static_cast<HttpRequestTypes>(item->type());
        lines[iterator] = type != HttpRequestTypes::BodyType ? text.replace("\n", "") : text;
        iterator++;
    }

    return lines.join("\n");
}

QStringList HttpRequestViewModel::getAllFieldsAsList() const noexcept
{
    QStringList result;
    foreach (auto item, *m_items) {
         result.append(item->text());
    }
    return result;
}

int HttpRequestViewModel::insertGlobalVariableToCursor(const QString &variable) noexcept
{
    auto item = m_items->value(m_selectedItem);
    auto content = item->text();
    auto cursorIndex = item->cursor();
    auto insertedVariable = "{{ " + variable + " }}";
    if (cursorIndex > content.size()) {
        content.append(insertedVariable);
    } else {
        content.insert(cursorIndex, insertedVariable);
    }

    item->setText(content);
    item->setCursor(cursorIndex + insertedVariable.size());

    emit dataChanged(index(m_selectedItem, 0), index(m_selectedItem, 0));

    return cursorIndex + insertedVariable.size();
}

QString HttpRequestViewModel::getTypeColor(int type) const
{
    auto requestType = static_cast<HttpRequestTypes>(type);

    switch (requestType) {
        case HttpRequestTypes::UnknownType:
            return "#CDCDB4";
        case HttpRequestTypes::UrlType:
            return  "#78D34E24";
        case HttpRequestTypes::MethodType:
            return "#788332AC";
        case HttpRequestTypes::HeaderType:
            return "#8FBDD3";
        case HttpRequestTypes::BodyType:
            return "#D7A86E";
        case HttpRequestTypes::FormItemType:
            return "#9FC088";
        case HttpRequestTypes::FormFileType:
            return "#9FC088";
        case HttpRequestTypes::HttpProtocolType:
            return "#8FBDD3";
        case HttpRequestTypes::BearerType:
            return "#8FBDD3";
        case HttpRequestTypes::TitleType:
            return "#C8E3D4";
        case HttpRequestTypes::ParamType:
            return "#78D34E24";
        case HttpRequestTypes::PastryType:
            return "#8FBDD3";
        case HttpRequestTypes::RouteType:
            return "#78D34E24";
        case HttpRequestTypes::OptionsType:
        case HttpRequestTypes::PostScriptType:
        case HttpRequestTypes::TimeoutType:
        case HttpRequestTypes::OrderType:
            return "#78FDB833";
        default:
            return "#CDCDB4";
    }
}

QString HttpRequestViewModel::getItemPrefix(const HttpRequestTypes itemType, const QString& initialValue) const noexcept
{
    QString prefix = "";
    if (itemType == HttpRequestTypes::HeaderType) {
        if (initialValue.startsWith("X-") || (m_textAdvisor != nullptr && m_textAdvisor->isContainsHeader(initialValue))) {
            prefix = "";
        } else {
            prefix = HeaderPrefix;
        }
    }

    prefix = m_requestTypesMapping.contains(itemType) ? m_requestTypesMapping.value(itemType) : "";

    return initialValue.startsWith(prefix) ? "" : prefix;
}

HttpRequestViewModel::HttpRequestTypes HttpRequestViewModel::tryGetRequestType(const QString &content)
{
    auto spaceIndex = content.indexOf(" ");
    if (spaceIndex == -1) return HttpRequestTypes::UnknownType;

    auto prefix = content.mid(0, spaceIndex + 1).toLower();
    if (m_prefixMapping.contains(prefix)) return m_prefixMapping.value(prefix);

    if (m_textAdvisor != nullptr && m_textAdvisor->isContainsHeader(prefix)) return HttpRequestTypes::HeaderType;

    return HttpRequestTypes::UnknownType;
}

void HttpRequestViewModel::fillPrefixMappings()
{
    m_prefixMapping.insert(UrlPrefix, HttpRequestTypes::UrlType);
    m_prefixMapping.insert(MethodPrefix, HttpRequestTypes::MethodType);
    m_prefixMapping.insert(HeaderPrefix, HttpRequestTypes::HeaderType);
    m_prefixMapping.insert(BodyPrefix, HttpRequestTypes::BodyType);
    m_prefixMapping.insert(JsonPrefix, HttpRequestTypes::BodyType);
    m_prefixMapping.insert(XmlPrefix, HttpRequestTypes::BodyType);
    m_prefixMapping.insert(FormPrefix, HttpRequestTypes::FormItemType);
    m_prefixMapping.insert(FilePrefix, HttpRequestTypes::FormFileType);
    m_prefixMapping.insert(ProtocolPrefix, HttpRequestTypes::HttpProtocolType);
    m_prefixMapping.insert(BearerPrefix, HttpRequestTypes::BearerType);
    m_prefixMapping.insert(TitlePrefix, HttpRequestTypes::TitleType);
    m_prefixMapping.insert(ParamPrefix, HttpRequestTypes::ParamType);
    m_prefixMapping.insert(PastryPrefix, HttpRequestTypes::PastryType);
    m_prefixMapping.insert(RoutePrefix, HttpRequestTypes::RouteType);
    m_prefixMapping.insert(OptionsPrefix, HttpRequestTypes::OptionsType);
    m_prefixMapping.insert(PostScriptPrefix, HttpRequestTypes::PostScriptType);
    m_prefixMapping.insert(TimeoutPrefix, HttpRequestTypes::TimeoutType);
    m_prefixMapping.insert(OrderPrefix, HttpRequestTypes::OrderType);

    auto keys = m_prefixMapping.keys();
    foreach (auto key, keys) {
        auto type = m_prefixMapping.value(key);
        if (!m_requestTypesMapping.contains(type)) m_requestTypesMapping.insert(type, key);
    }
}
