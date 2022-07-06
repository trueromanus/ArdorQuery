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

#include <QPixmap>
#include "responsebodylistmodel.h"

ResponseBodyListModel::ResponseBodyListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int ResponseBodyListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return  m_lines.size();
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

void ResponseBodyListModel::setBody(const QByteArray &body, const QString& formatter) noexcept
{
    beginResetModel();

    m_originalBody = body;
    if (formatter == OutputFormatImage) {
        auto result = m_imageSource.loadFromData(body);
        if (!result) m_imageSource.load("qrc:/Views/Icons/broken.svg");
        emit bodyImageChanged();
        emit bodyImageWidthChanged();
        emit bodyImageHeightChanged();
    } else {
        //WORKAROUND: Clearing image
        if (m_imageSource.width() > 2) m_imageSource = QPixmap(2,2).toImage();

        reformatting(formatter);
    }

    endResetModel();
}

void ResponseBodyListModel::reformatting(const QString &formatter) noexcept
{
    m_lines.clear();
    auto body = getFullBody();
    auto isHasFormatter = !formatter.isEmpty();
    auto lines = body.split("\n");
    if (isHasFormatter) {
        auto formatterInstance = m_formatterFactory->getFormatter(formatter);
        lines = formatterInstance->format(body).split("\n");
    }
    foreach (auto line, lines) {
        if (isHasFormatter || line.length() < 100) {
            m_lines.append(line);
            continue;
        }

        int currentStart = 0;
        int count = line.count();

        while (currentStart < count) {
            int end = currentStart + 100;
            end = end > count ? count - currentStart : 100;

            auto part = line.mid(currentStart, end);
            m_lines.append(part);
            currentStart += 100;
        }
    }
}

void ResponseBodyListModel::reformatBody(const QString &formatter) noexcept
{
    beginResetModel();

    reformatting(formatter);

    endResetModel();
}

QString ResponseBodyListModel::getFullBody() const noexcept
{
    return m_originalBody;
}

QByteArray ResponseBodyListModel::getFullBodyArray() const noexcept
{
    return m_originalBody;
}

bool ResponseBodyListModel::isHasBody() const noexcept
{
    return !m_originalBody.isEmpty();
}
