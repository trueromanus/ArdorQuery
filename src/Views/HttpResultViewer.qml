import QtQuick
import QtQuick.Layouts
import "Controls"

Item {
    id: root
    anchors.fill: parent

    property var viewModel

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.preferredHeight: 120
            Layout.fillWidth: true
            Layout.fillHeight: true

            BorderedPanel {
                id: responsePanel
                anchors.fill: parent
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.bottomMargin: 4

                Column {
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

                    /*Text {
                        id: bodyText
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: viewModel.body
                        wrapMode: Text.WordWrap
                    }*/
                }
            }
        }

    }
}
