import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */

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
            model: backend.tabs
            delegate: Item {
                id: itemRoot
                width: 200
                height: root.height
                property bool itemHovered: false

                Item {
                    clip: true
                    anchors.fill: parent

                    Rectangle {
                        color: isActived ? "white" : "transparent"
                        width: parent.width
                        height: parent.height + 4
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                        border.width: 1
                        border.color: isActived ? "#e7eaec" : (itemRoot.itemHovered ? "#e7eaec" : "transparent")
                        radius: 4
                    }
                }

                Text {
                    anchors.right: isRequests ? dropMenuButton.left : parent.right
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    height: parent.height
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 11
                    elide: Text.ElideRight
                    wrapMode: Text.Wrap
                    maximumLineCount: 1
                    text: isRequests ? backend.requests.selectedItem.requestModel.title : title
                    color: isActived || itemRoot.itemHovered ? "black" : "gray"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: {
                        if (isActived) return;

                        backend.tabs.activateTab(identifier);
                    }
                    onEntered: {
                        itemRoot.itemHovered = true;
                    }
                    onExited: {
                        itemRoot.itemHovered = false;
                    }
                }

                IconButton {
                    id: pendingButton
                    visible: isResult
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 24
                    height: 24
                    iconWidth: 18
                    iconHeight: 18
                    icon: storagePaths.icons + backend.requests.selectedItem.resultModel.displayStatus + ".svg"
                    onPressed: {
                    }
                }

                IconButton {
                    id: dropMenuButton
                    visible: isRequests
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 24
                    height: 24
                    iconWidth: 15
                    iconHeight: 15
                    icon: storagePaths.icons + "downmenu.svg"
                    onPressed: {
                        requestsPopup.open();
                    }
                }
            }
        }
    }

    IconButton {
        id: helpButton
        anchors.right: infoButton.left
        anchors.verticalCenter: parent.verticalCenter
        icon: storagePaths.icons + "question.svg"
        width: 30
        height: parent.height
        iconWidth: 20
        iconHeight: 20
        tooltipMessage: "Show keyboard shortcut description"
        onPressed: {
            backend.helpVisible = !backend.helpVisible;
        }
    }

    IconButton {
        id: infoButton
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        icon: storagePaths.icons + "info.svg"
        width: 30
        height: parent.height
        iconWidth: 20
        iconHeight: 20
        tooltipMessage: "About"
        onPressed: {
            aboutWindow.show();
        }
    }

    RequestDropDownPopup {
        id: requestsPopup
        x: 10
        y: 30
    }
}
