import QtQuick /* 2.15 */

Item {
    height: 40

    property alias title: responseHeaderText.text
    default property alias _contentChildren: buttons.data

    Text {
        id: responseHeaderText
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 12
    }

    Item {
        anchors.left: responseHeaderText.right
        anchors.right: parent.right
        height: parent.height

        Item {
            id: buttons
            height: parent.height
            width: 200
            anchors.right: parent.right
        }
    }
}
