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

#include "globalvariableslistmodel.h"

GlobalVariablesListModel::GlobalVariablesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

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

void GlobalVariablesListModel::replaceGlobalVariables(QString &value)
{
    QString result;
    foreach (auto variable, m_variables) {
        auto fullVariable = "{{" + variable + "}}";
        result = value.replace(fullVariable, m_variables.value(variable));
    }
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

    m_lines.append("");

    endResetModel();
}

bool GlobalVariablesListModel::keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept
{
    Q_UNUSED(nativeCode);
    Q_UNUSED(shift);
    Q_UNUSED(alt);

    // Ctrl-Insert
    if (key == Qt::Key_Insert && control) {
        addLine();
    }

    return false;
}

void GlobalVariablesListModel::keysReleased(int key) noexcept
{
    Q_UNUSED(key);
}

void GlobalVariablesListModel::fillLines()
{
    foreach (auto key, m_variables.keys()) {
        m_lines.append(key + " " + m_variables[key]);
    }
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
