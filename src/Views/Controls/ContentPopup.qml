import QtQuick
import QtQuick.Controls

Popup {
    modal: true
    closePolicy: Popup.CloseOnEscape
    focus: true
    x: (applicationWindow.width / 2) - (width / 2)
    y: (applicationWindow.height / 2) - (height / 2) - applicationWindow.header.height
    topPadding: 1
    bottomPadding: 1
    leftPadding: 1
    rightPadding: 1
    Overlay.modal: Rectangle {
        color: Qt.rgba(0,0,0, 0.1)
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
    }
}
