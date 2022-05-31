import QtQuick /* 2.15 */
import QtQuick.Layouts /* 1.15 */
import QtQuick.Controls /* 2.15 */
import "Controls"

Item {
    id: root
    anchors.fill: parent

    property var viewModel

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
                        width: responsePanel.width
                        title: "<b>Status:</b> " + viewModel.displayStatusCode + viewModel.networkError
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
                        width: responsePanel.width
                    }

                    BorderedHeader {
                        width: responsePanel.width
                        title: "Headers"

                        Item {
                            width: 200
                            height: 40
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
                    }

                    HorizontalDivider {
                        width: responsePanel.width
                    }

                    Item {
                        id: headersContainer
                        height: 180
                        width: responsePanel.width

                        ListView {
                            clip: true
                            anchors.fill: parent
                            model: viewModel.headers
                            delegate: Text {
                                leftPadding: 4
                                rightPadding: 10
                                width: headersContainer.width
                                wrapMode: Text.Wrap
                                text: modelData
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

                        Item {
                            width: 200
                            height: 40
                            IconButton {
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

                    ListView {
                        clip: true
                        anchors.fill: parent
                        flickableDirection: Flickable.HorizontalAndVerticalFlick
                        model: viewModel.bodyModel
                        delegate: Text {
                            leftPadding: 4
                            rightPadding: 10
                            textFormat: Text.PlainText
                            text: currentLine
                            width: bodyContainer.width
                            wrapMode: Text.Wrap
                        }
                        ScrollBar.vertical: ScrollBar {
                            active: true
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
}
