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

class OpenApiAddressesListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QStringList m_usedAddresses { QStringList() };
    int m_selectedIndex { -1 };
    enum OutputFormatRoles {
        IdentfierRole = Qt::UserRole + 1,
        TitleRole,
        IsSelectedRole,
    };

public:
    explicit OpenApiAddressesListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addAddress(const QString& address) noexcept;

    Q_INVOKABLE void selectItem(int index) noexcept;
    Q_INVOKABLE void deleteItem(int index) noexcept;

signals:

};

#endif // OPENAPIADDRESSESLISTMODEL_H
