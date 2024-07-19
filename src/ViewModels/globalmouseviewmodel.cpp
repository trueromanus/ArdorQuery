#include <QGuiApplication>
#include <QMouseEvent>
#include "globalmouseviewmodel.h"

GlobalMouseViewModel::GlobalMouseViewModel(QObject *parent)
    : QObject{parent}
{
    QGuiApplication::instance()->installEventFilter(this);
}

bool GlobalMouseViewModel::eventFilter(QObject* watched, QEvent* event)
{
    if (!m_moveTracking) return QObject::eventFilter(watched, event);
    if (!event->spontaneous()) return QObject::eventFilter(watched, event);

    QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseMove) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        auto position = mouseEvent->position();
        m_xCoordinate = position.x();
        m_yCoordinate = position.y();
        if (m_xCoordinate >= m_leftEdge && m_yCoordinate >= m_topEdge) {
            emit mouseMoved(m_xCoordinate, m_yCoordinate);
            return true;
        }
    }

    //need to handle when mouse will be released for finish operation
    if (m_moveTracking && eventType == QEvent::MouseButtonRelease) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        auto button = mouseEvent->button();
        if (button == Qt::RightButton) {
            setMoveTracking(false);
            return true;
        }
    }

    if (m_moveTracking && eventType == QEvent::Wheel) {
        setMoveTracking(false);
        return true;
    }

    return QObject::eventFilter(watched, event);
}

void GlobalMouseViewModel::setMoveTracking(bool moveTracking) noexcept
{
    if (m_moveTracking == moveTracking) return;

    m_moveTracking = moveTracking;
    emit moveTrackingChanged();
}
