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

#ifndef HTTPREQUESTVIEWMODEL_H
#define HTTPREQUESTVIEWMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "../Models/httprequestitem.h"

class HttpRequestViewModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedItem READ selectedItem WRITE setSelectedItem NOTIFY selectedItemChanged)

private:
    QScopedPointer<QList<HttpRequestItem*>> m_items { new QList<HttpRequestItem*>() };
    int m_selectedItem { 0 };
    enum HttpRequestRoles {
        TypeRole = Qt::UserRole + 1,
        TextRole,
        IsActiveRole,
        IndexRole,
    };

public:
    explicit HttpRequestViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int selectedItem() const noexcept { return m_selectedItem; }
    void setSelectedItem(const int selectedItem) noexcept;

    Q_INVOKABLE void addItem(const int position);
    Q_INVOKABLE void refreshItem(const int position, const QString& content);
    Q_INVOKABLE void setItemContent(const int position, const QString& content);

signals:
    void selectedItemChanged();

};

#endif // HTTPREQUESTVIEWMODEL_H
