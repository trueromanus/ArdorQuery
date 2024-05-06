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

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "globalvariableslistmodel.h"
#include "../globalhelpers.h"

GlobalVariablesListModel::GlobalVariablesListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_savedGlobalVariablesFile = getCachePath(m_savedGlobalVariablesFile);
    createIfNotExistsFile(m_savedGlobalVariablesFile, "[]");
    m_tabs.append(VariablesTab);
    m_selectedTab = VariablesTab;
    readCache();
    fillMappings();
    fillCommands();
    fillHelpShortcuts();
    m_globalVariableHandlers.insert(m_dateTimeNowGlobalVariable, &GlobalVariablesListModel::dateTimeNowVariable);
    m_globalVariableHandlers.insert(m_dateTimeUtcNowGlobalVariable, &GlobalVariablesListModel::dateTimeUtcNowVariable);
    m_globalVariableHandlers.insert(m_time24HoursNowGlobalVariable, &GlobalVariablesListModel::timeNowVariable);
}

int GlobalVariablesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_lines.size();
}

QVariant GlobalVariablesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto identifier = index.row();

    switch (role) {
        case LineRole: {
            return QVariant(m_lines.at(identifier));
        }
        case IdentifierRole: {
            return QVariant(identifier);
        }
        case IsActiveRole:{
            return QVariant(identifier == m_selected);
        }
        case IsOddRole: {
            return QVariant(identifier % 2 == 1);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> GlobalVariablesListModel::roleNames() const
{
    return {
        {
            LineRole,
            "line"
        },
        {
            IdentifierRole,
            "identifier"
        },
        {
            IsActiveRole,
            "isActive"
        },
        {
            IsOddRole,
            "isOdd"
        }
    };
}

QString GlobalVariablesListModel::replaceGlobalVariables(const QString &value)
{
    QString result = value;

    if (!m_variables.isEmpty()) {
        foreach (auto variable, m_variables.keys()) {
            auto fullVariable = "{{" + variable + "}}";
            auto fullVariableSpaces = "{{ " + variable + " }}";
            auto value = m_variables.value(variable);
            result = result.replace(fullVariable, value).replace(fullVariableSpaces, value);
        }
    }

    foreach (auto key, m_globalVariableHandlers.keys()) {
        auto item = m_globalVariableHandlers.value(key);
        result = std::invoke(item, this, result);
    }

    return result;
}

void GlobalVariablesListModel::setSelected(int selected) noexcept
{
    if (selected == m_selected) return;

    auto oldIndex = m_selected;

    m_selected = selected;
    emit selectedChanged();
    emit dataChanged(index(oldIndex), index(oldIndex));
    emit dataChanged(index(m_selected), index(m_selected));
}

void GlobalVariablesListModel::setHelpVisible(bool helpVisible) noexcept
{
    if (m_helpVisible == helpVisible) return;

    m_helpVisible = helpVisible;
    emit helpVisibleChanged();
}

void GlobalVariablesListModel::setFilterMode(const QString &mode) noexcept
{
    m_globalVariablesPopupMode = mode;
    emit variableNamesChanged();
}

void GlobalVariablesListModel::addLine()
{
    beginResetModel();

    if (m_lines.isEmpty()) {
        m_lines.append("");
    } else {
        auto selected = m_selected + 1;
        m_lines.insert(selected, "");
        setSelected(selected);
    }

    endResetModel();

    emit hasLinesChanged();
}

void GlobalVariablesListModel::addLineBefore()
{
    beginResetModel();

    if (m_lines.isEmpty()) {
        m_lines.append("");
    } else {
        m_lines.insert(m_selected, "");
    }

    endResetModel();

    emit hasLinesChanged();
}

void GlobalVariablesListModel::addLineToEnd()
{
    beginResetModel();

    m_lines.append("");

    endResetModel();
    setSelected(m_lines.size() - 1);

    emit hasLinesChanged();
}

void GlobalVariablesListModel::removeSelectedLine()
{
    beginResetModel();

    m_lines.removeAt(m_selected);

    endResetModel();

    setSelected(m_selected >= m_lines.size() ? m_selected - 1 : m_selected);

    emit hasLinesChanged();
}

void GlobalVariablesListModel::removeAllLines()
{
    beginResetModel();

    m_lines.clear();

    endResetModel();

    setSelected(0);

    emit hasLinesChanged();
}

void GlobalVariablesListModel::addVariable(const QString &name, const QString &value)
{
    m_variables.insert(name, value);
}

void GlobalVariablesListModel::setNeedShowSaveNotifications(bool needShowSaveNotifications) noexcept
{
    if (m_needShowSaveNotifications == needShowSaveNotifications) return;

    m_needShowSaveNotifications = needShowSaveNotifications;
    emit needShowSaveNotificationsChanged();
}

void GlobalVariablesListModel::setSelectedTab(const QString &selectedTab) noexcept
{
    if (m_selectedTab == selectedTab) return;

    m_selectedTab = selectedTab;
    emit selectedTabChanged();
}

QStringList GlobalVariablesListModel::variableNames() const noexcept
{
    if (m_globalVariablesPopupMode == "predefined") {
        return m_globalVariableHandlers.keys();
    }
    return m_variables.keys();
}

void GlobalVariablesListModel::refreshVariableNames() noexcept
{
    emit variableNamesChanged();
}

bool GlobalVariablesListModel::shortcutHandler(const QString &shortcut) noexcept
{
    if (!m_shortcutCommandMapping.contains(shortcut)) return false;

    auto command = m_shortcutCommandMapping.value(shortcut);

    if (command == m_addNewLineBelowCommand) {
        addLine();
    } else if (command == m_saveGlobalVariablesCommand) {
        parseLines();
    } else if (command == m_closeWindowCommand) {
        fillLines();
        emit closeWindowRequired();
    } else if (command == m_selectPreviousFieldCommand) {
        if (m_selected > 0) setSelected(m_selected - 1);
    } else if (command == m_selectNextFieldCommand) {
        if (m_selected < m_lines.count() - 1) setSelected(m_selected + 1);
    } else if (command == m_selectTopFieldCommand) {
        setSelected(0);
    } else if (command == m_selectLastFieldCommand) {
        if (!m_lines.isEmpty()) setSelected(m_lines.count() - 1);
    } else if (command == m_addLineAboveCommand) {
        addLineBefore();
    } else if (command == m_addLineToEndCommand) {
        addLineToEnd();
    } else if (command == m_removeSelectedFieldCommand) {
        removeSelectedLine();
    } else if (command == m_removeAllFieldCommand) {
        removeAllLines();
    } else if (command == m_helpCommand) {
        setHelpVisible(!m_helpVisible);
    }

    return true;
}

void GlobalVariablesListModel::fillLines()
{
    beginResetModel();

    m_lines.clear();
    foreach (auto key, m_variablesOrders) {
        m_lines.append(key + " " + m_variables[key]);
    }

    endResetModel();

    emit hasLinesChanged();
    if (m_selected >= m_lines.size()) setSelected(0);
}

void GlobalVariablesListModel::clearLines()
{
    m_variables.clear();
    foreach (auto line, m_lines) {
        auto parts = line.split(" ");
        if (parts.count() != 2) break;

        m_variables.insert(parts[0], parts[1]);
    }
    m_lines.clear();

}

void GlobalVariablesListModel::setLine(int identifier, const QString &value)
{
    if (identifier >= m_lines.count()) return;

    m_lines[identifier] = value;
}

void GlobalVariablesListModel::removeLine(int index)
{
    beginResetModel();

    m_lines.removeAt(index);

    endResetModel();
}

void GlobalVariablesListModel::parseLines()
{
    m_variables.clear();
    m_variablesOrders.clear();

    foreach (auto line, m_lines) {
        if (line.isEmpty()) continue;
        if (!line.contains(" ")) continue;

        auto parts = line.split(" ");
        if (parts[0].isEmpty() || parts[1].isEmpty()) continue;

        m_variables.insert(parts[0], parts[1]);
        m_variablesOrders.append(parts[0]);
    }

    writeCache();
}

void GlobalVariablesListModel::readCache()
{
    auto file = QFile(m_savedGlobalVariablesFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    auto content = file.readAll();

    file.close();

    auto document = QJsonDocument::fromJson(content);
    auto items = document.array();
    for (const auto &item : items){
        if (!item.isObject()) continue;

        auto variableObject = item.toObject();
        if (!variableObject.contains("name")) continue;

        auto name = variableObject.value("name").toString();
        auto value = variableObject.contains("value") ? variableObject.value("value").toString() : "";
        m_variables.insert(name, value);
        m_variablesOrders.append(name);
    }
}

void GlobalVariablesListModel::writeCache()
{
    QJsonArray items;
    foreach (auto key, m_variablesOrders) {
        auto value = m_variables.value(key);
        QJsonObject jsonObject;
        jsonObject["name"] = key;
        if (!value.isEmpty()) jsonObject["value"] = value;

        items.append(jsonObject);
    }

    QJsonDocument document(items);
    auto json = document.toJson();

    QFile file(m_savedGlobalVariablesFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    file.write(json);
    file.close();
}

void GlobalVariablesListModel::fillMappings()
{
    m_shortcutCommandMapping.insert("control-enter", m_addNewLineBelowCommand);
    m_shortcutCommandMapping.insert("control-s", m_saveGlobalVariablesCommand);
    m_shortcutCommandMapping.insert("control-pagedown", m_selectLastFieldCommand);
    m_shortcutCommandMapping.insert("pagedown", m_selectNextFieldCommand);
    m_shortcutCommandMapping.insert("control-pageup", m_selectTopFieldCommand);
    m_shortcutCommandMapping.insert("pageup", m_selectPreviousFieldCommand);
    m_shortcutCommandMapping.insert("control-h", m_helpCommand);
    m_shortcutCommandMapping.insert("f1", m_helpCommand);
    m_shortcutCommandMapping.insert("escape", m_closeWindowCommand);

    m_shortcutCommandMapping.insert("alt-enter", m_addLineAboveCommand);
    m_shortcutCommandMapping.insert("shift-enter", m_addLineToEndCommand);
    m_shortcutCommandMapping.insert("control-r", m_removeSelectedFieldCommand);
    m_shortcutCommandMapping.insert("shift-alt-r", m_removeAllFieldCommand);
}

void GlobalVariablesListModel::fillCommands()
{
    m_shortcutCommands.insert(m_addNewLineBelowCommand, "Adding a new empty field below the selected field");
    m_shortcutCommands.insert(m_saveGlobalVariablesCommand, "Save current global variables");
    m_shortcutCommands.insert(m_selectLastFieldCommand, "Selecting end a text field");
    m_shortcutCommands.insert(m_selectNextFieldCommand, "Select a text field below the currently selected field");
    m_shortcutCommands.insert(m_selectTopFieldCommand, "Selecting start a text field");
    m_shortcutCommands.insert(m_selectPreviousFieldCommand, "Select a text field above the currently selected field");
    m_shortcutCommands.insert(m_helpCommand, "Show interactive help for shortcuts");
    m_shortcutCommands.insert(m_closeWindowCommand, "Close Global Variables window");

    m_shortcutCommands.insert(m_addLineAboveCommand, "Adding a new empty line on top of the selected line");
    m_shortcutCommands.insert(m_addLineToEndCommand, "Adding a new empty line at the end of the lines");
    m_shortcutCommands.insert(m_removeSelectedFieldCommand, "Remove selected line");
    m_shortcutCommands.insert(m_removeAllFieldCommand, "Remove all lines");
}

void GlobalVariablesListModel::fillHelpShortcuts()
{
    m_shortcuts.clear();

    auto mappingIterator = QMapIterator(m_shortcutCommandMapping);
    QMultiMap<QString, QString> commandWithShortcuts;
    while (mappingIterator.hasNext()) {
        mappingIterator.next();
        commandWithShortcuts.insert(mappingIterator.value(), mappingIterator.key());
    }

    auto iterator = QMapIterator(m_shortcutCommands);

    while (iterator.hasNext()) {
        iterator.next();

        auto key = iterator.key();
        auto value = iterator.value();
        QVariantMap map;
        map["description"] = value;
        auto shortcuts = commandWithShortcuts.values(key);
        map["shortcuts"] = shortcuts.join(" or ")
#ifdef Q_OS_MACOS
           .replace("control", "Command")
           .replace("alt", "Option")
#else
           .replace("control", "Control")
           .replace("alt", "Alt")
#endif
           .replace("shift", "Shift")
           .replace("page", "Page")
           .replace("up", "Up")
           .replace("down", "Down")
           .replace("enter", "Enter")
           .replace("insert", "Insert")
           .replace("delete", "Delete")
           .replace("tab", "Tab")
           .replace("backspace", "Backspace")
           .replace("plus", "Plus")
           .replace("minus", "Minus")
           .replace("escape", "Escape")
           .replace("f1", "F1")
           .replace("f2", "F2")
           .replace("f3", "F3")
           .replace("f4", "F4")
           .replace("f5", "F5")
           .replace("f6", "F6")
           .replace("f7", "F7")
           .replace("f8", "F8")
           .replace("f9", "F9")
           .replace("f10", "F10")
           .replace("f11", "F11")
           .replace("f12", "F12");

        m_shortcuts.append(map);
    }

    emit shortcutsChanged();
}

QString GlobalVariablesListModel::dateTimeNowVariable(const QString &content)
{
    if (content.indexOf(m_dateTimeNowGlobalVariable) == -1) return content;

    QDateTime date = QDateTime::currentDateTime();
    QString dateAsAstring = date.toString(Qt::ISODate);
    auto nospaced = "{{" + m_dateTimeNowGlobalVariable + "}}";
    auto spaced = "{{ " + m_dateTimeNowGlobalVariable + " }}";
    return QString(content).replace(nospaced, dateAsAstring).replace(spaced, dateAsAstring);
}

QString GlobalVariablesListModel::dateTimeUtcNowVariable(const QString &content)
{
    if (content.indexOf(m_dateTimeUtcNowGlobalVariable) == -1) return content;

    QDateTime date = QDateTime::currentDateTimeUtc();
    QString dateAsAstring = date.toString(Qt::ISODate);
    auto nospaced = "{{" + m_dateTimeUtcNowGlobalVariable + "}}";
    auto spaced = "{{ " + m_dateTimeUtcNowGlobalVariable + " }}";
    return QString(content).replace(nospaced, dateAsAstring).replace(spaced, dateAsAstring);
}

QString GlobalVariablesListModel::timeNowVariable(const QString &content)
{
    if (content.indexOf(m_time24HoursNowGlobalVariable) == -1) return content;

    auto time = QDateTime::currentDateTimeUtc().time();
    QString timeAsAstring = time.toString("hh:mm:ss");
    auto nospaced = "{{" + m_time24HoursNowGlobalVariable + "}}";
    auto spaced = "{{ " + m_time24HoursNowGlobalVariable + " }}";
    return QString(content).replace(nospaced, timeAsAstring).replace(spaced, timeAsAstring);
}
