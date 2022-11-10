import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    width: 500
    height: 190
    minimumWidth: 500
    minimumHeight: 190
    maximumWidth: 500
    maximumHeight: 190
    modality: Qt.WindowModal
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "Export from OpenAPI"

    TextField {
        id: urlTextField
        anchors.left: parent.left
        anchors.right: parent.right
        text: backend.openApiExporter.url
        onTextChanged: {
            backend.openApiExporter.setUrl(text);
        }
    }

    Button {
        id: loadButton
        anchors.top: urlTextField.bottom
        text: backend.openApiExporter.alreadyLoaded ? "Reload scheme" : "Load scheme"
        enabled: backend.openApiExporter.url.length > 0
        onPressed: {
            backend.openApiExporter.loadOpenApiScheme();
        }
    }

    Text {
        anchors.top: loadButton.bottom
        visible: backend.openApiExporter.loading
        text: "Loading..."
    }
}
