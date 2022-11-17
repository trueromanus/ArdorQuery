import QtQuick /* 2.15 */
import QtQuick.Layouts /* 2.15 */
import QtQuick.Controls /* 2.15 */
import ArdorQuery.Backend /* 1.0 */

Rectangle {
    id: root
    width: 280
    height: parent.height
    anchors.right: parent.right
    visible: false

    property alias mode: listModel.mode

    ShortcutsListModel {
        id: listModel
    }

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        anchors.left: parent.left
        width: 1
        height: parent.height
        color: "#e7eaec"
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 6

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            CommonTextField {
                anchors.margins: 4
                activeFocusOnTab: false
                anchors.fill: parent
                text: listModel.filter
                onTextChanged: {
                    listModel.filter = text;
                }
            }
        }

        ListView {
            id: listView
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: listModel
            delegate: Item {
                required property string title
                required property string shortcuts
                required property string description

                width: listView.width
                height: titleText.height + descriptionText.height + shortcutsText.height + 20
                Text {
                    id: titleText
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    font.pointSize: 14
                    font.bold: true
                    text: title
                }
                Text {
                    id: descriptionText
                    anchors.top: titleText.bottom
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    font.pointSize: 10
                    text: description
                    wrapMode: Text.WordWrap
                }
                Text {
                    id: shortcutsText
                    anchors.top: descriptionText.bottom
                    anchors.topMargin: 8
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    font.pointSize: 10
                    text: shortcuts
                    wrapMode: Text.WordWrap
                }
            }
            ScrollBar.vertical: ScrollBar {
                active: true
            }
        }
    }

    onVisibleChanged: {
        if (!root.visible) root.nextItemInFocusChain().forceActiveFocus();
    }
}
