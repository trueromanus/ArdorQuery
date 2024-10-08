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

#ifndef MAINTABSLISTMODEL_H
#define MAINTABSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QScopedPointer>

class MainTabsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentTab READ currentTab NOTIFY currentTabChanged)

private:
    QScopedPointer<QStringList> m_tabs { new QStringList() };
    int m_activatedTab = { 0 };

    enum PagesTabsRoles {
        TitleRole = Qt::UserRole + 1,
        IsActiveRole,
        IdentifierRole,
        IsRequestsRole,
        IsResultRole
    };

public:
    explicit MainTabsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addTab(const QString& name) noexcept;
    void removeTab(const int index) noexcept;

    QString currentTab() const noexcept { return m_tabs->value(m_activatedTab); }

    void toggleTabs() noexcept;

    Q_INVOKABLE void activateTab(const int newIndex);

signals:
    void tabActivated(const int index);
    void currentTabChanged();

};

#endif // MAINTABSLISTMODEL_H
