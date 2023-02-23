import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    anchors.centerIn: parent
    width: 480
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
                id: titleText
                anchors.top: parent.top
                font.pointSize: 11
                text: "Select body"
            }

            ListView {
                id: requestsListView
                anchors.top: titleText.bottom
                anchors.topMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 4
                anchors.left: parent.left
                anchors.bottomMargin: 4
                anchors.bottom: parent.bottom
                model: backend.openApiExporter.bodyTypes
                clip: true
                delegate: Item {
                    width: requestsListView.width
                    height: 30

                    Rectangle {
                        anchors.fill: parent
                        color: "white"
                        opacity: .4
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                        anchors.topMargin: 2
                        anchors.bottomMargin: 2
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 4
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        text: modelData
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        wrapMode: Text.Wrap
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            backend.openApiExporter.prepareBodyType = modelData;
                            backend.importFromOpenApi(backend.openApiExporter.prepareIdentifier);
                            openApiExportWindow.item.close();
                        }
                    }
                }
            }
        }
    }
}
