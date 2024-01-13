import QtQuick

Item {
    width: parent.width
    height: 20
    visible: backend.requestPerformer.countRequests > 1 || backend.requestPerformer.countFinishedRequests > 1

    Text {
        text: backend.requestPerformer.countFinishedRequests + " ready requests out of " + backend.requestPerformer.countRequests +
              (backend.requestPerformer.countErrorRequests > 0 ? ", with errors " + backend.requestPerformer.countErrorRequests : "")
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
    }
}
