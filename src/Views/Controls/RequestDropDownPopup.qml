import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root
    width: 500
    height: 300
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    modal: true

    RowLayout {
        anchors.fill: parent

        Rectangle {
            color: "green"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Item {
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            Text {
                id: titleChangeName
                anchors.top: parent.top
                font.pointSize: 12
                text: "Change name"
            }

            Item {
                anchors.top: titleChangeName.bottom
                width: parent.width
                height: 50

                RowLayout {
                    spacing: 0
                    anchors.fill: parent

                    TextField {
                        id: changeNameField
                        Layout.fillWidth: true
                        text: backend.requests.selectedItem.title
                    }

                    IconButton {
                        id: saveButton
                        Layout.preferredWidth: 29
                        iconWidth: 20
                        iconHeight: 20
                        width: 22
                        height: 22
                        icon: storagePaths.icons + "save.svg"
                        onPressed: {
                            backend.requests.changeNameForSelectedItem(changeNameField.text);
                        }
                    }
                }
            }
        }
    }
}
