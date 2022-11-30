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

#ifndef OPENAPIADDRESSESLISTMODEL_H
#define OPENAPIADDRESSESLISTMODEL_H

#include <QAbstractListModel>
#include "../Models/openapiaddressmodel.h"

class OpenApiAddressesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasItems READ hasItems NOTIFY hasItemsChanged)

private:
    QSharedPointer<QList<OpenApiAddressModel*>> m_usedAddresses { new QList<OpenApiAddressModel*>() };
    int m_selectedIndex { -1 };
    enum OutputFormatRoles {
        IdentfierRole = Qt::UserRole + 1,
        TitleRole,
        IsSelectedRole,
        RouteRole,
        BaseUrlRole,
        FilterRole
    };

public:
    explicit OpenApiAddressesListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addAddress(const QString& title, const QString& route, const QString& baseUrl, const QString& filter) noexcept;
    QSharedPointer<QList<OpenApiAddressModel*>> getAddresses() noexcept;

    bool hasItems() const noexcept { return !m_usedAddresses->isEmpty(); }

    Q_INVOKABLE void selectItem(int index) noexcept;
    Q_INVOKABLE void deleteItem(int index) noexcept;

signals:
    void addressesChanged();
    void hasItemsChanged();

};

#endif // OPENAPIADDRESSESLISTMODEL_H
