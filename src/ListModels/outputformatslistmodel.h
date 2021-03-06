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

#ifndef OUTPUTFORMATSLISTMODEL_H
#define OUTPUTFORMATSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

class OutputFormatsListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    QStringList m_outputFormats { QStringList() };
    QMap<QString, QString> m_outputFormatTitles { QMap<QString, QString>() };
    enum OutputFormatRoles {
        IdentfierRole = Qt::UserRole + 1,
        TitleRole,
        IndexRole,
    };

public:
    explicit OutputFormatsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QString getTitle(const QString& value) noexcept;

signals:

};

#endif // OUTPUTFORMATSLISTMODEL_H
