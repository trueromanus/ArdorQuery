import QtQuick
import QtQuick.Controls
import ArdorQuery.Backend

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

    HttpRequestViewModel {
        id: viewModel
    }

    HttpRequestEditor {
        viewModel: viewModel
    }
}
