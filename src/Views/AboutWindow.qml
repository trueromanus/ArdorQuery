import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    width: 500
    height: 240
    minimumWidth: 500
    minimumHeight: 240
    maximumWidth: 500
    maximumHeight: 240
    modality: Qt.WindowModal
    flags: Qt.platform.os !== `windows` ? Qt.Dialog : Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "About ArdorQuery"

    Rectangle {
        anchors.fill: parent
        color: "#f2f2f2"
    }

    Image {
        id: logoImage
        source: storagePaths.icons + "logo.svg"
        width: 54
        height: 54
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
        text: "version 0.0.10"
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
        text: "Used 3rd libraries and technologies:<br>
            &nbsp;&nbsp;&nbsp;<a href='https://www.qt.io/'>Qt framework</a> cross-platform UI toolkit<br>
            &nbsp;&nbsp;&nbsp;<a href='https://www.openssl.org/'>OpenSSL</a> transport layer security<br>
            <br>
            If you have any questions please <a href='mailto:trueromanus@outlook.com'>email at this address</a>."
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
        text: "Copyright (c) 2022-2023 Roman Vladimirov"
        font.pointSize: 10
    }
}
