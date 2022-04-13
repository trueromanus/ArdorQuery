import QtQuick 2.15
import QtQuick.Controls 2.15

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
