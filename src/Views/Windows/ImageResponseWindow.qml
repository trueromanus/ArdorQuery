import QtQuick /* 2.15 */
import QtQuick.Controls /* 2.15 */
import ArdorQuery.Backend /* 1.0 */

ApplicationWindow {
    id: root

    property var viewModel

    background: Rectangle {
        anchors.fill: parent
        color: "lightgrey"
    }

    Flickable {
        anchors.fill: parent
        contentHeight: backendImage.height
        contentWidth: backendImage.width

        BackendImage {
            id: backendImage
            width: viewModel.bodyModel.bodyImageWidth
            height: viewModel.bodyModel.bodyImageHeight
            source: viewModel.bodyModel.bodyImage
        }

        ScrollBar.vertical: ScrollBar {
            active: true
        }
        ScrollBar.horizontal: ScrollBar {
            active: true
        }
    }
}
