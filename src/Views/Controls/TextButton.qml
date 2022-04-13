import QtQuick 2.15

Item {
    id: root
    height: 30
    width: titleText.width + 40

    property alias title: titleText.text
    property bool disabled: false
    property bool buttonHovered: false

    signal pressed()

    Rectangle {
        id: background
        color: disabled ? "#d1dade" : (buttonHovered ? "#169c81" : "#1ab394")
        anchors.fill: parent
        radius: 3
    }

    Text {
        id: titleText
        anchors.centerIn: parent
        color: "#fff"
        font.bold: true
        font.pointSize: 10
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onPressed: {
            root.pressed();
        }
        onEntered: {
            root.buttonHovered = true;
        }
        onExited: {
            root.buttonHovered = false;
        }
    }
}
