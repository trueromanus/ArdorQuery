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

class ShortcutsListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool isFiltered READ isFiltered NOTIFY isFilteredChanged)
    Q_PROPERTY(QList<QVariantMap> shortcuts READ shortcuts WRITE setShortcuts NOTIFY shortcutsChanged)
    Q_PROPERTY(QList<QVariantMap> filteredShortcuts READ filteredShortcuts NOTIFY filteredShortcutsChanged)

private:
    QString m_filter { "" };
    QList<ShortcutSection*> m_sections { QList<ShortcutSection*>() };
    QList<ShortcutSection*> m_filteredSections { QList<ShortcutSection*>() };
    QList<QVariantMap> m_shortcuts { QList<QVariantMap>() };
    QList<QVariantMap> m_filteredShortcuts { QList<QVariantMap>() };
    enum ShortCutRoles {
        SectionTitleRole = Qt::UserRole + 1,
        ShortcutsRole,
        DescriptionRole
    };

public:
    explicit ShortcutsListModel(QObject *parent = nullptr);

    QString filter() const noexcept { return m_filter; }
    void setFilter(const QString& filter) noexcept;

    QList<QVariantMap> shortcuts() const noexcept { return m_shortcuts; }
    void setShortcuts(QList<QVariantMap> shortcuts) noexcept;

    QList<QVariantMap> filteredShortcuts() const noexcept { return m_filteredShortcuts; }

    bool isFiltered() const noexcept { return m_filter.isEmpty(); }

    void refresh() noexcept;

signals:
    void filterChanged();
    void isFilteredChanged();
    void mainModeChanged();
    void openApiModeChanged();
    void shortcutsChanged();
    void filteredShortcutsChanged();

};

#endif // SHORTCUTSLISTMODEL_H
