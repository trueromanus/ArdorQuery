import QtQuick 2.15
import QtQuick.Controls 2.15
import ArdorQuery.Backend 1.0
import "Views/Controls"

ApplicationWindow {
    id: window
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    font.capitalization: Font.MixedCase
    background: Rectangle {
        anchors.fill: parent
        color: "white"
    }
    title: "ArdorQuery"

    header: ApplicationHeader {
    }

    HttpRequestEditor {
        viewModel: backend.requests.selectedItem.requestModel
    }

    Item {
        BackendViewModel {
            id: backend
        }

        Item {
            id: storagePaths
            property string icons: Qt.resolvedUrl("../Views/Icons/")
        }
    }

}
