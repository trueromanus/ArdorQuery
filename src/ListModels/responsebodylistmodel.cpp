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

#include "responsebodylistmodel.h"

ResponseBodyListModel::ResponseBodyListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int ResponseBodyListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return  m_visibleBody ? m_lines.size() : 0;
}

QVariant ResponseBodyListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();
    auto line = m_lines.at(currentIndex);

    switch (role) {
        case CurrentLineRole: {
            return QVariant(line);
        }
        case IndexRole: {
            return QVariant::fromValue(currentIndex);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ResponseBodyListModel::roleNames() const
{
    return {
        {
            CurrentLineRole,
            "currentLine"
        },
        {
            IndexRole,
            "currentIndex"
        }
    };
}

void ResponseBodyListModel::setBody(const QString &body) noexcept
{
    beginResetModel();

    m_lines.clear();
    m_lines.append(body.split("\n"));
    m_originalBody = body;

    endResetModel();
}

void ResponseBodyListModel::setVisibleBody(const bool visibleBody) noexcept
{
    if (m_visibleBody == visibleBody) return;

    beginResetModel();

    m_visibleBody = visibleBody;

    if (m_visibleBody) {
        m_lines.append(m_originalBody.split("\n"));
    } else {
        m_lines.clear();
    }

    endResetModel();

    emit visibleBodyChanged();
}
