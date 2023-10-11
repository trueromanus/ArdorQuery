#include <QGuiApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include "globaleventhandlermodel.h"

GlobalEventHandlerModel::GlobalEventHandlerModel(QObject *parent)
    : QObject{parent}
{
    QGuiApplication::instance()->installEventFilter(this);
}

bool GlobalEventHandlerModel::eventFilter(QObject *watched, QEvent *event)
{
    QEvent::Type t = event->type();

    if (!event->spontaneous()) return QObject::eventFilter(watched, event);

    if (t == QEvent::MouseButtonRelease) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        auto button = mouseEvent->button();
        if (button == Qt::XButton1) {
            emit backButtonPressed();
        }
        if (button == Qt::XButton2) {
            emit forwardButtonPressed();
        }

        return QObject::eventFilter(watched, event);
    }

    auto keyEvent = static_cast<QKeyEvent*>(event);

    if (t == QEvent::KeyPress) {
        auto button = keyEvent->key();

        if (button == Qt::Key_Control) m_controlPressed = true;
        if (button == Qt::Key_Shift) m_shiftPressed = true;
        if (button == Qt::Key_Alt) m_altPressed = true;

        if (button == Qt::Key_Minus || button == Qt::Key_Underscore) emit keyButtonPressed(m_minusKey);
        if (button == Qt::Key_Plus || button == Qt::Key_Equal) emit keyButtonPressed(m_plusKey);
        if (button == Qt::Key_F1) emit keyButtonPressed(m_f1Key);
        if (button == Qt::Key_Tab) emit keyButtonPressed(m_tabKey);

    }

    if (t == QEvent::KeyRelease) {
        auto button = keyEvent->key();

        if (button == Qt::Key_Control) m_controlPressed = false;
        if (button == Qt::Key_Shift) m_shiftPressed = false;
        if (button == Qt::Key_Alt) m_altPressed = false;
    }

    return QObject::eventFilter(watched, event);
}
