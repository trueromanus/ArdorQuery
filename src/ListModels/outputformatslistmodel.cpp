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

#include "outputformatslistmodel.h"
#include "../globalconstants.h"

OutputFormatsListModel::OutputFormatsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_outputFormats.append(OutputFormatAuto);
    m_outputFormats.append(OutputFormatJson);
    m_outputFormats.append(OutputFormatXml);
    m_outputFormats.append(OutputFormatHtml);
    m_outputFormats.append(OutputFormatPlainText);
    m_outputFormats.append(OutputFormatImage);
    m_outputFormats.append(OutputNeedDownloaded);

    m_outputFormatTitles.insert(OutputFormatAuto, "Auto");
    m_outputFormatTitles.insert(OutputFormatJson, "JSON");
    m_outputFormatTitles.insert(OutputFormatXml, "XML document");
    m_outputFormatTitles.insert(OutputFormatImage, "Image");
    m_outputFormatTitles.insert(OutputFormatHtml, "HTML");
    m_outputFormatTitles.insert(OutputNeedDownloaded, "Attachment");
    m_outputFormatTitles.insert(OutputFormatPlainText, "Plain Text");
}

int OutputFormatsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_outputFormats.size();
}

QVariant OutputFormatsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto format = m_outputFormats.at(index.row());

    switch (role) {
        case IdentfierRole: {
            return QVariant(format);
        }
        case TitleRole: {
            return QVariant(m_outputFormatTitles.value(format));
        }
        case IndexRole: {
            return QVariant(index.row());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> OutputFormatsListModel::roleNames() const
{
    return {
        {
            IdentfierRole,
            "identifier"
        },
        {
            TitleRole,
            "title"
        },
        {
            IndexRole,
            "itemIndex"
        }
    };
}

QString OutputFormatsListModel::getTitle(const QString &value) noexcept
{
    if (!m_outputFormatTitles.contains(value)) return "";

    return m_outputFormatTitles.value(value);
}
