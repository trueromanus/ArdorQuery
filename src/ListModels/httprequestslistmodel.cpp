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

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStandardPaths>
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

void HttpRequestsListModel::setup(QSharedPointer<TextAdvisorViewModel> textAdviser) noexcept
{
    m_textAdviser = textAdviser;
}

int HttpRequestsListModel::addItem(const HttpRequestModel* model) noexcept
{
    model->requestModel()->setTextAdvisor(m_textAdviser);

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

QString HttpRequestsListModel::getProfilePath() const noexcept
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/ardorquery.profile";
}

void HttpRequestsListModel::saveToProfile() const noexcept
{
    QJsonArray array;
    foreach (auto request, *m_requests) {
        auto requestModel = request->requestModel();
        auto lines = requestModel->getAllFieldsAsList();

        if (lines.isEmpty()) continue; // not make sense save empty queries
        if (lines.size() == 1 && lines.value(0).isEmpty()) continue; // not make sense save first query

        QJsonArray linesArray;
        foreach (auto line, lines) {
            linesArray.append(line);
        }
        QJsonObject item;
        item["lines"] = linesArray;
        array.append(item);
    }

    QJsonDocument document(array);
    auto json = document.toJson();

    auto path = getProfilePath();
    QFile profileFile(path);
    if (!profileFile.open(QFile::WriteOnly | QFile::Text)) return;

    profileFile.write(json);
    profileFile.close();
}

void HttpRequestsListModel::loadFromProfile() const noexcept
{
    auto path = getProfilePath();
    if (!QFile::exists(path)) return;

    QFile profileFile(path);
    if (!profileFile.open(QFile::ReadOnly | QFile::Text)) return;

    auto json = profileFile.readAll();
    profileFile.close();

    auto document = QJsonDocument::fromJson(json);
    auto array = document.array();
    foreach (auto arrayItem, array) {
        auto item = arrayItem.toObject();
        if (!item.contains("lines")) continue;

        auto lines = item.value("lines").toArray();
        if (lines.isEmpty()) continue;

        auto model = new HttpRequestModel(parent());

        auto request = model->requestModel();
        request->setTextAdvisor(m_textAdviser);

        if (!lines.isEmpty()) request->removeFirstItem();

        foreach (auto line, lines) {
            request->addRawLine(line.toString());
        }
        request->setSelectedItem(0);

        m_requests->append(model);
    }

    //if we load something from profile then need to remove first empty query
    if (m_requests->size() > 1) m_requests->removeAt(0);
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
