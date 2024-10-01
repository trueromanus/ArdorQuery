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
                clip: true
                delegate: Item {
                    id: subItemRoot
                    width: requestsListView.width
                    height: 30

                    required property string title
                    required property int currentIndex
                    required property bool isSelected
                    required property var resultViewModel

                    Rectangle {
                        anchors.fill: parent
                        border.width: 1
                        border.color: subItemRoot.isSelected ? "blue" : "transparent"
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        anchors.right: parent.right
                        anchors.rightMargin: 30
                        text: subItemRoot.title
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        wrapMode: Text.Wrap
                    }

                    Image {
                        id: image
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        anchors.verticalCenter: parent.verticalCenter
                        width: 22
                        height: 22
                        source: storagePaths.icons + subItemRoot.resultViewModel.displayStatus + ".svg"
                        mipmap: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            backend.requests.selectItem(subItemRoot.currentIndex);
                        }
                    }
                }
            }
        }
    }
}
