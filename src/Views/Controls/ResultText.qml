import QtQuick 2.15

Item {
    height: 20
    property alias title: innerText.text

    Text {
        id: innerText
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 9
    }
}

