import QtQuick
import QtQuick.Controls

Text {
    property string linkTooltip: ""

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
    }

    ToolTip.delay: 1000
    ToolTip.visible: linkTooltip && hoveredLink
    ToolTip.text: linkTooltip
}
