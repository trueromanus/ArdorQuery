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
    minimumWidth: 600
    minimumHeight: 480
    width: 800
    height: 600
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

    HttpRequestEditor {
        id: requestEditor
        focus: true
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
        shortcuts: backend.shortcuts
        onInnerTextPressed: {
            backend.focusedHelpTextField = true;
        }
        onVisibleChanged: {
            if (!visible) backend.focusedHelpTextField = false;
        }
    }

    HistoryRequestsPalette {
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
        BackendViewModel {
            id: backend
            requestExternal.httpRequest: backend.requests.selectedItem.requestModel
            requestExternal.textAdvisor: backend.textAdviser
            requestPerformer.globalVariable: backend.globalVariables
            openApiExporter.onNeedCloseWindow: {
                openApiExportWindow.item.close();
            }
            openApiExporter.onSelectRouteFromList: function (identifier, replaceCurrent) {
                backend.importFromOpenApi(identifier, replaceCurrent);
            }
            globalVariables.onCloseWindowRequired: {
                globalVariablesWindow.item.close();
            }
            onNeedOpenFile: {
                openDialog.open();
                globalEventHandler.clear();
            }
            onNeedSaveFile: {
                saveDialog.open();
                globalEventHandler.clear();
            }
            onNeedGenerateImage: {
                saveImageDialog.open();
                globalEventHandler.clear();
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

    FileDialog {
        id: saveDownloadFileDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Any files (*.*)"]
        selectedFile: backend.requests.selectedItem.resultModel.defaultDownloadFile
        onAccepted: {
            backend.saveDownloadedFile(saveDownloadFileDialog.selectedFile);
        }
    }

    GlobalEventHandlerModel {
        id: globalEventHandler
        onKeysChanged: function (shortcut) {
            if (!window.activeFocusItem) return;

            const handled = backend.shortcutHandler(shortcut);
            if (handled) globalEventHandler.setHandledLastSession();
        }
    }

    onActiveFocusItemChanged: {
        if (!window.activeFocusItem) globalEventHandler.clear();
    }

    Component.onDestruction: {
        backend.saveCurrentRequestsToProfile();
    }
}
