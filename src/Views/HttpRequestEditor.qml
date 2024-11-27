import QtQuick
import QtQuick.Controls

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
        focus: true
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
            property int lastCursorPosition

            onIsNeedFocusedChanged: {
                if (isNeedFocused) {
                    listView.forceActiveFocus();
                    textArea.forceActiveFocus();
                }
            }

            TextArea {
                id: textArea
                focus: isNeedFocused
                anchors.fill: parent
                text: textContent
                wrapMode: Text.WrapAnywhere
                selectByMouse: true
                font.pointSize: 12
                activeFocusOnTab: false
                background: Rectangle {
                    anchors.fill: parent
                    color: typeColor
                    opacity: .3
                    border.width: isActive ? 1 : 0
                    border.color: "#DC0000FF"
                }
                onTextChanged: {
                    listView.model.setItemContent(currentIndex, text);
                }
                onPressed: {
                    backend.focusedHelpTextField = false;
                    if (listView.model.selectedItem !== currentIndex) {
                        listView.model.selectedItem = currentIndex;
                        if (backend.showGlobalVariablesPopup) backend.closeGlobalVariables();
                    }
                }
                onActiveFocusChanged: {
                    if (backend.tabs.currentTab !== 'Request') return; //dirty hack but I don't know how to resolve it
                    if (backend.focusedHelpTextField) return; // fixing for text field in help

                    if (isNeedFocused && !textArea.activeFocus && !textArea.focus) {
                        textArea.focus = true;
                    }
                }
                onCursorPositionChanged: {
                    listView.model.setItemCursor(currentIndex, textArea.cursorPosition);
                }
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }

    Item {
        id: globalVariablesPopup
        visible: backend.showGlobalVariablesPopup
        width: 200
        height: 100

        Rectangle {
            anchors.fill: parent
            border.width: 1
            border.color: "lightgray"
            radius: 4
        }

        ListView {
            id: globalVariablesListView
            anchors.fill: parent
            model: backend.globalVariables.variableNames
            delegate: Item {
                width: globalVariablesListView.width
                height: 30

                Rectangle {
                    visible: modelData === backend.selectedGlobalVariable
                    anchors.fill: parent
                    color: "lightgray"
                    opacity: .8
                    anchors.leftMargin: 1
                    anchors.rightMargin: 1
                }

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    text: modelData
                    verticalAlignment: Text.AlignVCenter
                    maximumLineCount: 2
                    wrapMode: Text.Wrap
                    elide: Text.ElideRight
                }
            }
            onVisibleChanged: {
                if (!globalVariablesPopup.visible) return;

                let activeItem;
                for (const item of listView.contentItem.children) {
                    if (item.isActive) activeItem = item;
                }

                if (!activeItem) return;

                const textField = activeItem.children[0];
                let originalText = textField.text;
                let cursorPosition = textField.cursorPosition;

                let leftCursor = textField.cursorRectangle.x;
                const leftCursotLimit = root.width - globalVariablesPopup.width;
                if (leftCursor > leftCursotLimit) leftCursor = leftCursotLimit;

                globalVariablesPopup.x = leftCursor;
                let bottomElement = activeItem.y + activeItem.height - listView.contentY;
                if (bottomElement > root.height - 100) bottomElement = activeItem.y - listView.contentY - globalVariablesPopup.height;
                globalVariablesPopup.y = bottomElement;
            }
        }
    }

    Connections {
        target: backend
        function onGlobalVariableSelected(lastPosition) {
            let activeItem;
            for (const item of listView.contentItem.children) {
                if (item.isActive) activeItem = item;
            }

            if (!activeItem) return;

            const textField = activeItem.children[0];
            textField.cursorPosition = lastPosition;
        }
    }

    Text {
        id: emptyText
    }

    TextMetrics {
        id: textMetrics
        font.family: emptyText.font.family
        font.pointSize: 12
        elide: Text.ElideRight
    }

}
