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
    header: Item {
        id: headerContainer
        height: 30
        width: root.width

        GlobalVariablesHeader {
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
                backend.globalVariables.helpVisible = !backend.globalVariables.helpVisible;
            }
        }
    }

    Text {
        visible: !backend.globalVariables.hasLines
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
                    listView.forceActiveFocus();
                    textArea.forceActiveFocus();
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
                    backend.focusedHelpTextField = false;
                    if (listView.model.selected !== identifier) listView.model.selected = identifier;
                }               
                onActiveFocusChanged: {
                    //if (backend.tabs.currentTab !== 'Variables') return; //dirty hack but I don't know how to resolve it
                    if (backend.globalVariables.focusedHelpTextField) return; // fixing for text field in help

                    if (isActive && !textArea.activeFocus) {
                        textArea.forceActiveFocus();
                    }
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }

    Item {
        id: notificationContainer
        visible: backend.globalVariables.needShowSaveNotifications
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.rightMargin: 4
        width: 260
        height: 80

        Rectangle {
            anchors.fill: parent
            border.width: 1
            border.color: "#d3d3d3"
            radius: 4
            color: "#96d3d3d3"
        }

        Text {
            width: parent.width - 30
            height: parent.height - 2
            anchors.left: parent.left
            anchors.leftMargin: 4
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignJustify
            wrapMode: Text.WordWrap
            font.pointSize: 10
            maximumLineCount: 3
            elide: Text.ElideRight
            text: "Any changes will be discarded once the window is closed. Save your changes by pressing Ctrl-S."
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
                backend.globalVariables.needShowSaveNotifications = false;
            }
        }
    }

    ShorcutsHelperPanel {
        visible: backend.globalVariables.helpVisible
        shortcuts: backend.globalVariables.shortcuts
        onInnerTextPressed: {
            backend.globalVariables.focusedHelpTextField = true;
        }
        onVisibleChanged: {
            if (!visible) backend.globalVariables.focusedHelpTextField = false;
        }
    }

    Connections {
        target: globalEventHandler
        function onKeysChanged (state) {
            if (!root.activeFocusItem) return;

            const handled = backend.globalVariables.shortcutHandler(state);
            if (handled) globalEventHandler.setHandledLastSession();
        }
    }

    onActiveFocusItemChanged: {
        if (!root.activeFocusItem) globalEventHandler.clear();
    }

    onClosing: {
        backend.globalVariables.fillLines();
    }
}
