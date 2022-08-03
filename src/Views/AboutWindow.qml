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
    title: "About"

    Image {
        id: logoImage
        source: storagePaths.icons + "logo.svg"
        width: 50
        height: 50
        mipmap: true
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    Text {
        id: applicationName
        anchors.left: logoImage.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 4
        text: "ArdorQuery"
        font.pointSize: 11
        font.bold: true
    }

    Text {
        id: versionInfo
        anchors.left: logoImage.right
        anchors.top: applicationName.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 4
        text: "version 0.0.2"
        font.pointSize: 10
    }

    Text {
        id: opensourceInfo
        anchors.left: logoImage.right
        anchors.top: versionInfo.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 12
        text: "This application is open source (GPL 3.0).<br>You can check out sources <a href='https://github.com/trueromanus/ArdorQuery'>at this address</a>."
        font.pointSize: 10
        onLinkActivated: function (link) {
            Qt.openUrlExternally(link);
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }

    Text {
        id: qtInfo
        anchors.left: logoImage.right
        anchors.top: opensourceInfo.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 12
        text: "Used Qt version 6.2.1<br>If you want to get sources of Qt please <a href='mailto:rorhww3glmwj@list.ru'>email this address</a>."
        font.pointSize: 10
        onLinkActivated: function (link) {
            Qt.openUrlExternally(link);
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }

    Text {
        id: copyrightInfo
        anchors.left: logoImage.right
        anchors.top: qtInfo.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 12
        text: "Copyright (c) 2022 Roman Vladimirov. All right reserved."
        font.pointSize: 10
    }
}
