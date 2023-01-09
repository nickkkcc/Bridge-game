import QtQuick 2.15
import Style 1.0
import Buttons 1.0

Item {
    id: root
    property var mainClient: null
    property real groupSpace: root.height / 15

    BaseButton {
        id: _connectBtn
        height: (parent.height - 2 * groupSpace) / 2
        width: parent.width
        buttonText: "Присоединиться"
        anchors.top: parent.top
        onClicked: client.connectToServer()
    }
    BaseButton {
        id: _exitBtn
        height: (parent.height - 2 * groupSpace) / 2
        width: parent.width
        buttonText: "Выход"
        anchors {
            top: _connectBtn.bottom
            topMargin: 2 * groupSpace
        }
        onClicked: Qt.quit()
    }
}
