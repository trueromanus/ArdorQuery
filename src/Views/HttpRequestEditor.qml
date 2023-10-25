import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */

Item {
    id: root
    anchors.fill: parent
    focus: true

    property alias viewModel: listView.model

    signal refreshFocus()

    onRefreshFocus: {
        listView.forceActiveFocus();
    }

    Image {
        anchors.fill: parent
        fillMode: Image.Tile
        source: storagePaths.images + "backgroundpattern.png"
        opacity: .4
    }

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        boundsBehavior: ListView.StopAtBounds
        flickDeceleration: 5000
        delegate: Item {
            width: listView.width
            height: textArea.contentHeight + textArea.font.pixelSize

            required property bool isNeedFocused
            required property string typeColor
            required property bool isActive
            required property string textContent
            required property int currentIndex

            onIsNeedFocusedChanged: {
                if (isNeedFocused) {
                    textArea.forceActiveFocus();
                } else {
                    textArea.focus = false;
                }
            }

            TextArea {
                id: textArea
                focus: isNeedFocused
                anchors.fill: parent
                text: textContent
                wrapMode: Text.WrapAnywhere
                selectByMouse: true
                background: Rectangle {
                    anchors.fill: parent
                    color: typeColor
                    opacity: .3
                    border.width: isActive ? 1 : 0
                    border.color: "blue"
                }
                onTextChanged: {
                    listView.model.setItemContent(currentIndex, text);
                }
                onPressed: {
                    if (listView.model.selectedItem !== currentIndex) listView.model.selectedItem = currentIndex;
                }
                Keys.onPressed: (event) => {
                    const isControl = event.modifiers & Qt.ControlModifier;
                    const isShift = event.modifiers & Qt.ShiftModifier;
                    const isAlt = event.modifiers & Qt.AltModifier;
                    if (isControl && event.key === Qt.Key_Z) { // disable shotcut Ctrl-Z because it can make undo
                        event.accepted = true;
                        return;
                    }
                    if (isAlt) {
                        event.accepted = true;
                        return;
                    }
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }

    Component.onCompleted: {
        viewModel.selectedItem = 0; //WORKAROUND: fix loosing focus after start application
    }
}
