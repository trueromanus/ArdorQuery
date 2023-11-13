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
            }
        }
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }
}
