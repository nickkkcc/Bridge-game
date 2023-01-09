import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

//import Client 1.0
import Style 1.0
import Pages 1.0
import "qrc:/qml/Pages"
import Buttons 1.0
import QtQuick.Layouts 1.15
import MainClient 1.0
import PlayersList 1.0
import FindLobbyManage 1.0

Window {
    id: mainWindow
    visibility: "Maximized"
    visible: true
    title: qsTr("BridgeGame")

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: startPage
        background: BaseBackground {}
    }
    StartPage {
        id: startPage
    }

    MainPage {
        id: _mainPage
        visible: false
    }

    PrelobbyPage {
        id: _preLobbypage
        visible: false
    }

    GamePage {
        id: _gamePage
        visible: false
    }
    Client {
        id: client
        playersList: PlayersList {
            id: playersList
        }
        findLobbyManager: FindLobbyManager {
            id: findLobbymanager
        }
        onSendConnected: {
            stack.clear()
            stack.push(_mainPage)
        }

        onSendDisconnected: {

            stack.clear()
            stack.push(startPage)
        }
        onSendLoginAnswer: {
            stack.clear()
            stack.push(_preLobbypage)
        }
        onGameStarted: {
            stack.clear()
            stack.push(_gamePage)
        }
    }
}
