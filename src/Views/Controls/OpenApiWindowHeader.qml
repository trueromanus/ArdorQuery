import QtQuick
import QtQuick.Controls

Item {
    id: root
    height: 30
    width: parent.width

    Rectangle {
        opacity: .2
        color: "#f2f2f2"
        anchors.fill: parent
    }

    Row {
        id: tabsRow
        anchors.fill: parent
        anchors.leftMargin: 8
        spacing: 2

        Repeater {
            model: backend.openApiExporter.tabs
            delegate: Item {
                id: itemRoot
                width: 200
                height: root.height
                property bool itemHovered: false

                Item {
                    clip: true
                    anchors.fill: parent

                    Rectangle {
                        color: modelData === backend.openApiExporter.selectedTab ? "white" : "transparent"
                        width: parent.width
                        height: parent.height + 4
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                        border.width: 1
                        border.color: modelData === backend.openApiExporter.selectedTab ? "#e7eaec" : (itemRoot.itemHovered ? "#e7eaec" : "transparent")
                        radius: 4
                    }
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    height: parent.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 11
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                    text: modelData
                    color: modelData === backend.openApiExporter.selectedTab || itemRoot.itemHovered ? "black" : "gray"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: {
                        if (modelData === backend.openApiExporter.selectedTab) return;

                        backend.openApiExporter.selectedTab = modelData;
                    }
                    onEntered: {
                        itemRoot.itemHovered = true;
                    }
                    onExited: {
                        itemRoot.itemHovered = false;
                    }
                }
            }
        }
    }
}

