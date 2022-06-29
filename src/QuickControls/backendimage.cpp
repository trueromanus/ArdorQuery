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
    auto bounding_rect = boundingRect();
    auto scaled = m_source.scaledToHeight(bounding_rect.height());
    auto center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0) center.setX(0);
    if(center.y() < 0) center.setY(0);

    painter->drawImage(center, scaled);
}
