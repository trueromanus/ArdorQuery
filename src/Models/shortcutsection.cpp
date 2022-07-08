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

#include <QScopedPointer>
#include <QStringList>
#include "shortcutsection.h"

void ShortcutSection::setTitle(const QString& title) noexcept
{
    m_title = title;
}

void ShortcutSection::setDescription(const QString& description) noexcept
{
    m_description = description;
}

void ShortcutSection::addShortcut(const QString &key, const QString &description) noexcept
{
    m_shortcuts.append(std::make_tuple(key, description));
}

bool ShortcutSection::inFilter(const QString &filter) const noexcept
{
    auto iterator = std::find_if(
        m_shortcuts.cbegin(),
        m_shortcuts.cend(),
        [filter] (std::tuple<QString, QString> item) {
            QString shortcut = std::get<0>(item).toLower();
            if (shortcut.contains(filter)) return true;

            QString description = std::get<1>(item).toLower();
            if (description.contains(filter)) return true;

            return false;
        }
    );
    return !(iterator == m_shortcuts.cend());
}

void ShortcutSection::formatShortcuts()
{
    auto lines = QScopedPointer<QStringList>(new QStringList());
    foreach (auto shortcut, m_shortcuts) {
        auto key = std::get<0>(shortcut);
        auto notes = std::get<1>(shortcut);

        auto shortcutLine = "<b>" + key + "</b>  " + "<i>" + notes + "</i>";
        lines->append(shortcutLine);
    }

    m_formattedShortcuts = lines->join("<br>");
}
