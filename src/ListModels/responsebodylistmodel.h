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

#ifndef RESPONSEBODYLISTMODEL_H
#define RESPONSEBODYLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

class ResponseBodyListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool visibleBody READ visibleBody WRITE setVisibleBody NOTIFY visibleBodyChanged)

private:
    QStringList m_lines { QStringList() };
    QString m_originalBody { "" };
    bool m_visibleBody { false };

    enum ResponseBodyRoles {
        CurrentLineRole = Qt::UserRole + 1,
        IndexRole
    };

public:
    explicit ResponseBodyListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setBody(const QString& body) noexcept;

    bool visibleBody() const noexcept { return m_visibleBody; }
    void setVisibleBody(const bool visibleBody) noexcept;

signals:
    void visibleBodyChanged();

};

#endif // RESPONSEBODYLISTMODEL_H
