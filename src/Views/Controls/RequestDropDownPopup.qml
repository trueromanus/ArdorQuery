import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */
import QtQuick.Layouts /* 1.15 */

Popup {
    id: root
    width: 500
    height: 300
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    modal: true

    RowLayout {
        anchors.fill: parent

        Item {
            id: requestsContainer
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: titleRequests
                anchors.top: parent.top
                font.pointSize: 11
                text: "Queries"
            }

            ListView {
                id: requestsListView
                anchors.top: titleRequests.bottom
                anchors.topMargin: 9
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.left: parent.left
                anchors.bottomMargin: 4
                anchors.bottom: parent.bottom
                model: backend.requests
                delegate: Item {
                    width: requestsListView.width
                    height: 30

                    Rectangle {
                        anchors.fill: parent
                        border.width: 1
                        border.color: isSelected ? "blue" : "transparent"
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        text: title
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        wrapMode: Text.Wrap
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            backend.requests.selectItem(currentIndex);
                        }
                    }
                }
            }

            Rectangle {
                anchors.right: parent.right
                anchors.rightMargin: 9
                color: "black"
                width: 1
                height: parent.height
            }
        }

        Item {
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            Text {
                id: titleChangeName
                anchors.top: parent.top
                font.pointSize: 11
                text: "Change name"
            }

            Item {
                anchors.top: titleChangeName.bottom
                width: parent.width
                height: 50

                RowLayout {
                    spacing: 0
                    anchors.fill: parent

                    TextField {
                        id: changeNameField
                        Layout.fillWidth: true
                        text: backend.requests.selectedItem.title
                    }

                    IconButton {
                        id: saveButton
                        Layout.preferredWidth: 29
                        iconWidth: 20
                        iconHeight: 20
                        width: 22
                        height: 22
                        icon: storagePaths.icons + "save.svg"
                        onPressed: {
                            backend.requests.changeNameForSelectedItem(changeNameField.text);
                        }
                    }
                }
            }

            TextButton {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                title: "Add new query"
                onPressed: {
                    backend.addNewRequest("New Query");
                }
            }
        }
    }
}
