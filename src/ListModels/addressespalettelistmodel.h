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

#ifndef ADDRESSESPALETTELISTMODEL_H
#define ADDRESSESPALETTELISTMODEL_H

#include <QAbstractListModel>
#include <QUuid>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include "../Models/openapiaddressmodel.h"

class AddressesPaletteListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QSharedPointer<QList<OpenApiAddressModel*>> m_addresses;
    QMap<QUuid, OpenApiAddressModel*> m_addressesMap { QMap<QUuid, OpenApiAddressModel*>() };
    QList<QUuid> m_history { QList<QUuid>() };
    int m_selected { 0 };
    enum AddressPaletterRoles {
        IdentifierRole = Qt::UserRole + 1,
        TitleRole,
        IsSelectedRole
    };

public:
    explicit AddressesPaletteListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<OpenApiAddressModel*>> addresses);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void selectItem();
    void selectNext();
    Q_INVOKABLE void selectItemByAddressIndex(int index);
    void refresh(bool needRecreateHistory = false);
    void recreateHistory();
    OpenApiAddressModel* getSelectedAddressById(const QUuid& id);
    int getSelectedAddressIndex();

signals:
    void itemSelected(const QUuid& id);

};

#endif // ADDRESSESPALETTELISTMODEL_H
