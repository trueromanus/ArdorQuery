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

#ifndef SHORTCUTSECTION_H
#define SHORTCUTSECTION_H

#include <QString>
#include <QList>

class ShortcutSection
{

private:
    QString m_title { "" };
    QString m_description { "" };
    QList<std::tuple<QString, QString>> m_shortcuts { QList<std::tuple<QString, QString>>() };
    QString m_formattedShortcuts { "" };

public:
    QString title() const noexcept { return m_title; }
    QString description() const noexcept { return m_description; }
    QList<std::tuple<QString, QString>> shortcuts() const noexcept { return m_shortcuts; }
    QString formattedShortcuts() const noexcept { return m_formattedShortcuts; }

    void setTitle(const QString& title) noexcept;
    void setDescription(const QString& description) noexcept;
    void addShortcut(const QString& key, const QString& description) noexcept;
    bool inFilter(const QString& filter) const noexcept;

    void formatShortcuts();

};

#endif // SHORTCUTSECTION_H
