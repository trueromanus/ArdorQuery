import QtQuick /* 2.15 */
import QtQuick.Layouts /* 1.15 */
import QtQuick.Controls /* 2.15 */
import ArdorQuery.Backend /* 1.0 */
import "Controls"

Item {
    id: root
    anchors.fill: parent

    property var viewModel

    signal refreshFocus()

    onRefreshFocus: {
        searchTextField.forceActiveFocus();
    }

    ColumnLayout {
        id: fieldContainer
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            BorderedPanel {
                id: responsePanel
                anchors.fill: parent
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.bottomMargin: 4

                Image {
                    anchors.top: panelsContainer.top
                    anchors.topMargin: 40
                    anchors.left: panelsContainer.left
                    anchors.right: panelsContainer.right
                    height: headersDivider.y - 40
                    fillMode: Image.Tile
                    source: storagePaths.images + "backgroundpattern.png"
                    opacity: .2
                }

                Column {
                    id: panelsContainer
                    BorderedHeader {
                        width: responsePanel.width
                        title: "Response data"
                    }

                    HorizontalDivider {
                        width: responsePanel.width
                    }

                    Item {
                        width: 10
                        height: 8
                    }

                    ResultText {
                        id: statusCodeText
                        width: responsePanel.width - 10
                        title: "<b>Status:</b> " + viewModel.displayStatusCode + " " + viewModel.networkError
                        textContent.wrapMode: Text.NoWrap
                        textContent.elide: Text.ElideRight
                        textContent.maximumLineCount: 1
                        textContent.width: statusCodeText.width
                        textContent.height: statusCodeText.height
                        ToolTip.delay: 1000
                        ToolTip.text: viewModel.networkError
                        ToolTip.visible: itemHovered && viewModel.networkError

                        property bool itemHovered: false

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                parent.itemHovered = true;
                            }
                            onExited: {
                                parent.itemHovered = false;
                            }
                        }
                    }

                    ResultText {
                        id: responseTime
                        width: responsePanel.width
                        title: "<b>Response time:</b> " + viewModel.responseTime
                    }

                    ResultText {
                        id: responseSize
                        width: responsePanel.width
                        title: "<b>Response size:</b> " + viewModel.responseReadableSize + " " + viewModel.responseSize
                    }

                    Item {
                        width: 10
                        height: 8
                    }

                    HorizontalDivider {
                        id: headersDivider
                        width: responsePanel.width
                    }

                    BorderedHeader {
                        width: responsePanel.width
                        title: "Headers"

                        IconButton {
                            anchors.right: parent.right
                            anchors.rightMargin: 2
                            anchors.verticalCenter: parent.verticalCenter
                            width: 24
                            height: 24
                            iconWidth: 22
                            iconHeight: 22
                            icon: storagePaths.icons + "copy.svg"
                            tooltipMessage: "Copy all headers to clipboard"
                            onPressed: {
                                viewModel.copyHeadersToClipboard();
                            }
                        }
                    }

                    HorizontalDivider {
                        width: responsePanel.width
                    }

                    Item {
                        id: headersContainer
                        height: 180
                        width: responsePanel.width

                        Image {
                            anchors.fill: parent
                            fillMode: Image.Tile
                            source: storagePaths.images + "backgroundpattern.png"
                            opacity: .2
                        }

                        ListView {
                            clip: true
                            anchors.fill: parent
                            model: viewModel.headers
                            boundsBehavior: ListView.StopAtBounds
                            delegate: TextArea {
                                readOnly: true
                                leftPadding: 4
                                selectByMouse: true
                                rightPadding: 10
                                topPadding: 0
                                bottomPadding: 0
                                width: headersContainer.width
                                textFormat: TextArea.RichText
                                wrapMode: Text.Wrap
                                text: modelData
                                font.pointSize: 9
                                background: Rectangle {
                                    anchors.fill: parent
                                    color: "transparent"
                                    border.width: 0
                                }
                            }
                            ScrollBar.vertical: ScrollBar {
                                active: true
                            }
                        }
                    }

                    HorizontalDivider {
                        width: responsePanel.width
                    }

                    BorderedHeader {
                        width: responsePanel.width
                        title: "Body"

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: searchTextField.left
                            anchors.rightMargin: 4
                            text: viewModel.bodyModel.countFindedLinesText
                        }

                        TextField {
                            id: searchTextField
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: copyBodyButton.left
                            anchors.rightMargin: 10
                            width: 150
                            selectByMouse: true
                            onEditingFinished: {
                                viewModel.bodyModel.searchText(text);
                                backend.refreshFindedIndex();
                            }
                            Keys.onPressed: (event) => {
                                const needAccepted = backend.keysHandler(
                                    event.key,
                                    event.nativeScanCode,
                                    (event.modifiers & Qt.ControlModifier),
                                    (event.modifiers & Qt.ShiftModifier),
                                    (event.modifiers & Qt.AltModifier)
                                );
                                if (needAccepted) event.accepted = true;
                            }
                            Keys.onReleased: (event) => {
                                backend.keysReleased(event.key);
                            }
                        }

                        Item {
                            width: 100
                            height: parent.height
                            anchors.centerIn: parent

                            Text {
                                anchors.right: changeFormatter.left
                                anchors.rightMargin: 6
                                anchors.verticalCenter: parent.verticalCenter
                                font.pointSize: 10.5
                                text: backend.outputFormats.getTitle(viewModel.outputFormat) +
                                    (viewModel.actualFormat ? " / " + backend.outputFormats.getTitle(viewModel.actualFormat) : "")

                                MouseArea {
                                    anchors.fill: parent
                                    onPressed: {
                                        outputFormatMenu.open();
                                    }
                                }
                            }

                            IconButton {
                                id: changeFormatter
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.rightMargin: 2
                                anchors.verticalCenter: parent.verticalCenter
                                width: 15
                                height: 15
                                iconWidth: 15
                                iconHeight: 15
                                icon: storagePaths.icons + "downmenu.svg"
                                tooltipMessage: "Change output format"
                                onPressed: {
                                    outputFormatMenu.open();
                                }

                                Menu {
                                    id: outputFormatMenu
                                    y: -20
                                    modal: true
                                    focus: true
                                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                                    Instantiator {
                                        model: backend.outputFormats

                                        MenuItem {
                                            text: title
                                            onPressed: {
                                                viewModel.outputFormat = identifier;
                                                viewModel.reformatBody();
                                            }
                                        }

                                        onObjectAdded: function (index, object) {
                                            outputFormatMenu.insertItem(index, object)
                                        }
                                        onObjectRemoved: function (object) {
                                            outputFormatMenu.removeItem(object)
                                        }
                                    }
                                }
                            }
                        }

                        IconButton {
                            id: copyBodyButton
                            anchors.right: parent.right
                            anchors.rightMargin: 2
                            anchors.verticalCenter: parent.verticalCenter
                            width: 24
                            height: 24
                            iconWidth: 22
                            iconHeight: 22
                            icon: storagePaths.icons + "copy.svg"
                            tooltipMessage: "Copy all body to clipboard"
                            onPressed: {
                                viewModel.copyBodyToClipboard();
                            }
                        }
                    }

                    HorizontalDivider {
                        width: responsePanel.width
                    }
                }

                Item {
                    id: bodyContainer
                    anchors.top: panelsContainer.bottom
                    width: responsePanel.width
                    height: fieldContainer.height - panelsContainer.height - 4

                    Image {
                        anchors.fill: parent
                        fillMode: Image.Tile
                        source: storagePaths.images + "backgroundpattern.png"
                        opacity: .2
                    }

                    Loader {
                        id: listComponentLoader
                        visible: !viewModel.showImage
                        anchors.fill: parent
                        sourceComponent: !viewModel.showImage ? listComponent : null
                    }

                    Component {
                        id: listComponent

                        ListView {
                            id: listStrings
                            clip: true
                            anchors.fill: parent
                            flickDeceleration: 5000
                            flickableDirection: Flickable.HorizontalAndVerticalFlick
                            boundsBehavior: ListView.StopAtBounds
                            model: viewModel.bodyModel
                            delegate: Item {
                                width: listStrings.width
                                height: line.height

                                Rectangle {
                                    color: "black"
                                    opacity: .05
                                    anchors.fill: parent
                                    visible: isFindIndex
                                }
                                Text {
                                    id: line
                                    leftPadding: 4
                                    rightPadding: 10
                                    textFormat: viewModel.isFormatting ? Text.RichText : Text.PlainText
                                    text: currentLine
                                    width: bodyContainer.width
                                    wrapMode: Text.Wrap
                                    font.pointSize: 9
                                }
                            }
                            ScrollBar.vertical: ScrollBar {
                                active: true
                            }
                        }
                    }

                    Loader {
                        visible: viewModel.showImage
                        anchors.fill: parent
                        sourceComponent: viewModel.showImage ? imageComponent : null
                    }

                    Component {
                        id: imageComponent

                        BackendImage {
                            visible: viewModel.showImage
                            anchors.fill: parent
                            anchors.bottomMargin: 4
                            source: viewModel.bodyModel.bodyImage

                            MouseArea {
                                anchors.fill: parent

                                onPressed: {
                                    imageWindow.showWindow = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Item {
        id: spinnerContainer
        anchors.fill: parent
        visible: viewModel.isRunning

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

    Connections {
        target: backend
        function onChangedFindedIndex(findedLine){
            listComponentLoader.item.positionViewAtIndex(findedLine, ListView.Contain);
        }
    }
}
