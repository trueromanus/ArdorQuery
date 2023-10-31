#include <QGuiApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include "globaleventhandlermodel.h"

GlobalEventHandlerModel::GlobalEventHandlerModel(QObject *parent)
    : QObject{parent}
{
    QGuiApplication::instance()->installEventFilter(this);

    m_keyMapping.insert(Qt::Key_Control, m_control);
    m_keyOrder.insert(m_control, 0);
    m_keyMapping.insert(Qt::Key_Shift, m_shift);
    m_keyOrder.insert(m_shift, 1);
    m_keyMapping.insert(Qt::Key_Alt, m_alt);
    m_keyOrder.insert(m_alt, 2);

    m_keyMapping.insert(Qt::Key_Minus, m_minusKey);
    m_keyMapping.insert(Qt::Key_Underscore, m_minusKey);
    m_keyOrder.insert(m_minusKey, 3);
    m_keyMapping.insert(Qt::Key_Plus, m_plusKey);
    m_keyMapping.insert(Qt::Key_Equal, m_plusKey);
    m_keyOrder.insert(m_plusKey, 4);

    m_keyMapping.insert(Qt::Key_F1, m_f1Key);
    m_keyOrder.insert(m_f1Key, 5);
    m_keyMapping.insert(Qt::Key_F2, m_f2Key);
    m_keyOrder.insert(m_f2Key, 6);
    m_keyMapping.insert(Qt::Key_F3, m_f3Key);
    m_keyOrder.insert(m_f3Key, 7);
    m_keyMapping.insert(Qt::Key_F4, m_f4Key);
    m_keyOrder.insert(m_f4Key, 8);
    m_keyMapping.insert(Qt::Key_F5, m_f5Key);
    m_keyOrder.insert(m_f5Key, 9);
    m_keyMapping.insert(Qt::Key_F6, m_f6Key);
    m_keyOrder.insert(m_f6Key, 10);
    m_keyMapping.insert(Qt::Key_F7, m_f7Key);
    m_keyOrder.insert(m_f7Key, 11);
    m_keyMapping.insert(Qt::Key_F8, m_f8Key);
    m_keyOrder.insert(m_f8Key, 12);
    m_keyMapping.insert(Qt::Key_F9, m_f9Key);
    m_keyOrder.insert(m_f9Key, 13);
    m_keyMapping.insert(Qt::Key_F10, m_f10Key);
    m_keyOrder.insert(m_f10Key, 14);
    m_keyMapping.insert(Qt::Key_F11, m_f11Key);
    m_keyOrder.insert(m_f11Key, 15);
    m_keyMapping.insert(Qt::Key_F12, m_f12Key);
    m_keyOrder.insert(m_f12Key, 16);
    m_keyMapping.insert(Qt::Key_Tab, m_tabKey);
    m_keyOrder.insert(m_tabKey, 17);
    m_keyMapping.insert(Qt::Key_Escape, m_escapeKey);
    m_keyOrder.insert(m_escapeKey, 18);
    m_keyMapping.insert(Qt::Key_Home, m_homeKey);
    m_keyOrder.insert(m_homeKey, 19);
    m_keyMapping.insert(Qt::Key_Insert, m_insertKey);
    m_keyOrder.insert(m_insertKey, 20);
    m_keyMapping.insert(Qt::Key_End, m_endKey);
    m_keyOrder.insert(m_endKey, 21);
    m_keyMapping.insert(Qt::Key_Delete, m_deleteKey);
    m_keyOrder.insert(m_deleteKey, 22);

    m_keyMapping.insert(Qt::Key_BracketLeft, m_leftBracket);
    m_keyMapping.insert(Qt::Key_BraceLeft, m_leftBracket);
    m_keyMapping.insert(219, m_leftBracket);
    m_keyOrder.insert(m_leftBracket, 23);
    m_keyMapping.insert(Qt::Key_BracketRight, m_rightBracket);
    m_keyMapping.insert(Qt::Key_BraceRight, m_rightBracket);
    m_keyMapping.insert(221, m_rightBracket);
    m_keyOrder.insert(m_rightBracket, 24);

    m_keyMapping.insert(Qt::Key_Enter, m_enter);
    m_keyMapping.insert(Qt::Key_Return, m_enter);
    m_keyOrder.insert(m_enter, 25);

    m_keyMapping.insert(Qt::Key_Backspace, m_backspace);
    m_keyOrder.insert(m_backspace, 26);

    m_keyMapping.insert(Qt::Key_PageDown, m_pageDown);
    m_keyOrder.insert(m_pageDown, 50);
    m_keyMapping.insert(Qt::Key_PageUp, m_pageUp);
    m_keyOrder.insert(m_pageUp, 51);

    m_keyMapping.insert(Qt::Key_0, m_zeroKey);
    m_keyOrder.insert(m_zeroKey, 52);
    m_keyMapping.insert(Qt::Key_1, m_oneKey);
    m_keyOrder.insert(m_oneKey, 53);
    m_keyMapping.insert(Qt::Key_2, m_twoKey);
    m_keyOrder.insert(m_twoKey, 54);
    m_keyMapping.insert(Qt::Key_3, m_threeKey);
    m_keyOrder.insert(m_threeKey, 55);
    m_keyMapping.insert(Qt::Key_4, m_fooKey);
    m_keyOrder.insert(m_fooKey, 56);
    m_keyMapping.insert(Qt::Key_5, m_fiveKey);
    m_keyOrder.insert(m_fiveKey, 57);
    m_keyMapping.insert(Qt::Key_6, m_sixKey);
    m_keyOrder.insert(m_sixKey, 58);
    m_keyMapping.insert(Qt::Key_7, m_sevenKey);
    m_keyOrder.insert(m_sevenKey, 59);
    m_keyMapping.insert(Qt::Key_8, m_eightKey);
    m_keyOrder.insert(m_eightKey, 60);
    m_keyMapping.insert(Qt::Key_9, m_nineKey);
    m_keyOrder.insert(m_nineKey, 61);

    m_keyMapping.insert(Qt::Key_Down, m_down);
    m_keyOrder.insert(m_down, 62);
    m_keyMapping.insert(Qt::Key_Up, m_up);
    m_keyOrder.insert(m_up, 63);

    m_keyMapping.insert(190, m_dot);
    m_keyMapping.insert(Qt::Key_Period, m_dot);
    m_keyOrder.insert(m_down, 64);
    m_keyMapping.insert(188, m_comma);
    m_keyMapping.insert(Qt::Key_Comma, m_comma);
    m_keyOrder.insert(m_up, 65);

    m_keyMapping.insert(Qt::Key_A, m_aKey);
    m_keyOrder.insert(m_aKey, 100);
    m_keyMapping.insert(Qt::Key_B, m_bKey);
    m_keyOrder.insert(m_bKey, 101);
    m_keyMapping.insert(Qt::Key_C, m_cKey);
    m_keyOrder.insert(m_cKey, 102);
    m_keyMapping.insert(Qt::Key_D, m_dKey);
    m_keyOrder.insert(m_dKey, 103);
    m_keyMapping.insert(Qt::Key_E, m_eKey);
    m_keyOrder.insert(m_eKey, 104);
    m_keyMapping.insert(Qt::Key_F, m_fKey);
    m_keyOrder.insert(m_fKey, 105);
    m_keyMapping.insert(Qt::Key_G, m_gKey);
    m_keyOrder.insert(m_gKey, 106);
    m_keyMapping.insert(Qt::Key_H, m_hKey);
    m_keyOrder.insert(m_hKey, 107);
    m_keyMapping.insert(Qt::Key_I, m_iKey);
    m_keyOrder.insert(m_iKey, 108);
    m_keyMapping.insert(Qt::Key_J, m_jKey);
    m_keyOrder.insert(m_jKey, 109);
    m_keyMapping.insert(Qt::Key_K, m_kKey);
    m_keyOrder.insert(m_kKey, 110);
    m_keyMapping.insert(Qt::Key_L, m_lKey);
    m_keyOrder.insert(m_lKey, 111);
    m_keyMapping.insert(Qt::Key_M, m_mKey);
    m_keyOrder.insert(m_mKey, 112);
    m_keyMapping.insert(Qt::Key_N, m_nKey);
    m_keyOrder.insert(m_nKey, 113);
    m_keyMapping.insert(Qt::Key_O, m_oKey);
    m_keyOrder.insert(m_oKey, 114);
    m_keyMapping.insert(Qt::Key_P, m_pKey);
    m_keyOrder.insert(m_pKey, 115);
    m_keyMapping.insert(Qt::Key_Q, m_qKey);
    m_keyOrder.insert(m_qKey, 116);
    m_keyMapping.insert(Qt::Key_R, m_rKey);
    m_keyOrder.insert(m_rKey, 117);
    m_keyMapping.insert(Qt::Key_S, m_sKey);
    m_keyOrder.insert(m_sKey, 118);
    m_keyMapping.insert(Qt::Key_T, m_tKey);
    m_keyOrder.insert(m_tKey, 119);
    m_keyMapping.insert(Qt::Key_U, m_uKey);
    m_keyOrder.insert(m_uKey, 120);
    m_keyMapping.insert(Qt::Key_V, m_vKey);
    m_keyOrder.insert(m_vKey, 121);
    m_keyMapping.insert(Qt::Key_W, m_wKey);
    m_keyOrder.insert(m_wKey, 122);
    m_keyMapping.insert(Qt::Key_X, m_xKey);
    m_keyOrder.insert(m_xKey, 123);
    m_keyMapping.insert(Qt::Key_Y, m_yKey);
    m_keyOrder.insert(m_yKey, 124);
    m_keyMapping.insert(Qt::Key_Z, m_zKey);
    m_keyOrder.insert(m_zKey, 125);

    m_mouseKeyMapping.insert(Qt::XButton1, m_mouseXButton1);
    m_mouseKeyMapping.insert(Qt::XButton2, m_mouseXButton2);

    m_remappingVirtualCodes.insert(188); // comma
    m_remappingVirtualCodes.insert(190); // period
    m_remappingVirtualCodes.insert(219); // [{
    m_remappingVirtualCodes.insert(221); // }]
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

        // fix issue with keyboard layout
        if (virtualCode >= 65 && virtualCode <= 90) button = virtualCode;
        if (m_remappingVirtualCodes.contains(virtualCode)) button = virtualCode;

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

void GlobalEventHandlerModel::clear()
{
    m_pressedKeys.clear();
    m_pressedMouseKeys.clear();
}

QString GlobalEventHandlerModel::pressedKeysToString()
{
    auto orders = m_keyOrder;
    QStringList keys = m_pressedKeys.values();
    std::sort(
        keys.begin(),
        keys.end(),
        [orders](const QString& left, const QString& right){
            return orders[left] < orders[right];
        }
    );

    return keys.join("-");
}

QString GlobalEventHandlerModel::pressedMouseKeysToString()
{
    QStringList result;
    if (m_pressedMouseKeys.contains(m_mouseXButton1)) result.append(m_mouseXButton1);
    if (m_pressedMouseKeys.contains(m_mouseXButton2)) result.append(m_mouseXButton2);

    return result.join("-");
}
