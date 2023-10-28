import QtQuick
import QtQuick.Controls

TextField {
    id: root
    selectByMouse: true
    background: Rectangle {
        anchors.fill: parent
        color: root.activeFocus ? "white" : "transparent"
        opacity: .8
        border.width: 1
        border.color: root.activeFocus ? "blue" : "gray"
    }
}
