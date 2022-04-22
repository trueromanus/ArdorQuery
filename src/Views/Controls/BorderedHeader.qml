import QtQuick 2.15

Item {
    height: 40

    property alias title: responseHeaderText.text

    Text {
        id: responseHeaderText
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 12
    }
}
