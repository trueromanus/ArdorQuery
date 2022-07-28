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

#include "notificationlistmodel.h"
#include "../globalenums.h"
#include "../globalconstants.h"

NotificationListModel::NotificationListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_timer->setInterval(500);
    connect(m_timer.get(), &QTimer::timeout, this, &NotificationListModel::timerTriggered);
}

void NotificationListModel::pushMessage() noexcept
{
    auto expired = QDateTime::currentDateTime().addSecs(2);
    m_currentEvents.enqueue(std::make_tuple(m_messages.count() - 1, expired));
    if (!m_timer->isActive()) m_timer->start();
    refresh();
}

void NotificationListModel::refresh() noexcept
{
    beginResetModel();
    endResetModel();
}

int NotificationListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_currentEvents.size();
}

QVariant NotificationListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentEvent = m_currentEvents.at(index.row());
    auto indexOfItem = std::get<IndexField>(currentEvent);
    auto item = m_messages.at(indexOfItem);
    auto topic = std::get<NotificationTopicField>(item);

    switch (role) {
        case TitleRole: {
            return QVariant(std::get<NotificationTitleField>(item));
        }
        case MessageRole: {
            return QVariant(std::get<NotificationMessageField>(item));
        }
        case IsErrorRole: {
            return QVariant(topic == NotificationErrorTopic);
        }
        case IsInformationRole: {
            return QVariant(topic == NotificationInfoTopic);
        }
        case IsAnimationRole: {
            return m_currentAnimating && index.row() == 0;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> NotificationListModel::roleNames() const
{
    return {
        {
            TitleRole,
            "title"
        },
        {
            MessageRole,
            "message"
        },
        {
            IsErrorRole,
            "isError"
        },
        {
            IsInformationRole,
            "isInformation"
        },
        {
            IsAnimationRole,
            "isActiveAnimation"
        }
    };
}

void NotificationListModel::itemAnimationEnded() noexcept
{
    m_currentAnimating = false;

    beginResetModel();
    m_currentEvents.dequeue();
    endResetModel();

    if (m_currentEvents.isEmpty()) m_timer->stop();
}

void NotificationListModel::pushErrorMessage(const QString &title, const QString &message)
{
    m_messages.append(std::make_tuple(title, message, NotificationErrorTopic));
    pushMessage();
}

void NotificationListModel::pushInfoMessage(const QString &title, const QString &message)
{
    m_messages.append(std::make_tuple(title, message, NotificationInfoTopic));
    pushMessage();
}

void NotificationListModel::timerTriggered()
{
    if (m_currentAnimating) return;
    if (m_currentEvents.isEmpty()) return;

    auto expired = std::get<ExpiredField>(m_currentEvents.head());
    if (QDateTime::currentDateTime() >= expired) {
        m_currentAnimating = true;
        emit dataChanged(index(0), index(0));
    }
}
