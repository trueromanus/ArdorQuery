#ifndef GLOBALMOUSEVIEWMODEL_H
#define GLOBALMOUSEVIEWMODEL_H

#include <QObject>

class GlobalMouseViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int leftEdge READ leftEdge WRITE setLeftEdge NOTIFY leftEdgeChanged)
    Q_PROPERTY(int topEdge READ topEdge WRITE setTopEdge NOTIFY topEdgeChanged)
    Q_PROPERTY(bool moveTracking READ moveTracking WRITE setMoveTracking NOTIFY moveTrackingChanged)
    Q_PROPERTY(int xCoordinate READ xCoordinate NOTIFY xCoordinateChanged)
    Q_PROPERTY(int yCoordinate READ yCoordinate NOTIFY yCoordinateChanged)

private:
    int m_leftEdge { 0 };
    int m_topEdge { 0 };
    int m_xCoordinate { 0 };
    int m_yCoordinate { 0 };
    bool m_moveTracking { false };

public:
    explicit GlobalMouseViewModel(QObject *parent = nullptr);

    bool eventFilter(QObject* watched, QEvent* event);

    int leftEdge() const noexcept { return m_leftEdge; }
    void setLeftEdge(int leftEdge) noexcept;

    int topEdge() const noexcept { return m_topEdge; }
    void setTopEdge(int topEdge) noexcept;

    bool moveTracking() const noexcept { return m_moveTracking; }
    void setMoveTracking(bool moveTracking) noexcept;

    int xCoordinate() const noexcept { return m_xCoordinate; }
    int yCoordinate() const noexcept { return m_yCoordinate; }

signals:
    void leftEdgeChanged();
    void topEdgeChanged();
    void mouseMoved(int x, int y);
    void moveTrackingChanged();
    void xCoordinateChanged();
    void yCoordinateChanged();

};

#endif // GLOBALMOUSEVIEWMODEL_H
