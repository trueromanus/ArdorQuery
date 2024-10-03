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
        model: backend.requestsCommandPaletter
        clip: true
        delegate: Item {
            id: itemRoot
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
                anchors.rightMargin: 36
                text: title
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                maximumLineCount: 2
            }

            Image {
                id: image
                anchors.right: parent.right
                anchors.rightMargin: 14
                anchors.verticalCenter: parent.verticalCenter
                width: 22
                height: 22
                source: storagePaths.icons + displayStatus + ".svg"
                mipmap: true
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }

    Connections {
        target: backend.requestsCommandPaletter
        function onItemIndexSelected(id: int) {
            items.positionViewAtIndex(id, ListView.Contain);
        }
    }
}
