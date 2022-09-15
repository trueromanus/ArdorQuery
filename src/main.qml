import QtQuick
import QtQuick.Controls
import ArdorQuery.Backend
import QtQuick.Dialogs
import "Views/Controls"
import "Views/Windows"
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
        id: requestEditor
        visible: backend.tabs.currentTab === 'Request'
        viewModel: backend.requests.selectedItem.requestModel
    }

    HttpResultViewer {
        id: resultViewer
        visible: backend.tabs.currentTab === 'Result'
        viewModel: backend.requests.selectedItem.resultModel
    }

    ShorcutsHelperPanel {
        id: shortcutPanel
    }

    CommandPalette {
        id: commandPalette
        anchors.centerIn: parent
        visible: backend.openedCommandPalette
        enabled: backend.openedCommandPalette
    }

    NotificationPool {
        anchors.right: parent.right
        anchors.top: parent.top
        width: 290
        height: 200
    }

    AboutWindow {
        id: aboutWindow
    }

    Loader {
        id: imageWindow

        property bool showWindow

        sourceComponent: showWindow ? imageWindowComponent : null

        onLoaded: {
            imageWindow.item.showMaximized();
        }
    }

    Component {
        id: imageWindowComponent

        ImageResponseWindow {
            id: imageResponseWindow
            viewModel: backend.requests.selectedItem.resultModel
            onClosing: {
                imageWindow.showWindow = false;
            }
        }
    }


    Item {
        BackendViewModel {
            id: backend
            requestPerformer.httpRequest: backend.requests.selectedItem.requestModel
            requestPerformer.httpRequestResult: backend.requests.selectedItem.resultModel
            requestExternal.httpRequest: backend.requests.selectedItem.requestModel
            requestExternal.textAdvisor: backend.textAdviser
            onNeedOpenFile: {
                openDialog.open();
            }
            onNeedSaveFile: {
                saveDialog.open();
            }
            onNeedGenerateImage: {
                saveImageDialog.open();
            }
        }

        Connections {
            target: backend.tabs
            function onCurrentTabChanged () {
                switch (backend.tabs.currentTab) {
                    case 'Request':
                        requestEditor.refreshFocus();
                        break;
                    case 'Result':
                        resultViewer.refreshFocus();
                        break;
                }
            }
        }

        Item {
            id: storagePaths
            property string icons: Qt.resolvedUrl("../Views/Icons/")
            property string images: Qt.resolvedUrl("../Views/Images/")
        }
    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Text files (*.txt)", "Any files (*.*)"]
        onAccepted: {
            backend.openedFile(openDialog.selectedFile);
        }
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Text files (*.txt)", "Any files (*.*)"]
        onAccepted: {
            backend.savedFile(saveDialog.selectedFile);
        }
    }

    FileDialog {
        id: saveImageDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Image files (*.png)"]
        onAccepted: {
            backend.generateImage(saveImageDialog.selectedFile);
        }
    }

}
