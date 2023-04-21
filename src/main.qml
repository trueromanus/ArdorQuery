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

    footer: ApplicationFooter {
    }

    Item {
        id: keysItem
        Keys.onPressed: (event) => {
            const needAccepted = backend.keysHandler(
                event.key,
                event.nativeScanCode,
                (event.modifiers & Qt.ControlModifier),
                (event.modifiers & Qt.ShiftModifier),
                (event.modifiers & Qt.AltModifier)
            );
            if (needAccepted) event.accepted = true;
        }
        Keys.onReleased: (event) => {
            backend.keysReleased(event.key);
        }
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
        visible: backend.helpVisible
        keysItem: keysItem
        Component.onCompleted: {
            shortcutPanel.mode = "main";
        }
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

    Loader {
        id: openApiExportWindow

        property bool showWindow

        sourceComponent: showWindow ? openApiWindowComponent : null

        onLoaded: {
            openApiExportWindow.item.show();
        }
    }

    Component {
        id: openApiWindowComponent

        OpenApiExportWindow {
            onClosing: {
                openApiExportWindow.showWindow = false;
            }
        }
    }

    Loader {
        id: globalVariablesWindow

        property bool showWindow

        sourceComponent: showWindow ? globalVariablesWindowComponent : null

        onLoaded: {
            globalVariablesWindow.item.show();
            backend.globalVariables.fillLines();
        }
    }

    Component {
        id: globalVariablesWindowComponent

        GlobalVariablesWindow {
            onClosing: {
                globalVariablesWindow.showWindow = false;
                backend.globalVariables.clearLines();
            }
        }
    }

    Item {
        GlobalMouseViewModel {
            id: globalMouseViewModel
        }

        BackendViewModel {
            id: backend
            requestExternal.httpRequest: backend.requests.selectedItem.requestModel
            requestExternal.textAdvisor: backend.textAdviser
            requestPerformer.globalVariable: backend.globalVariables
            globalVariables.onCloseWindowRequired: {
                globalVariablesWindow.item.close();
            }
            onNeedOpenFile: {
                openDialog.open();
            }
            onNeedSaveFile: {
                saveDialog.open();
            }
            onNeedGenerateImage: {
                saveImageDialog.open();
            }
            onNeedOpenApiExportWindow: {
                openApiExportWindow.showWindow = true;
            }
            onNeedGlobalVariablesWindow: {
                globalVariablesWindow.showWindow = true;
            }
            Component.onDestruction: {
                backend.openApiExporter.addresses.saveSavedOptions();
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
