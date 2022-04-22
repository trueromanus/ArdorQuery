import QtQuick
import QtQuick.Controls
import ArdorQuery.Backend
import "Views/Controls"
import "Views"

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
        visible: backend.tabs.currentTab === 'Request'
        viewModel: backend.requests.selectedItem.requestModel
    }

    HttpResultViewer {
        visible: backend.tabs.currentTab === 'Result'
        viewModel: backend.requests.selectedItem.resultModel
    }

    Item {
        BackendViewModel {
            id: backend
            requestPerformer.httpRequest: backend.requests.selectedItem.requestModel
            requestPerformer.httpRequestResult: backend.requests.selectedItem.resultModel
        }

        Item {
            id: storagePaths
            property string icons: Qt.resolvedUrl("../Views/Icons/")
        }
    }

}
