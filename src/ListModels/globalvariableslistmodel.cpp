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
    readCache();
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
    if (m_variables.isEmpty()) return value;

    QString result = value;
    foreach (auto variable, m_variables.keys()) {
        auto fullVariable = "{{" + variable + "}}";
        auto value = m_variables.value(variable);
        result = result.replace(fullVariable, value);
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
}

void GlobalVariablesListModel::addVariable(const QString &name, const QString &value)
{
    m_variables.insert(name, value);
}

bool GlobalVariablesListModel::keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept
{
    Q_UNUSED(nativeCode);
    Q_UNUSED(shift);
    Q_UNUSED(alt);

    // Esc
    if (key == Qt::Key_Escape) {
        emit closeWindowRequired();
        return true;
    }

    // Ctrl-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && control) {
        addLine();
        return true;
    }

    // Ctrl-S
    if ((nativeCode == 31 || key == Qt::Key_S) && control) {
        parseLines();
        return true;
    }

    // PgUp
    if (key == Qt::Key_PageUp && !control) {
        if (m_selected > 0) setSelected(m_selected - 1);
        return true;
    }

    // PgDown
    if (key == Qt::Key_PageDown && !control) {
        if (m_selected < m_lines.count() - 1) setSelected(m_selected + 1);
        return true;
    }

    // Ctrl-PgUp
    if (key == Qt::Key_PageUp && control) {
        setSelected(0);
        return true;
    }

    // Ctrl-PgDown
    if (key == Qt::Key_PageDown && control) {
        if (!m_lines.isEmpty()) setSelected(m_lines.count() - 1);
        return true;
    }


    return false;
}

void GlobalVariablesListModel::keysReleased(int key) noexcept
{
    Q_UNUSED(key);
}

void GlobalVariablesListModel::fillLines()
{
    beginResetModel();

    m_lines.clear();
    foreach (auto key, m_variables.keys()) {
        m_lines.append(key + " " + m_variables[key]);
    }

    endResetModel();
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

    foreach (auto line, m_lines) {
        if (line.isEmpty()) continue;

        if (line.contains(" ")) {
            auto parts = line.split(" ");
            m_variables.insert(parts[0], parts[1]);
        } else {
            m_variables.insert(line, "");
        }
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
    }
}

void GlobalVariablesListModel::writeCache()
{
    QJsonArray items;
    foreach (auto key, m_variables.keys()) {
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
