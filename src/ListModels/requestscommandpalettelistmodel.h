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

#ifndef REQUESTSCOMMANDPALETTELISTMODEL_H
#define REQUESTSCOMMANDPALETTELISTMODEL_H

#include <QAbstractListModel>
#include <QUuid>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include "../Models/httprequestmodel.h"

class RequestsCommandPaletteListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    QSharedPointer<QList<HttpRequestModel*>> m_requests;
    QMap<QUuid, HttpRequestModel*> m_requestsMap { QMap<QUuid, HttpRequestModel*>() };
    QList<QUuid> m_history { QList<QUuid>() };
    int m_selected { 0 };
    enum CommandPaletterRoles {
        IdentifierRole = Qt::UserRole + 1,
        TitleRole,
        IsSelectedRole,
        StatusIconRole
    };

public:
    explicit RequestsCommandPaletteListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<HttpRequestModel*>> requests);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void selectItem();
    void selectNext();
    void forceSelectItem(QUuid id);
    void refresh(bool needRecreateHistory = false);
    void recreateHistory();

signals:
    void itemSelected(const QUuid& id);
    void itemIndexSelected(int index);

};

#endif // REQUESTSCOMMANDPALETTELISTMODEL_H
