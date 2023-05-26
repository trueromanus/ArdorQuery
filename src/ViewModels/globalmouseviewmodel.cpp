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
    QEvent::Type t = event->type();
    if (t == QEvent::MouseMove && m_moveTracking && event->spontaneous()) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        auto position = mouseEvent->position();
        m_xCoordinate = position.x();
        m_yCoordinate = position.y();
        if (m_xCoordinate >= m_leftEdge && m_yCoordinate >= m_topEdge) {
            emit mouseMoved(m_xCoordinate, m_yCoordinate);
        }
    }

    return QObject::eventFilter(watched, event);
}

void GlobalMouseViewModel::setLeftEdge(int leftEdge) noexcept
{
    if (m_leftEdge == leftEdge) return;

    m_leftEdge = leftEdge;
    emit leftEdgeChanged();
}

void GlobalMouseViewModel::setTopEdge(int topEdge) noexcept
{
    if (m_topEdge == topEdge) return;

    m_topEdge = topEdge;
    emit topEdgeChanged();
}

void GlobalMouseViewModel::setMoveTracking(bool moveTracking) noexcept
{
    if (m_moveTracking == moveTracking) return;

    m_moveTracking = moveTracking;
    emit moveTrackingChanged();
}
