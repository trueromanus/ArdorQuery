import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */
import ArdorQuery.Backend /* 1.0 */
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

    ShorcutsHelperPanel {
        id: shortcutPanel
    }

    Item {
        BackendViewModel {
            id: backend
            requestPerformer.httpRequest: backend.requests.selectedItem.requestModel
            requestPerformer.httpRequestResult: backend.requests.selectedItem.resultModel
            requestExternal.httpRequest: backend.requests.selectedItem.requestModel
            requestExternal.textAdvisor: backend.textAdviser
        }

        Item {
            id: storagePaths
            property string icons: Qt.resolvedUrl("../Views/Icons/")
        }
    }

}
