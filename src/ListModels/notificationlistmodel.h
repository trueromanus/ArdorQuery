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

#ifndef NOTIFICATIONLISTMODEL_H
#define NOTIFICATIONLISTMODEL_H

#include <QObject>
#include <QObject>
#include <QAbstractListModel>
#include <QQueue>
#include <QDateTime>
#include <QTimer>
#include <QSharedPointer>

class NotificationListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    bool m_currentAnimating { false };
    QQueue<std::tuple<int, QDateTime>> m_currentEvents { QQueue<std::tuple<int, QDateTime>>() };
    QScopedPointer<QTimer> m_timer { new QTimer() };
    QList<std::tuple<QString, QString, QString>> m_messages { QList<std::tuple<QString, QString, QString>>() };

    enum NotificationPoolField {
        IndexField = 0,
        ExpiredField
    };

    enum NotificationPoolRoles {
        TitleRole = Qt::UserRole + 1,
        MessageRole,
        IsErrorRole,
        IsInformationRole,
        IsAnimationRole
    };

public:
    explicit NotificationListModel(QObject *parent = nullptr);
    void pushMessage() noexcept;
    void refresh() noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void itemAnimationEnded() noexcept;

    void pushErrorMessage(const QString &title, const QString &message);
    void pushInfoMessage(const QString &title, const QString &message);

signals:
    void startItemAnimation(const int index);

private slots:
    void timerTriggered();

};

#endif // NOTIFICATIONLISTMODEL_H
