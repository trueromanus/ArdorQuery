import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Controls"

ApplicationWindow {
    id: root
    width: 700
    height: 500
    minimumWidth: 700
    minimumHeight: 400
    maximumWidth: 800
    maximumHeight: 600
    modality: Qt.WindowModal
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "Export from OpenAPI"
    background: Rectangle {
        anchors.fill: parent
        color: "#f2f2f2"
    }
    header: Item {
        id: headerContainer
        height: 30
        width: root.width

        Rectangle {
            opacity: .2
            color: "#f2f2f2"
            anchors.fill: parent
        }

        IconButton {
            id: helpButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            icon: storagePaths.icons + "question.svg"
            width: 30
            height: parent.height
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Show keyboard shortcut description"
            onPressed: {
                openApiShortcutPanel.visible = !openApiShortcutPanel.visible;
            }
        }
    }

    RowLayout {
        id: addressLine
        width: parent.width
        height: 40
        spacing: 2

        Item {
            Layout.preferredWidth: 100
            Layout.fillHeight: true

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
                font.pixelSize: 14
                text: "Scheme URL"
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            CommonTextField {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: parent.left
                focus: true
                text: backend.openApiExporter.url
                onTextChanged: {
                    backend.openApiExporter.setUrl(text);
                }
                Keys.onPressed: (event) => {
                    const needAccepted = backend.openApiExporter.keysHandler(
                        event.key,
                        event.nativeScanCode,
                        (event.modifiers & Qt.ControlModifier),
                        (event.modifiers & Qt.ShiftModifier),
                        (event.modifiers & Qt.AltModifier)
                    );
                    if (needAccepted) event.accepted = true;
                }
                Keys.onReleased: (event) => {
                    backend.openApiExporter.keysReleased(event.key);
                }
            }
        }
    }

    RowLayout {
        id: baseUrlLine
        width: parent.width
        height: 40
        anchors.top: addressLine.bottom
        spacing: 2

        Item {
            Layout.preferredWidth: 100
            Layout.fillHeight: true

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
                font.pixelSize: 14
                text: "Base URL"
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            CommonTextField {
                id: baseUrlTextField
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: parent.left
                text: backend.openApiExporter.baseUrl
                onTextChanged: {
                    backend.openApiExporter.baseUrl = text;
                }
                Keys.onPressed: (event) => {
                    const needAccepted = backend.openApiExporter.keysHandler(
                        event.key,
                        event.nativeScanCode,
                        (event.modifiers & Qt.ControlModifier),
                        (event.modifiers & Qt.ShiftModifier),
                        (event.modifiers & Qt.AltModifier)
                    );
                    if (needAccepted) event.accepted = true;
                }
                Keys.onReleased: (event) => {
                    backend.openApiExporter.keysReleased(event.key);
                }
            }
        }
    }

    RowLayout {
        id: filterLine
        width: parent.width
        height: 40
        anchors.top: baseUrlLine.bottom
        spacing: 2

        Item {
            Layout.preferredWidth: 100
            Layout.fillHeight: true

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
                font.pixelSize: 14
                text: "Filter"
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            CommonTextField {
                id: filterTextField
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: parent.left
                enabled: backend.openApiExporter.routeList
                text: backend.openApiExporter.routeList.filter
                onTextChanged: {
                    backend.openApiExporter.routeList.filter = text;
                }
                Keys.onPressed: (event) => {
                    const needAccepted = backend.openApiExporter.keysHandler(
                        event.key,
                        event.nativeScanCode,
                        (event.modifiers & Qt.ControlModifier),
                        (event.modifiers & Qt.ShiftModifier),
                        (event.modifiers & Qt.AltModifier)
                    );
                    if (needAccepted) event.accepted = true;
                }
                Keys.onReleased: (event) => {
                    backend.openApiExporter.keysReleased(event.key);
                }
            }
        }
    }

    ListView {
        id: routesListView
        anchors.top: filterLine.bottom
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
                color: "white"
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

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        backend.importFromOpenApi(identifier);
                        root.close();
                    }
                }
            }

            Text {
                anchors.left: methodText.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - methodText.width - 30
                height: parent.height
                text: route + " <b>" + description + "</b>"
                verticalAlignment: Text.AlignVCenter
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

    Item {
        id: spinnerContainer
        anchors.fill: parent
        visible: backend.openApiExporter.loading

        Rectangle {
            anchors.fill: parent
            color: "lightgray"
            opacity: .5
        }

        Image {
            id: image
            anchors.centerIn: parent
            source: storagePaths.icons + "spinner.svg"
            width: 40
            height: 40
            mipmap: true
            RotationAnimation on rotation {
                loops: Animation.Infinite
                from: 0
                to: 360
                running: spinnerContainer.visible
                duration: 1500
            }
        }

        MouseArea {
            enabled: spinnerContainer.visible
            propagateComposedEvents: false
            anchors.fill: parent
        }
    }

    ShorcutsHelperPanel {
        id: openApiShortcutPanel
        mode: "openapi"
        visible: backend.openApiExporter.helpVisible
    }
}
