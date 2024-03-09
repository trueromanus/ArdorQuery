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
                    if (listView.model.selectedItem !== currentIndex) listView.model.selectedItem = currentIndex;
                }
                onActiveFocusChanged: {
                    if (backend.tabs.currentTab !== 'Request') return; //dirty hack but I don't know how to resolve it
                    if (backend.focusedHelpTextField) return; // fixing for text field in help

                    if (isNeedFocused && !textArea.activeFocus) {
                        textArea.forceActiveFocus();
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

    ListView {
        id: globalVariablesListView
        visible: backend.showGlobalVariablesPopup
        width: 100
        height: 100
        model: backend.globalVariables.variableNames
        delegate: Item {
            width: globalVariablesListView.width
            height: 30

            Rectangle {
                visible: modelData === backend.selectedGlobalVariable
                anchors.fill: parent
                color: "lightgray"
                opacity: .8
            }

            Text {
                text: modelData
                maximumLineCount: 2
                wrapMode: Text.Wrap
            }
        }
        onVisibleChanged: {
            if (!globalVariablesListView.visible) return;

            let activeItem;
            for (const item of listView.contentItem.children) {
                if (item.isActive) activeItem = item;
            }

            if (!activeItem) return;

            const textField = activeItem.children[0];
            let originalText = textField.text;
            let cursorPosition = textField.cursorPosition;

            let leftCursor = textField.cursorRectangle.x;
            const leftCursotLimit = parent.width - globalVariablesListView.width;
            if (leftCursor > leftCursotLimit) leftCursor = leftCursotLimit;

            globalVariablesListView.x = leftCursor;
            let bottomElement = activeItem.y + activeItem.height - listView.contentY;
            if (bottomElement > root.height - 100) bottomElement = activeItem.y - listView.contentY - globalVariablesListView.height;
            globalVariablesListView.y = bottomElement;
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
