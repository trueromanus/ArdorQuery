import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 700
    height: 300
    minimumWidth: 700
    minimumHeight: 300
    maximumWidth: 800
    maximumHeight: 600
    modality: Qt.WindowModal
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "Export from OpenAPI"

    RowLayout {
        id: addressLine
        width: parent.width
        height: 40
        spacing: 2

        TextField {
            Layout.fillWidth: true
            text: backend.openApiExporter.url
            onTextChanged: {
                backend.openApiExporter.setUrl(text);
            }
        }

        Button {
            Layout.preferredWidth: 180
            text: backend.openApiExporter.alreadyLoaded ? "Reload scheme" : "Load scheme"
            enabled: backend.openApiExporter.url.length > 0
            onPressed: {
                backend.openApiExporter.loadOpenApiScheme();
            }
        }
    }

    ListView {
        id: routesListView
        anchors.top: addressLine.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        spacing: 2
        clip: true
        model: backend.openApiExporter.routeList
        delegate: Item {
            width: routesListView.width
            height: 40

            Rectangle {
                anchors.fill: parent
                anchors.leftMargin: 2
                anchors.rightMargin: 2
                color: "transparent"
                border.color: methodColor
                border.width: 1
                radius: 8
            }

            Rectangle {
                id: methodText
                anchors.left: parent.left
                anchors.leftMargin: 6
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                width: 70
                radius: 8
                color: methodColor

                Text {
                    anchors.centerIn: parent
                    color: "white"
                    font.pointSize: 10
                    font.bold: true
                    text: method
                }
            }

            Text {
                anchors.left: methodText.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                text: route
                maximumLineCount: 1
                elide: Text.ElideRight
                wrapMode: Text.NoWrap
                font.pointSize: 10
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
            visible: true
        }
    }

    Text {
        anchors.centerIn: parent
        visible: backend.openApiExporter.loading
        text: "Loading..."
    }
}
