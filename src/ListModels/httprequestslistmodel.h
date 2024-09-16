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

#ifndef HTTPREQUESTSLISTMODEL_H
#define HTTPREQUESTSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QUuid>
#include <QSharedPointer>
#include "../Models/httprequestmodel.h"

class HttpRequestsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(HttpRequestModel* selectedItem READ selectedItem NOTIFY selectedItemChanged)

private:
    QSharedPointer<QList<HttpRequestModel*>> m_requests { new QList<HttpRequestModel*>() };
    QSharedPointer<TextAdvisorViewModel> m_textAdviser;
    int m_selectedIndex { 0 };

    enum HttpRequestsRoles {
        RequestTitleRole = Qt::UserRole + 1,
        ViewModelRole,
        ResultViewModelRole,
        IsSelectedRole,
        IndexRole,
    };

public:
    explicit HttpRequestsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setup(QSharedPointer<TextAdvisorViewModel> textAdviser) noexcept;

    HttpRequestModel* selectedItem() const noexcept { return m_requests->at(m_selectedIndex); }

    int addItem(const HttpRequestModel* model) noexcept;
    bool singleRequest() const noexcept;
    void deleteSelectedItem() noexcept;

    QSharedPointer<QList<HttpRequestModel*>> getList() const noexcept;

    QString getProfilePath() const noexcept;
    void saveToProfile() const noexcept;
    void loadFromProfile() const noexcept;

    HttpRequestModel* getSelectedRequest() const noexcept;

    Q_INVOKABLE void selectItem(const int newIndex) noexcept;
    Q_INVOKABLE void selectItemById(const QUuid& id) noexcept;

signals:
    void selectedItemChanged();

};

#endif // HTTPREQUESTSLISTMODEL_H
