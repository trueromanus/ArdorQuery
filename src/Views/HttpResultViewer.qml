import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
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
                        title: "<b>Status:</b> " + viewModel.displayStatusCode
                    }

                    ResultText {
                        id: responseTime
                        width: responsePanel.width
                        title: "<b>Response time:</b> " + viewModel.responseTime
                    }

                    ResultText {
                        id: responseSize
                        width: responsePanel.width
                        title: "<b>Response size:</b> " + viewModel.responseSize
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

                    Flickable {
                        clip: true
                        anchors.fill: parent
                        flickableDirection: Flickable.HorizontalAndVerticalFlick
                        contentWidth: lines.width
                        contentHeight: lines.height

                        Column {
                            id: lines

                            Repeater {
                                model: viewModel.bodyModel
                                delegate: Text {
                                    id: lineText
                                    leftPadding: 4
                                    textFormat: Text.PlainText
                                    text: currentLine
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        ScrollBar.horizontal: ScrollBar {
                            active: true
                        }
                    }
                }
            }
        }
    }

    onVisibleChanged: {
        viewModel.bodyModel.visibleBody = root.visible;
    }
}
