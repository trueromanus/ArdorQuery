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
#include <QImage>
#include "../Formatters/formatterfactory.h"
#include "../globalconstants.h"

class ResponseBodyListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QImage bodyImage READ bodyImage NOTIFY bodyImageChanged)
    Q_PROPERTY(int bodyImageWidth READ bodyImageWidth NOTIFY bodyImageWidthChanged)
    Q_PROPERTY(int bodyImageHeight READ bodyImageHeight NOTIFY bodyImageHeightChanged)

private:
    QStringList m_lines { QStringList() };
    QString m_originalBody { "" };
    QScopedPointer<FormatterFactory> m_formatterFactory { new FormatterFactory() };
    QImage m_imageSource { QImage() };

    enum ResponseBodyRoles {
        CurrentLineRole = Qt::UserRole + 1,
        IndexRole
    };

public:
    explicit ResponseBodyListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setBody(const QByteArray& body, const QString& formatter) noexcept;
    void reformatting(const QString& formatter) noexcept;
    void reformatBody(const QString& formatter) noexcept;
    QString getFullBody() const noexcept;
    bool isHasBody() const noexcept;
    QImage bodyImage() const noexcept { return m_imageSource; }
    int bodyImageWidth() const noexcept { return m_imageSource.width(); }
    int bodyImageHeight() const noexcept { return m_imageSource.height(); }

signals:
    void visibleBodyChanged();
    void bodyImageChanged();
    void bodyImageWidthChanged();
    void bodyImageHeightChanged();

};

#endif // RESPONSEBODYLISTMODEL_H
