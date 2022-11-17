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

#ifndef SHORTCUTSLISTMODEL_H
#define SHORTCUTSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "../Models/shortcutsection.h"

class ShortcutsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool isFiltered READ isFiltered NOTIFY isFilteredChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString mainMode READ mainMode NOTIFY mainModeChanged)
    Q_PROPERTY(QString openApiMode READ openApiMode NOTIFY openApiModeChanged)

private:
    const QString m_mainMode { "main" };
    const QString m_openApiMode { "openapi" };
    QString m_filter { "" };
    QString m_mode { "" };
    QList<ShortcutSection*> m_sections { QList<ShortcutSection*>() };
    QList<ShortcutSection*> m_filteredSections { QList<ShortcutSection*>() };
    enum ShortCutRoles {
        SectionTitleRole = Qt::UserRole + 1,
        ShortcutsRole,
        DescriptionRole
    };

public:
    explicit ShortcutsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString mainMode() const noexcept { return m_mainMode; }
    QString openApiMode() const noexcept { return m_openApiMode; }

    QString filter() const noexcept { return m_filter; }
    void setFilter(const QString& filter) noexcept;

    QString mode() const noexcept { return m_mode; }
    void setMode(const QString& mode) noexcept;

    bool isFiltered() const noexcept { return m_filter.isEmpty(); }

    void refresh() noexcept;

signals:
    void filterChanged();
    void isFilteredChanged();
    void modeChanged();
    void mainModeChanged();
    void openApiModeChanged();

};

#endif // SHORTCUTSLISTMODEL_H
