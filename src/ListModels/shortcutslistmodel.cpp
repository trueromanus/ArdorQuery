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
    : QAbstractListModel{parent}
{
    auto performSection = new ShortcutSection();
    performSection->setMode(m_mainMode);
    performSection->setTitle("Execution of a query");
    performSection->setDescription("Start the execution of a query or cancel the execution of an already running query");
    performSection->addShortcut("Ctrl-Z or F5", "Run request");
    performSection->addShortcut("Ctrl-B or F4", "Cancel request");
    m_sections.append(performSection);

    auto movingSection = new ShortcutSection();
    movingSection->setMode(m_mainMode);
    movingSection->setTitle("Adding fields");
    movingSection->setDescription("Create a new empty field at the end, beginning or before the selected field");
    movingSection->addShortcut("Ctrl-Enter", "Adding a new empty field below the selected field");
    movingSection->addShortcut("Alt-Enter", "Adding a new empty field on top of the selected field");
    movingSection->addShortcut("Shift-Enter", "Adding a new empty field at the end of the fields");
    m_sections.append(movingSection);

    auto removeFieldsSection = new ShortcutSection();
    removeFieldsSection->setMode(m_mainMode);
    removeFieldsSection->setTitle("Deleting fields");
    removeFieldsSection->setDescription("Delete all fields or only specific ones depending on the position");
    removeFieldsSection->addShortcut("Ctrl-R", "Delete all fields");
    removeFieldsSection->addShortcut("Alt-Shift-R", "Delete selected field");
    m_sections.append(removeFieldsSection);

    auto selectingFieldsSection = new ShortcutSection();
    selectingFieldsSection->setMode(m_mainMode);
    selectingFieldsSection->setTitle("Selecting fields");
    selectingFieldsSection->setDescription("Delete all fields or only specific ones depending on the position");
    selectingFieldsSection->addShortcut("PgUp or PgDown", "Select a text field above/below the currently selected field");
    selectingFieldsSection->addShortcut("Ctrl-PgUp or Ctrl-PgDown", "Selecting start/end a text field");
    m_sections.append(selectingFieldsSection);

    auto importFieldsSection = new ShortcutSection();
    importFieldsSection->setMode(m_mainMode);
    importFieldsSection->setTitle("Import fields");
    importFieldsSection->setDescription("Import fields from file, clipboard etc");
    importFieldsSection->addShortcut("Shift-Alt-L", "Clear all fields and load fields from clipboard");
    importFieldsSection->addShortcut("Ctrl-L or F3", "Add fields from the clipboard below the currently selected field");
    importFieldsSection->addShortcut("Alt-=", "Import fields from file");
    m_sections.append(importFieldsSection);

    auto sortingSection = new ShortcutSection();
    sortingSection->setMode(m_mainMode);
    sortingSection->setTitle("Sorting fields");
    sortingSection->setDescription("Sorting fields according to specific criteria");
    sortingSection->addShortcut("Ctrl-{", "Sorting by type in ascending order");
    sortingSection->addShortcut("Ctrl-}", "Sorting by type in descending order");
    m_sections.append(sortingSection);

    auto tabsSection = new ShortcutSection();
    tabsSection->setMode(m_mainMode);
    tabsSection->setTitle("Tab management");
    tabsSection->setDescription("Switching and opening tabs in an app");
    tabsSection->addShortcut("F11", "Toggle tabs betweens Query and Result");
    m_sections.append(tabsSection);

    auto exportSection = new ShortcutSection();
    exportSection->setMode(m_mainMode);
    exportSection->setTitle("Export fields");
    exportSection->setDescription("Export fields to clipboard, file etc");
    exportSection->addShortcut("Ctrl-S or F10", "Copy fields to clipboard");
    exportSection->addShortcut("Ctrl-=", "Save fields to file");
    m_sections.append(exportSection);

    auto bodySearchSection = new ShortcutSection();
    bodySearchSection->setMode(m_mainMode);
    bodySearchSection->setTitle("Search in response body");
    bodySearchSection->setDescription("Search result navigation in response body");
    bodySearchSection->addShortcut("Ctrl-Alt-Down", "Next founded result");
    bodySearchSection->addShortcut("Ctrl-Alt-Up", "Previous founded result");
    m_sections.append(bodySearchSection);

    auto queriesSection = new ShortcutSection();
    queriesSection->setMode(m_mainMode);
    queriesSection->setTitle("Queries management");
    queriesSection->setDescription("Creating new query, deleting existing etc");
    queriesSection->addShortcut("Ctrl-Insert", "Create new query");
    queriesSection->addShortcut("Ctrl-Tab", "holding Ctrl and further pressing Tab change opening query");
    m_sections.append(queriesSection);

    auto coptResultSection = new ShortcutSection();
    coptResultSection->setMode(m_mainMode);
    coptResultSection->setTitle("Copying results");
    coptResultSection->setDescription("Copying parts of a result and so on");
    coptResultSection->addShortcut("Shift-Alt-B", "Copy result body to clipboard");
    coptResultSection->addShortcut("Shift-Alt-H", "Copy result headers to clipboard");
    coptResultSection->addShortcut("Ctrl-8", "Generate image contains query fields, response summary and headers and save to file");
    coptResultSection->addShortcut("Alt-8", "Generate image contains query fields, response summary and headers and save to clipboard");
    m_sections.append(coptResultSection);

    auto getOpenApiSchemeSection = new ShortcutSection();
    getOpenApiSchemeSection->setMode(m_openApiMode);
    getOpenApiSchemeSection->setTitle("Getting Open API Schema");
    getOpenApiSchemeSection->setDescription("Load or reaload OpenAPI scheme from remote server");
    getOpenApiSchemeSection->addShortcut("Ctrl-Z or F5", "Load scheme");
    m_sections.append(getOpenApiSchemeSection);

    auto addressesManagement = new ShortcutSection();
    addressesManagement->setMode(m_openApiMode);
    addressesManagement->setTitle("Addresses management");
    addressesManagement->setDescription("Create SavedOption and select it later");
    addressesManagement->addShortcut("Ctrl-Insert", "Create a new SavedOption from the currently filled fields");
    addressesManagement->addShortcut("Ctrl-Home", "Edit current selected SavedOption");
    addressesManagement->addShortcut("Ctrl-Tab", "holding Ctrl and further pressing Tab change selected SavedOption");
    m_sections.append(addressesManagement);
}

int ShortcutsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filteredSections.size();
}

QVariant ShortcutsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto section = m_filteredSections.at(index.row());

    switch (role) {
        case SectionTitleRole: {
            return QVariant(section->title());
        }
        case DescriptionRole: {
            return QVariant(section->description());
        }
        case ShortcutsRole: {
            return QVariant(section->formattedShortcuts());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ShortcutsListModel::roleNames() const
{
    return {
        {
            SectionTitleRole,
            "title"
        },
        {
            ShortcutsRole,
            "shortcuts"
        },
        {
            DescriptionRole,
            "description"
        }
    };
}

void ShortcutsListModel::setFilter(const QString &filter) noexcept
{
    if (m_filter == filter) return;

    m_filter = filter;
    emit filterChanged();
    emit isFilteredChanged();

    refresh();
}

void ShortcutsListModel::setMode(const QString &mode) noexcept
{
    if (m_mode == mode) return;

    m_mode = mode;
    emit modeChanged();

    setFilter("");
    refresh();
}

void ShortcutsListModel::refresh() noexcept
{
    beginResetModel();

    m_filteredSections.clear();

    auto lowerFilter = m_filter.toLower();

    foreach (auto section, m_sections) {
        if (m_mode != section->mode()) continue;

        if (!m_filter.isEmpty()) {
            auto inTitle = section->title().toLower().contains(lowerFilter);
            auto inDescription = section->description().toLower().contains(lowerFilter);
            if (inTitle || inDescription || section->inFilter(lowerFilter)) {
                m_filteredSections.append(section);
                continue;
            }
        } else {
            m_filteredSections.append(section);
        }
    }

    endResetModel();
}
