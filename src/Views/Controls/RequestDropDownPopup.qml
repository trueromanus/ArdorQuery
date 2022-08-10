import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */
import QtQuick.Layouts /* 1.15 */

Popup {
    id: root
    width: 400
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
        }
    }
}
