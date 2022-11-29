import QtQuick
import QtQuick.Controls

Item {
    width: 320
    height: 250

    Rectangle {
        anchors.fill: parent
        color: "white"
        border.color: "black"
        border.width: 1
    }

    ListView {
        id: items
        anchors.fill: parent
        anchors.margins: 4
        model: backend.openApiExporter.addressPalette
        delegate: Item {
            width: items.width
            height: 26

            Rectangle {
                color: "lightgray"
                anchors.fill: parent
                visible: isSelected
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 4
                anchors.right: parent.right
                anchors.rightMargin: 4
                width: parent.width
                height: parent.height
                text: title
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                elide: Text.ElideRight
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }
}
