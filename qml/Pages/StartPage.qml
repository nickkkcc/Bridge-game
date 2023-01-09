import QtQuick 2.15
import QtQuick.Controls 2.15
import Style 1.0
//import MainClient 1.0
import Buttons 1.0
import "qrc:/qml/Images"

Page {
    id: root
    visible: false
    background: BaseBackground {}

    StartPageImage {
        id: image
    }

    StartPageButtonGroup {
        id: _btnGroup
        mainClient: pageClient
        anchors.top: image.bottom
        anchors.horizontalCenter: image.horizontalCenter
        anchors.topMargin: parent.height / 10
        height: parent.height / 4
        width: parent.width / 5
    }
    Component.onCompleted: {
        visible: true
    }
}
