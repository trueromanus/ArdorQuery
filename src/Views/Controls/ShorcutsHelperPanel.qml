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

    property Item keysItem
    property var shortcuts

    ShortcutsListModel {
        id: viewModel
        shortcuts: root.shortcuts
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
                text: viewModel.filter
                onTextChanged: {
                    viewModel.filter = text;
                }
            }
        }

        ListView {
            id: listView
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: viewModel.filteredShortcuts
            delegate: Item {
                width: listView.width
                height: descriptionText.height + 4

                Text {
                    id: descriptionText
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    anchors.right: parent.right
                    anchors.rightMargin: 14
                    font.pointSize: 10
                    textFormat: Text.RichText
                    text: "<b>" + modelData.shortcuts + "</b> " + modelData.description
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
