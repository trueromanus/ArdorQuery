import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Controls"

ApplicationWindow {
    id: root
    width: 700
    height: 600
    minimumWidth: 700
    minimumHeight: 400
    maximumWidth: 800
    maximumHeight: 800
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "Import from OpenAPI"
    background: Rectangle {
        anchors.fill: parent
        color: "white"
    }
    header: Item {
        id: headerContainer
        height: 30
        width: root.width

        OpenApiWindowHeader {
            id: windowHeader
            anchors.left: parent.left
            anchors.top: parent.top
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

    Item {
        anchors.fill: parent
        visible: backend.openApiExporter.exporterPage

        Rectangle {
            anchors.fill: parent
            color: "#f2f2f2"
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
                }
            }
        }

        RowLayout {
            id: titleLine
            width: parent.width
            height: 40
            anchors.top: filterLine.bottom
            spacing: 2

            Item {
                Layout.preferredWidth: 100
                Layout.fillHeight: true

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    font.pixelSize: 14
                    text: "Title"
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                CommonTextField {
                    id: titleTextField
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.left: parent.left
                    text: backend.openApiExporter.title
                    onTextChanged: {
                        backend.openApiExporter.title = text;
                    }
                }
            }
        }

        RowLayout {
            id: authLine
            width: parent.width
            height: 40
            anchors.top: titleLine.bottom
            spacing: 2

            Item {
                Layout.preferredWidth: 100
                Layout.fillHeight: true

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    font.pixelSize: 14
                    text: "Securities"
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                CommonTextField {
                    id: authTextField
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.left: parent.left
                    text: backend.openApiExporter.authMethod
                    onTextChanged: {
                        backend.openApiExporter.authMethod = text;
                    }
                }
            }
        }

        ListView {
            id: routesListView
            anchors.top: authLine.bottom
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

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (backend.openApiExporter.isHasFewBodies(identifier)) {
                            backend.openApiExporter.prepareIdentifier = identifier;
                            bodyTypesPopup.open();

                        } else {
                            backend.importFromOpenApi(identifier);
                            root.close();
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {
                active: true
                visible: true
            }
        }

        Item {
            visible: !backend.openApiExporter.routeList.hasItems
            width: routesListView.width
            height: routesListView.height
            anchors.top: routesListView.top

            Item {
                anchors.centerIn: parent
                width: 200
                height: routesEmptyBoxImage.height + routesEmptyText.height

                Image {
                    id: routesEmptyBoxImage
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 60
                    height: 60
                    mipmap: true
                    source: storagePaths.icons + "emptybox.svg"
                }
                Text {
                    id: routesEmptyText
                    anchors.top: routesEmptyBoxImage.bottom
                    anchors.horizontalCenter: routesEmptyBoxImage.horizontalCenter
                    font.pointSize: 10
                    text: "Routes not loaded yet"
                }
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
    }

    Item {
        anchors.fill: parent
        visible: backend.openApiExporter.savedOptionsPage

        Rectangle {
            anchors.fill: parent
            color: "#f2f2f2"
        }

        ListView {
            id: optionsListView
            anchors.fill: parent
            model: backend.openApiExporter.addresses
            delegate: Item {
                width: optionsListView.width
                height: 60

                Rectangle {
                    color: "#CDCDB4"
                    radius: 4
                    opacity: .5
                    anchors.fill: parent
                    anchors.margins: 2
                }

                Text {
                    id: titleText
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.top: parent.top
                    anchors.topMargin: 6
                    width: parent.width - 10
                    height: 15
                    text: title
                    font.pointSize: 10.5
                    maximumLineCount: 1
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                }
                Text {
                    id: routeText
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.top: titleText.bottom
                    width: parent.width - 10
                    height: 15
                    text: route
                    font.pointSize: 9
                    maximumLineCount: 1
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                }
                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.top: routeText.bottom
                    width: parent.width - 10
                    height: 15
                    text: baseRoute
                    font.pointSize: 9
                    maximumLineCount: 1
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (backend.openApiExporter.loading) return;

                        backend.openApiExporter.addressPalette.selectItemByAddressIndex(identifier);
                        backend.openApiExporter.togglePages();
                    }
                }

                IconButton {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    width: 24
                    height: 24
                    icon: storagePaths.icons + "delete.svg"
                    iconWidth: 20
                    iconHeight: 20
                    tooltipMessage: "Delete saved option"
                    onPressed: {
                        backend.openApiExporter.addresses.deleteItem(identifier);
                    }
                }
            }
        }

        Item {
            visible: !backend.openApiExporter.addresses.hasItems
            anchors.fill: parent

            Image {
                id: emptyBoxImage
                anchors.centerIn: parent
                width: 60
                height: 60
                source: storagePaths.icons + "emptybox.svg"
            }
            Text {
                anchors.top: emptyBoxImage.bottom
                anchors.horizontalCenter: emptyBoxImage.horizontalCenter
                font.pointSize: 10
                text: "You don't have any saved options yet"
            }
        }
    }

    AddressesPalette {
        id: addressesPalette
        anchors.centerIn: parent
        visible: backend.openApiExporter.openedCommandPalette
        enabled: backend.openApiExporter.openedCommandPalette
    }

    ShorcutsHelperPanel {
        id: openApiShortcutPanel
        visible: backend.openApiExporter.helpVisible
        shortcuts: backend.openApiExporter.shortcuts
    }

    BodyTypesPopup {
        id: bodyTypesPopup
    }

    Rectangle {
        id: notificationContainer
        visible: backend.openApiExporter.errorMessage.length
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.rightMargin: 4
        width: 250
        height: 70
        border.width: 1
        border.color: "lightgray"
        radius: 4

        Text {
            anchors.fill: parent
            anchors.margins: 4
            text: backend.openApiExporter.errorMessage
        }

        IconButton {
            id: closeButton
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.rightMargin: 2
            icon: storagePaths.icons + "cancel.svg"
            width: 22
            height: 22
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Close notification"
            onPressed: {
                backend.openApiExporter.clearErrorMessage();
            }
        }
    }

    Connections {
        target: globalEventHandler
        function onKeysChanged (state) {
            if (!root.activeFocusItem) return;

            const handled = backend.openApiExporter.shortcutHandler(state);
            if (handled) globalEventHandler.setHandledLastSession();
        }
    }

    onActiveFocusItemChanged: {
        if (!root.activeFocusItem) globalEventHandler.clear();
    }
}
