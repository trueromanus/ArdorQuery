import QtQuick
import QtQuick.Controls

Item {
    id: root
    property alias icon: image.source
    property alias iconWidth: image.width
    property alias iconHeight: image.height
    property string tooltipMessage: ""
    property bool hoveredContainer: false

    signal pressed()

    Image {
        id: image
        anchors.centerIn: parent
        width: 22
        height: 22
        mipmap: true
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            root.pressed();
        }
        onEntered: {
            cursorShape = Qt.PointingHandCursor;
            hoveredContainer = true;
        }
        onExited: {
            cursorShape = Qt.ArrowCursor;
            hoveredContainer = false;
        }
    }

    ToolTip.delay: 1000
    ToolTip.visible: tooltipMessage && hoveredContainer
    ToolTip.text: tooltipMessage
}
