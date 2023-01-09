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
        buttonText: qsTr("Войти в аккаунт")
        anchors.top: root.top
        onClicked: {
            client.loadImages()
            loginPopup.open()
        }
    }

    BaseButton {
        id: _findLobbyBtn
        height: (root.height - 4 * groupSpace) / 3
        width: root.width
        buttonText: qsTr("Регистрация")
        anchors {
            top: _createLobbyBtn.bottom
            topMargin: 2 * groupSpace
        }
        onClicked: {
            if (client.questionsIsEmpty()) {
                client.sendRegisterQuestions()
            }

            registerPopup.open()
        }
    }
    BaseButton {
        id: _logOutBtn
        height: (root.height - 4 * groupSpace) / 3
        width: root.width
        buttonText: qsTr("Отсоединиться")
        anchors {
            top: _findLobbyBtn.bottom
            topMargin: 2 * groupSpace
        }
        onClicked: client.closeFromServer()
    }

    LoginPopup {
        id: loginPopup
        width: root.parent.width / 1.7
        height: root.parent.height / 1.7
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
    RegisterPopup {
        id: registerPopup
        width: root.parent.width / 1.7
        height: root.parent.height / 1.7
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }
}
