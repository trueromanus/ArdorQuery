import QtQuick
import QtQuick.Controls

Item {
    width: 280
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
        model: backend.requestsCommandPaletter
        delegate: Item {
            width: items.width
            height: 30

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
                text: title
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                maximumLineCount: 2
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }
}
