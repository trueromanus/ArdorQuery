import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Controls"

ApplicationWindow {
    id: root
    width: 600
    height: 500
    minimumWidth: 500
    minimumHeight: 300
    maximumWidth: 800
    maximumHeight: 800
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "Global Variables"
    background: Rectangle {
        anchors.fill: parent
        color: "lightgray"
        opacity: .3
    }

    Text {
        text: "To add a new global variable press Ctrl-Enter<br />To save changes press Ctrl-S"
        anchors.centerIn: parent
        font.pointSize: 11
        horizontalAlignment: Text.AlignHCenter
        color: "gray"
    }

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        boundsBehavior: ListView.StopAtBounds
        flickDeceleration: 5000
        model: backend.globalVariables
        delegate: Item {
            width: listView.width
            height: textArea.contentHeight + textArea.font.pixelSize

            required property bool isActive
            required property string line
            required property int identifier
            required property bool isOdd

            onIsActiveChanged: {
                if (isActive) {
                    textArea.forceActiveFocus();
                } else {
                    textArea.focus = false;
                }
            }

            TextArea {
                id: textArea
                focus: isActive
                anchors.fill: parent
                text: line
                wrapMode: Text.WrapAnywhere
                selectByMouse: true
                background: Rectangle {
                    anchors.fill: parent
                    color: isOdd ? "#E0D8B0" : "#8FBDD3"
                    opacity: .3
                    border.width: isActive ? 1 : 0
                    border.color: "blue"
                }
                onTextChanged: {
                    listView.model.setLine(identifier, text);
                }
                onPressed: {
                    if (listView.model.selected !== identifier) listView.model.selected = identifier;
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

    Connections {
        target: globalEventHandler
        function onKeysChanged (state) {
            if (!root.activeFocusItem) return;

            backend.globalVariables.shortcutHandler(state);
        }
    }
}
