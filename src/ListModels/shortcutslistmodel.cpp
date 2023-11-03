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

#include "shortcutslistmodel.h"

ShortcutsListModel::ShortcutsListModel(QObject *parent)
    : QObject{parent}
{
    /*auto getOpenApiSchemeSection = new ShortcutSection();
    getOpenApiSchemeSection->setMode(m_openApiMode);
    getOpenApiSchemeSection->setTitle("Getting Open API Schema");
    getOpenApiSchemeSection->setDescription("Load or reaload OpenAPI scheme from remote server");
    getOpenApiSchemeSection->addShortcut("Ctrl-Z or F5", "Load scheme");
    getOpenApiSchemeSection->addShortcut("Ctrl-B or F4", "Cancel load scheme");
    m_sections.append(getOpenApiSchemeSection);

    auto addressesManagement = new ShortcutSection();
    addressesManagement->setMode(m_openApiMode);
    addressesManagement->setTitle("Addresses management");
    addressesManagement->setDescription("Create SavedOption and select it later");
    addressesManagement->addShortcut("Ctrl-Insert", "Create a new SavedOption from the currently filled fields");
    addressesManagement->addShortcut("Ctrl-Home", "Edit current selected SavedOption");
    addressesManagement->addShortcut("Ctrl-Tab", "holding Ctrl and further pressing Tab change selected SavedOption");
    m_sections.append(addressesManagement);*/
}

void ShortcutsListModel::setFilter(const QString &filter) noexcept
{
    if (m_filter == filter) return;

    m_filter = filter;
    emit filterChanged();
    emit isFilteredChanged();

    refresh();
}

void ShortcutsListModel::setShortcuts(QList<QVariantMap> shortcuts) noexcept
{
    if (m_shortcuts == shortcuts) return;

    m_shortcuts = shortcuts;
    emit shortcutsChanged();

    refresh();
}

void ShortcutsListModel::refresh() noexcept
{
    auto lowerFilter = m_filter.toLower();

    m_filteredShortcuts.clear();

    if (lowerFilter.isEmpty()) {
        m_filteredShortcuts.append(m_shortcuts);
        emit filteredShortcutsChanged();
        return;
    }

    foreach (auto shortcut, m_shortcuts) {
        auto isDescription = shortcut.value("description").toString().toLower().contains(lowerFilter);
        auto isShortcuts = shortcut.value("shortcuts").toString().toLower().contains(lowerFilter);
        if (isDescription || isShortcuts) m_filteredShortcuts.append(shortcut);
    }

    emit filteredShortcutsChanged();
}
