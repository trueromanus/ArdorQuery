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
        }
    };
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

void GlobalVariablesListModel::addLine()
{
    m_lines.append("");
}

void GlobalVariablesListModel::removeLine(int index)
{
    m_lines.removeAt(index);
}
