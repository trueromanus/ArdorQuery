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
#include <QSharedPointer>
#include "../Models/openapiaddressmodel.h"

class OpenApiAddressesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasItems READ hasItems NOTIFY hasItemsChanged)

private:
    QSharedPointer<QList<OpenApiAddressModel*>> m_usedAddresses { new QList<OpenApiAddressModel*>() };
    int m_selectedIndex { -1 };
    QString m_savedOptionsFile { "saveoptions" };
    const QString m_addressField { "address" };
    const QString m_baseUrlField { "baseUrl" };
    const QString m_filterField { "filter" };
    const QString m_titleField { "title" };
    const QString m_securitiesField { "securities" };
    const QString m_addressIdField { "addressId" };
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

    void addAddress(const QString& title, const QString& route, const QString& baseUrl, const QString& filter, const QString& securities) noexcept;
    QSharedPointer<QList<OpenApiAddressModel*>> getAddresses() noexcept;

    bool hasItems() const noexcept { return !m_usedAddresses->isEmpty(); }

    void refreshItems() noexcept;

    Q_INVOKABLE void selectItem(int index) noexcept;
    Q_INVOKABLE void editItem(int index, const QString& title, const QString& route, const QString& baseUrl, const QString& filter, const QString& securities) noexcept;
    Q_INVOKABLE void deleteItem(const QString& title) noexcept;
    Q_INVOKABLE void saveSavedOptions() noexcept;

private:
    void readCache();
    void writeCache();

signals:
    void addressesChanged();
    void hasItemsChanged();

};

#endif // OPENAPIADDRESSESLISTMODEL_H
