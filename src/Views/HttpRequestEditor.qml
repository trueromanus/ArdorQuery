import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */

Item {
    anchors.fill: parent
    focus: true
    Keys.onPressed: (event) => {
        if ((event.key === Qt.Key_Enter || event.key === Qt.Key_Return) && event.modifiers & Qt.ControlModifier) {
            viewModel.addItem(listView.model.selectedItem + 1);
            event.accepted = false;
        }
        if ((event.key === Qt.Key_Enter || event.key === Qt.Key_Return) && event.modifiers & Qt.AltModifier) {
            if (listView.model.selectedItem > 0) viewModel.addItem(listView.model.selectedItem - 1);
            event.accepted = false;
        }
        if ((event.key === Qt.Key_Enter || event.key === Qt.Key_Return) && event.modifiers & Qt.ShiftModifier) {
            viewModel.addItem(-1);
            event.accepted = false;
        }
        // Ctrl-S or F5
        if ((event.nativeScanCode === 31 && (event.modifiers & Qt.ControlModifier)) || event.nativeScanCode === 63) {
            backend.requestPerformer.performRequest();
            event.accepted = false;
        }

        console.log(event.nativeScanCode)

        // Ctrl-L or F3
        if ((event.nativeScanCode === 38 && (event.modifiers & Qt.ControlModifier)) || event.nativeScanCode === 61) {
            //TODO: load from clipboard
            backend.requestExternal.appendFromClipboard();
            event.accepted = false;
        }

        // Ctrl-R
        if (event.nativeScanCode === 19 && (event.modifiers & Qt.ControlModifier)) {
            viewModel.clearFields()
            event.accepted = false;
        }
        // PgUp/Numpad PgUp or Ctrl-PgUp/Ctrl-Numpad PgUp
        if (event.nativeScanCode === 329 || event.nativeScanCode === 73) {
            if (event.modifiers & Qt.ControlModifier) {
                viewModel.selectFirstField();
            } else {
                viewModel.selectUpField();
            }
            event.accepted = false;
        }
        // PgDown/Numpad PgDown or Ctrl-PgDown/Ctrl-Numpad PgDown
        if (event.nativeScanCode === 337 || event.nativeScanCode === 81) {
            if (event.modifiers & Qt.ControlModifier) {
                viewModel.selectLastField();
            } else {
                viewModel.selectDownField();
            }
            event.accepted = false;
        }

    }

    property alias viewModel: listView.model

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        delegate: Item {
            width: listView.width
            height: textArea.contentHeight + textArea.font.pixelSize

            TextArea {
                id: textArea
                focus: isNeedFocused
                anchors.fill: parent
                text: textContent
                wrapMode: Text.WrapAnywhere
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
                onFocusChanged: {
                    if (!focus) {
                        if (isNeedFocused) {
                            focus = true;
                        }

                        return;
                    }

                    listView.model.selectedItem = currentIndex;
                }
            }
        }
        flickDeceleration: 5000
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }
}
