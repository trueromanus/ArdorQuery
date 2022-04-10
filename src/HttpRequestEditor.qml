import QtQuick
import QtQuick.Controls

Item {
    anchors.fill: parent
    focus: true
    Keys.onPressed: (event) => {
        if ((event.key === Qt.Key_Enter || event.key === Qt.Key_Return) && event.modifiers & Qt.ControlModifier) {
            viewModel.addItem(-1);
            event.accepted = false;
        }
        if ((event.key === Qt.Key_Enter || event.key === Qt.Key_Return) && event.modifiers & Qt.AltModifier) {
            //viewModel.addItem(-1);
            event.accepted = false;
        }
    }

    property alias viewModel: listView.model

    ListView {
        id: listView
        anchors.fill: parent
        delegate: Item {
            width: listView.width
            height: textArea.contentHeight + textArea.font.pixelSize

            TextArea {
                id: textArea
                anchors.fill: parent
                text: textContent
                wrapMode: Text.WrapAnywhere
                background: Rectangle {
                    anchors.fill: parent
                    color: "#CDCDB4" // "#FDD12D" //"#E3D970" //"#CD919E" //"#83838D"
                    opacity: .3
                    border.width: isActive ? 1 : 0
                    //border.color: "#809fff"
                    border.color: "blue"
                }
                onTextChanged: {
                    listView.model.setItemContent(listView.model.selectedItem, text);
                }
                onFocusChanged: {
                    if (!focus) return;

                    listView.model.selectedItem = currentIndex;
                }
            }
        }
        flickDeceleration: 5000
        clip: true
        ScrollBar.vertical: ScrollBar {
            active: true
        }
    }
}
