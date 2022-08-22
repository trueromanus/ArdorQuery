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

#include "backendimage.h"

BackendImage::BackendImage()
{

}

void BackendImage::setSource(const QImage &image) noexcept
{
    if (m_source == image) return;

    m_source = image;
    emit sourceChanged();

    update();
}

void BackendImage::paint(QPainter *painter)
{
    if (m_source.height() > height()) {
        auto bounding_rect = boundingRect();
        auto scaled = m_source.scaledToHeight(bounding_rect.height());
        auto center = bounding_rect.center() - scaled.rect().center();

        if(center.x() < 0) center.setX(0);
        if(center.y() < 0) center.setY(0);

        painter->drawImage(center, scaled);
        return;
    }
    if (m_source.height() < height()) {
        auto bounding_rect = boundingRect();
        auto center = bounding_rect.center() - m_source.rect().center();

        if(center.x() < 0) center.setX(0);
        if(center.y() < 0) center.setY(0);

        painter->drawImage(center, m_source);
        return;
    }

    //if original size not need processing
    painter->drawImage(QPoint(0,0), m_source);
}
