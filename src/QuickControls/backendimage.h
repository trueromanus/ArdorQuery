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

#ifndef BACKENDIMAGE_H
#define BACKENDIMAGE_H

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class BackendImage : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QImage source READ source WRITE setSource NOTIFY sourceChanged)

private:
    QImage m_source;

public:
    BackendImage();

    QImage source() const noexcept { return m_source; }
    void setSource(const QImage& image) noexcept;

    void paint(QPainter *painter);

signals:
    void sourceChanged();

};

#endif // BACKENDIMAGE_H
