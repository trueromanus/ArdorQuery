#include <QGuiApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include "globaleventhandlermodel.h"

GlobalEventHandlerModel::GlobalEventHandlerModel(QObject *parent)
    : QObject{parent}
{
    QGuiApplication::instance()->installEventFilter(this);

    m_keyMapping.insert(Qt::Key_Control, m_control);
    m_keyMapping.insert(Qt::Key_Shift, m_shift);
    m_keyMapping.insert(Qt::Key_Alt, m_alt);

    m_keyMapping.insert(Qt::Key_Minus, m_minusKey);
    m_keyMapping.insert(Qt::Key_Underscore, m_minusKey);
    m_keyMapping.insert(Qt::Key_Plus, m_plusKey);
    m_keyMapping.insert(Qt::Key_Equal, m_plusKey);

    m_keyMapping.insert(Qt::Key_F1, m_f1Key);
    m_keyMapping.insert(Qt::Key_F2, m_f2Key);
    m_keyMapping.insert(Qt::Key_F3, m_f3Key);
    m_keyMapping.insert(Qt::Key_F4, m_f4Key);
    m_keyMapping.insert(Qt::Key_F5, m_f5Key);
    m_keyMapping.insert(Qt::Key_F6, m_f6Key);
    m_keyMapping.insert(Qt::Key_F7, m_f7Key);
    m_keyMapping.insert(Qt::Key_F8, m_f8Key);
    m_keyMapping.insert(Qt::Key_F9, m_f9Key);
    m_keyMapping.insert(Qt::Key_F10, m_f10Key);
    m_keyMapping.insert(Qt::Key_F11, m_f11Key);
    m_keyMapping.insert(Qt::Key_F12, m_f12Key);
    m_keyMapping.insert(Qt::Key_Tab, m_tabKey);
    m_keyMapping.insert(Qt::Key_Escape, m_escapeKey);

    m_keyMapping.insert(Qt::Key_A, m_aKey);
    m_keyMapping.insert(Qt::Key_B, m_bKey);
    m_keyMapping.insert(Qt::Key_C, m_cKey);

    m_mouseKeyMapping.insert(Qt::XButton1, m_mouseXButton1);
    m_mouseKeyMapping.insert(Qt::XButton2, m_mouseXButton2);
}

bool GlobalEventHandlerModel::eventFilter(QObject *watched, QEvent *event)
{
    QEvent::Type t = event->type();

    if (!event->spontaneous()) return QObject::eventFilter(watched, event);

    if (t == QEvent::MouseButtonRelease || t == QEvent::MouseButtonPress) {
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        auto button = mouseEvent->button();

        auto keyName = m_mouseKeyMapping.value(button);

        auto countKeys = m_pressedMouseKeys.count();

        if (t == QEvent::MouseButtonPress && !m_pressedMouseKeys.contains(keyName)) m_pressedMouseKeys.insert(keyName);
        if (t == QEvent::MouseButtonRelease && m_pressedMouseKeys.contains(keyName)) m_pressedMouseKeys.remove(keyName);

        if (m_pressedMouseKeys.count() != countKeys) emit mouseChanged(pressedMouseKeysToString());

        return QObject::eventFilter(watched, event);
    }

    if (t == QEvent::KeyPress || t == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent*>(event);
        auto button = keyEvent->key();
        auto virtualCode = keyEvent->nativeVirtualKey();

        if (virtualCode >= 41 && virtualCode <= 90) button = virtualCode; // fix issue with keyboard layout

        if (!m_keyMapping.contains(button)) return QObject::eventFilter(watched, event);

        auto keyName = m_keyMapping.value(button);
        auto countKeys = m_pressedKeys.count();

        if (t == QEvent::KeyPress && !m_pressedKeys.contains(keyName)) m_pressedKeys.insert(keyName);
        if (t == QEvent::KeyRelease && m_pressedKeys.contains(keyName)) m_pressedKeys.remove(keyName);

        auto isChanged = countKeys != m_pressedKeys.count();
        if (isChanged) emit keysChanged(pressedKeysToString());
    }

    return QObject::eventFilter(watched, event);
}

QString GlobalEventHandlerModel::pressedKeysToString()
{
    QStringList result;
    if (m_pressedKeys.contains(m_control)) result.append(m_control);
    if (m_pressedKeys.contains(m_shift)) result.append(m_shift);
    if (m_pressedKeys.contains(m_alt)) result.append(m_alt);

    if (m_pressedKeys.contains(m_f1Key)) result.append(m_f1Key);
    if (m_pressedKeys.contains(m_f2Key)) result.append(m_f2Key);
    if (m_pressedKeys.contains(m_f3Key)) result.append(m_f3Key);
    if (m_pressedKeys.contains(m_f4Key)) result.append(m_f4Key);
    if (m_pressedKeys.contains(m_f5Key)) result.append(m_f5Key);
    if (m_pressedKeys.contains(m_f6Key)) result.append(m_f6Key);
    if (m_pressedKeys.contains(m_f7Key)) result.append(m_f7Key);
    if (m_pressedKeys.contains(m_f8Key)) result.append(m_f8Key);
    if (m_pressedKeys.contains(m_f9Key)) result.append(m_f9Key);
    if (m_pressedKeys.contains(m_f10Key)) result.append(m_f10Key);
    if (m_pressedKeys.contains(m_f11Key)) result.append(m_f11Key);
    if (m_pressedKeys.contains(m_f12Key)) result.append(m_f12Key);
    if (m_pressedKeys.contains(m_escapeKey)) result.append(m_escapeKey);

    if (m_pressedKeys.contains(m_plusKey)) result.append(m_plusKey);
    if (m_pressedKeys.contains(m_minusKey)) result.append(m_minusKey);

    if (m_pressedKeys.contains(m_aKey)) result.append(m_aKey);
    if (m_pressedKeys.contains(m_bKey)) result.append(m_bKey);
    if (m_pressedKeys.contains(m_cKey)) result.append(m_cKey);

    return result.join("-");
}

QString GlobalEventHandlerModel::pressedMouseKeysToString()
{
    QStringList result;
    if (m_pressedMouseKeys.contains(m_mouseXButton1)) result.append(m_mouseXButton1);
    if (m_pressedMouseKeys.contains(m_mouseXButton2)) result.append(m_mouseXButton2);

    return result.join("-");
}
