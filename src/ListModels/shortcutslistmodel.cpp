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
    performSection->setTitle("Execution of a query");
    performSection->setDescription("Start the execution of a query or cancel the execution of an already running query");
    performSection->addShortcut("Ctrl-Z or F5", "Run request");
    performSection->addShortcut("Ctrl-B or F4", "Cancel request");
    performSection->formatShortcuts();
    m_sections.append(performSection);

    auto movingSection = new ShortcutSection();
    movingSection->setTitle("Adding fields");
    movingSection->setDescription("Create a new empty field at the end, beginning or before the selected field");
    movingSection->addShortcut("Ctrl-Enter", "Adding a new empty field below the selected field");
    movingSection->addShortcut("Alt-Enter", "Adding a new empty field on top of the selected field");
    movingSection->addShortcut("Shift-Enter", "Adding a new empty field at the end of the fields");
    movingSection->formatShortcuts();
    m_sections.append(movingSection);

    auto removeFieldsSection = new ShortcutSection();
    removeFieldsSection->setTitle("Deleting fields");
    removeFieldsSection->setDescription("Delete all fields or only specific ones depending on the position");
    removeFieldsSection->addShortcut("Ctrl-R", "Delete all fields");
    removeFieldsSection->addShortcut("Shift-R", "Delete selected field");
    removeFieldsSection->formatShortcuts();
    m_sections.append(removeFieldsSection);

    auto selectingFieldsSection = new ShortcutSection();
    selectingFieldsSection->setTitle("Selecting fields");
    selectingFieldsSection->setDescription("Delete all fields or only specific ones depending on the position");
    selectingFieldsSection->addShortcut("PgUp or PgDown", "Select a text field above/below the currently selected field");
    selectingFieldsSection->addShortcut("Ctrl-PgUp or Ctrl-PgDown", "Selecting start/end a text field");
    selectingFieldsSection->formatShortcuts();
    m_sections.append(selectingFieldsSection);

    auto importFieldsSection = new ShortcutSection();
    importFieldsSection->setTitle("Import fields");
    importFieldsSection->setDescription("Import fields from file, clipboard etc");
    importFieldsSection->addShortcut("Shift-L", "Clear all fields and load fields from clipboard");
    importFieldsSection->addShortcut("Ctrl-L or F3", "Add fields from the clipboard below the currently selected field");
    importFieldsSection->formatShortcuts();
    m_sections.append(importFieldsSection);

    auto sortingSection = new ShortcutSection();
    sortingSection->setTitle("Sorting fields");
    sortingSection->setDescription("Sorting fields according to specific criteria");
    sortingSection->addShortcut("Ctrl-{", "Sorting by type in ascending order");
    sortingSection->addShortcut("Ctrl-}", "Sorting by type in descending order");
    sortingSection->formatShortcuts();
    m_sections.append(sortingSection);

    auto tabsSection = new ShortcutSection();
    tabsSection->setTitle("Tab management");
    tabsSection->setDescription("Switching and opening tabs in an app");
    tabsSection->addShortcut("F11", "Toggle tabs betweens Query and Result");
    m_sections.append(sortingSection);
}

int ShortcutsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filter.isEmpty() ? m_sections.size() : m_filteredSections.size();
}

QVariant ShortcutsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto section = m_filter.isEmpty() ? m_sections.at(index.row()) : m_filteredSections.at(index.row());

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

void ShortcutsListModel::refresh() noexcept
{
    beginResetModel();

    m_filteredSections.clear();

    if (!m_filter.isEmpty()) {
        auto lowerFilter = m_filter.toLower();
        foreach (auto section, m_sections) {
            auto inTitle = section->title().toLower().contains(lowerFilter);
            auto inDescription = section->description().toLower().contains(lowerFilter);
            if (inTitle || inDescription || section->inFilter(lowerFilter)) {
                m_filteredSections.append(section);
                continue;
            }
        }
    }

    endResetModel();
}
