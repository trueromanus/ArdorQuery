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
