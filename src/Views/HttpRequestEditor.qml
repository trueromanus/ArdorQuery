import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */

Item {
    id: root
    anchors.fill: parent
    focus: true
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

    property alias viewModel: listView.model

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
                textArea.focus = isNeedFocused;
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
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }
}
