#include "outputformatslistmodel.h"
#include "../globalconstants.h"

OutputFormatsListModel::OutputFormatsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_outputFormats.append(OutputFormatAuto);
    m_outputFormats.append(OutputFormatJson);
    m_outputFormats.append(OutputFormatXml);
    m_outputFormats.append(OutputFormatHtml);
    m_outputFormats.append(OutputFormatImage);

    m_outputFormatTitles.insert(OutputFormatAuto, "Auto");
    m_outputFormatTitles.insert(OutputFormatJson, "JSON");
    m_outputFormatTitles.insert(OutputFormatXml, "XML document");
    m_outputFormatTitles.insert(OutputFormatImage, "Image");
    m_outputFormatTitles.insert(OutputFormatHtml, "HTML");
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
