import QtQuick 2.15
import Style 1.0
import Buttons 1.0
import Pages 1.0
import "qrc:/qml/Pages"

Item {
    id: root
    property real groupSpace: root.height / 15

    BaseButton {
        id: _createLobbyBtn
        height: (root.height - 4 * groupSpace) / 3
        width: root.width
        buttonText: qsTr("Создать лобби")
        anchors.top: root.top
        onClicked: {
            _lobbyPopup.open()
            client.clearPlayersList()
        }
    }

    BaseButton {
        id: _findLobbyBtn
        height: (root.height - 4 * groupSpace) / 3
        width: root.width
        buttonText: qsTr("Поиск лобби")
        anchors {
            top: _createLobbyBtn.bottom
            topMargin: 2 * groupSpace
        }
        onClicked: {
            client.findGame()
            client.findLobbyManager.selfExit = false
            _findLobbyPopup.open()
        }
    }
    BaseButton {
        id: _logOutBtn
        height: (root.height - 4 * groupSpace) / 3
        width: root.width
        buttonText: qsTr("Выйти из аккаунта")
        anchors {
            top: _findLobbyBtn.bottom
            topMargin: 2 * groupSpace
        }
        onClicked: {
            client.closeFromServer()
        }
    }

    LobbyPopup {
        id: _lobbyPopup
        width: root.parent.width / 1.7
        height: root.parent.height / 1.7
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
    FindLobbyPopup {
        id: _findLobbyPopup
        width: root.parent.width / 1.7
        height: root.parent.height / 1.7
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
}
