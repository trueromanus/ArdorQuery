#ifndef GLOBALMOUSEVIEWMODEL_H
#define GLOBALMOUSEVIEWMODEL_H

#include <QObject>

class GlobalMouseViewModel : public QObject
{
    Q_OBJECT
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

    bool moveTracking() const noexcept { return m_moveTracking; }
    void setMoveTracking(bool moveTracking) noexcept;

    int xCoordinate() const noexcept { return m_xCoordinate; }
    int yCoordinate() const noexcept { return m_yCoordinate; }

signals:
    void mouseMoved(int x, int y);
    void moveTrackingChanged();
    void xCoordinateChanged();
    void yCoordinateChanged();

};

#endif // GLOBALMOUSEVIEWMODEL_H
