import QtQuick 2.15
import QtQuick.Controls 2.15
import Style 1.0
import Buttons 1.0

Popup {
    id: root
    anchors.centerIn: parent
    background: Rectangle {
        color: Style.mainPage.header.backColor
        radius: Style.radius
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
    }
    focus: true
    modal: true
    opacity: 0.9

    PopupHeader {
        headerText: qsTr("Игровое лобби: " + client.lobbyUud)
        id: _header
    }
    onClosed: {
        _selectTeamText.text = qsTr("Выберите команду!")
        _createLobby.text = qsTr("Создать комнату")
        _createLobby.enabled = false
        _teamBox.enabled = true
        if (client.lobbyCreated && !_createLobby._gameStart) {
            client.exitLobby()
        }
    }

    Rectangle {
        id: _selectTeamSection
        anchors {
            left: parent.left
            top: _header.bottom
            bottom: parent.bottom
            leftMargin: Style.registerPopup.margins
            topMargin: Style.registerPopup.margins
            bottomMargin: Style.registerPopup.margins
        }
        color: Style.registerPopup.backColor
        radius: Style.registerPopup.radius
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth

        height: parent.height - _header.height
        width: parent.width - (0.6 * parent.width + Style.registerPopup.margins)

        Text {
            id: _selectTeamText
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: Style.registerPopup.margins
                rightMargin: Style.registerPopup.margins
                topMargin: Style.registerPopup.margins
            }
            height: (parent.height - 5 * Style.registerPopup.margins) / 4
            width: _selectTeamSection
            text: qsTr("Выберите команду!")
            font {
                family: "SF Mono"
                pointSize: Math.min(_selectTeamText.height,
                                    _selectTeamText.width) / 8
                bold: true
            }
            color: Style.registerPopup.textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
        }

        Rectangle {
            id: _listSection
            height: (parent.height - 5 * Style.registerPopup.margins) / 4
            width: _selectTeamSection.width
            radius: Style.registerPopup.radius
            anchors {
                top: _selectTeamText.bottom
                left: parent.left
                right: parent.right
                leftMargin: Style.registerPopup.margins
                rightMargin: Style.registerPopup.margins
                topMargin: Style.registerPopup.margins
            }
            color: "transparent"
            border.color: Style.registerPopup.borderColor
            border.width: 3

            ListView {
                id: _teamBox
                anchors.centerIn: parent
                width: parent.width - 10
                height: parent.height - 10
                highlight: highlight
                highlightFollowsCurrentItem: true
                highlightMoveDuration: 489
                focus: true
                clip: true

                model: client.TeamSelect
                delegate: Rectangle {
                    id: delegateR
                    width: _teamBox.width
                    height: _teamBox.height
                    color: index
                           === _teamBox.currentIndex ? Qt.lighter(
                                                           Style.registerPopup.backColor,
                                                           1.2) : Style.registerPopup.backColor
                    radius: Style.registerPopup.radius
                    border.width: Style.registerPopup.borderWidth
                    border.color: Style.registerPopup.borderColor

                    Text {
                        anchors.fill: parent
                        anchors.margins: Style.registerPopup.margins
                        text: qsTr(client.TeamSelect[index])
                        wrapMode: Text.WordWrap
                        font {
                            family: "SF Mono"
                            pointSize: Math.min(delegateR.height,
                                                delegateR.width) / 8
                            bold: true
                        }
                        color: Style.registerPopup.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            _teamBox.currentIndex = index
                            _selectTeamText.text = qsTr(
                                        "Ваша команда: " + client.TeamSelect[index])
                            _createLobby.enabled = true
                        }
                    }
                }
            }
        }

        BaseButton {
            property int btnAction: 0
            property bool _gameStart: false
            id: _createLobby
            anchors {
                left: parent.left
                right: parent.right
                top: _listSection.bottom
                leftMargin: Style.registerPopup.margins
                rightMargin: Style.registerPopup.margins
                topMargin: Style.registerPopup.margins
            }
            height: (parent.height - 5 * Style.registerPopup.margins) / 4
            buttonText: qsTr("Создать комнату")
            borderRadius: Style.loginPopup.radius
            btnTextColorDown: Qt.darker(Style.registerPopup.textColor, 2)
            btnTextColorUp: Style.registerPopup.textColor
            btnBackColorDown: Qt.darker(Style.registerPopup.backColor, 2)
            btnBackColorUp: Style.registerPopup.backColor
            btnBorderColorDown: Qt.lighter(Style.registerPopup.borderColor, 2)
            btnBorderColorUp: Style.registerPopup.borderColor
            enabled: false
            onClicked: {
                if (!client.lobbyCreated) {
                    client.createLobby(_teamBox.currentIndex)
                    _teamBox.enabled = false
                    _createLobby.text = qsTr("Начать игру")
                } else {
                    client.startGame()
                    _createLobby._gameStart = true
                    root.close()
                }
            }
        }
        Rectangle {
            id: _playersInLobbyCountSection
            height: (parent.height - 5 * Style.registerPopup.margins) / 4
            width: _selectTeamSection.width
            radius: Style.registerPopup.radius
            color: "transparent"
            border.color: Style.registerPopup.borderColor
            border.width: 3
            anchors {
                top: _createLobby.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: Style.registerPopup.margins
            }
            Text {
                anchors.fill: parent
                anchors.margins: Style.registerPopup.margins
                text: qsTr("Игроков в лобби: " + client.players_lobby_count)
                wrapMode: Text.WordWrap
                font {
                    family: "SF Mono"
                    pointSize: Math.min(_playersInLobbyCountSection.height,
                                        _playersInLobbyCountSection.width) / 8
                    bold: true
                }
                color: Style.registerPopup.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }
    Rectangle {
        id: playersSection
        anchors {
            top: _header.bottom
            bottom: parent.bottom
            left: _selectTeamSection.right
            right: parent.right
            margins: Style.registerPopup.margins
        }
        width: parent.width - _selectTeamSection.width - 2 * Style.registerPopup.margins
        height: parent.height - _header.height
        color: "transparent"

        Rectangle {
            id: _friendsOnlineSection
            anchors {
                left: parent.left
                bottom: parent.bottom
                top: parent.top
            }
            width: parent.width / 2 - 0.5 * Style.registerPopup.margins
            height: parent.height
            color: Style.registerPopup.backColor
            radius: Style.registerPopup.radius
            border.color: Style.registerPopup.borderColor
            border.width: Style.registerPopup.borderWidth
            Text {
                id: _friendText
                width: parent.width
                height: _selectTeamText.height
                anchors {
                    top: parent.top
                    right: parent.right
                    left: parent.left
                }

                anchors.margins: Style.registerPopup.margins
                text: qsTr("Список друзей (онлайн):")
                wrapMode: Text.WordWrap
                font {
                    family: "SF Mono"
                    pointSize: Math.min(_friendText.height,
                                        _friendText.width) / 8
                    bold: true
                }
                color: Style.registerPopup.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            ListView {
                id: _friendBox
                anchors {
                    top: _friendText.bottom
                    right: parent.right
                    left: parent.left
                    bottom: parent.bottom
                    margins: Style.registerPopup.margins
                }
                highlight: highlight
                highlightFollowsCurrentItem: true
                highlightMoveDuration: 489
                focus: true
                clip: true

                model: client.playersList.friendsAliases
                delegate: Rectangle {
                    id: _friendDelegate
                    width: _friendBox.width - 10
                    height: _friendBox.height / 6
                    color: index
                           === _friendBox.currentIndex ? Qt.lighter(
                                                             Style.registerPopup.backColor,
                                                             1.2) : Style.registerPopup.backColor
                    radius: Style.registerPopup.radius
                    border.width: Style.registerPopup.borderWidth
                    border.color: Style.registerPopup.borderColor
                    Rectangle {
                        anchors {
                            top: parent.top
                            left: parent.left
                            bottom: parent.bottom
                            margins: Style.registerPopup.margins
                        }
                        id: deleteFriendImageBorder
                        height: parent.height
                        width: height
                        color: "transparent"
                        border.width: parent.height / 20
                        border.color: deleteFriendImageMA.containsMouse ? "transparent" : "black"
                        Image {
                            id: deleteFriendImage
                            source: Style.deleteFriend
                            sourceSize.height: parent.height - deleteFriendImageBorder.border.width
                            sourceSize.width: parent.heigh - deleteFriendImageBorder.border.width
                            anchors {
                                fill: parent
                            }
                        }
                        MouseArea {
                            id: deleteFriendImageMA
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                client.deleteFriend(index)
                            }
                        }
                    }
                    Rectangle {
                        anchors {
                            top: parent.top
                            left: deleteFriendImageBorder.right
                            bottom: parent.bottom
                            margins: Style.registerPopup.margins
                        }
                        id: inviteImageBorder
                        height: parent.height
                        width: height
                        color: "transparent"
                        border.width: parent.height / 20
                        border.color: inviteImageMA.containsMouse ? "transparent" : "black"
                        Image {
                            id: inviteImage
                            source: Style.invitePlayer
                            sourceSize.height: parent.height - inviteImageBorder.width
                            sourceSize.width: parent.heigh - inviteImageBorder.width
                            anchors {
                                fill: parent
                            }
                        }
                        MouseArea {
                            id: inviteImageMA
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                client.inviteFriend(index)
                            }
                        }
                    }
                    Text {
                        anchors {
                            left: inviteImageBorder.right
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                            margins: Style.registerPopup.margins
                        }
                        text: qsTr(client.playersList.friendsAliases[index])
                        wrapMode: Text.WordWrap
                        font {
                            family: "SF Mono"
                            pointSize: Math.min(_friendDelegate.height,
                                                _friendDelegate.width) / 8
                            bold: true
                        }
                        color: Style.registerPopup.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }
            }
        }
        Rectangle {
            id: _playersOnlineSection
            anchors {
                left: _friendsOnlineSection.right
                bottom: parent.bottom
                top: parent.top
                right: parent.right
                leftMargin: Style.registerPopup.margins
            }
            width: parent.width / 2 - 0.5 * Style.registerPopup.margins
            height: parent.height
            color: Style.registerPopup.backColor
            radius: Style.registerPopup.radius
            border.color: Style.registerPopup.borderColor
            border.width: Style.registerPopup.borderWidth
            Text {
                id: _playersText
                width: parent.width
                height: _selectTeamText.height
                anchors {
                    top: parent.top
                    right: parent.right
                    left: parent.left
                }

                anchors.margins: Style.registerPopup.margins
                text: qsTr("Список игроков (онлайн):")
                wrapMode: Text.WordWrap
                font {
                    family: "SF Mono"
                    pointSize: Math.min(_playersText.height,
                                        _playersText.width) / 8
                    bold: true
                }
                color: Style.registerPopup.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            ListView {
                id: _playerBox
                anchors {
                    top: _playersText.bottom
                    right: parent.right
                    left: parent.left
                    bottom: parent.bottom
                    margins: Style.registerPopup.margins
                }

                highlight: highlight
                highlightFollowsCurrentItem: true
                highlightMoveDuration: 489
                focus: true
                clip: true

                model: client.playersList.playersAliases
                delegate: Rectangle {
                    id: _playerDelegate
                    width: _playerBox.width - 10
                    height: _playerBox.height / 6
                    color: index
                           === _playerBox.currentIndex ? Qt.lighter(
                                                             Style.registerPopup.backColor,
                                                             1.2) : Style.registerPopup.backColor
                    radius: Style.registerPopup.radius
                    border.width: Style.registerPopup.borderWidth
                    border.color: Style.registerPopup.borderColor
                    Rectangle {
                        anchors {
                            top: parent.top
                            left: parent.left
                            bottom: parent.bottom
                            margins: Style.registerPopup.margins
                        }
                        id: addFriendImageBorder
                        height: parent.height
                        width: height
                        color: "transparent"
                        border.width: parent.height / 20
                        border.color: addFriendImageMA.containsMouse ? "transparent" : "black"
                        Image {
                            id: addFriendImage
                            source: Style.addFriend
                            sourceSize.height: parent.height - addFriendImageBorder.border.width
                            sourceSize.width: parent.heigh - addFriendImageBorder.border.width
                            anchors {
                                fill: parent
                            }
                            MouseArea {
                                id: addFriendImageMA
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    client.addFriend(index)
                                }
                            }
                        }
                    }
                    Rectangle {
                        anchors {
                            top: parent.top
                            left: addFriendImageBorder.right
                            bottom: parent.bottom
                            margins: Style.registerPopup.margins
                        }
                        id: invitePLayersImageBorder
                        height: parent.height
                        width: height
                        color: "transparent"
                        border.width: parent.height / 20
                        border.color: invitePlayersImageMA.containsMouse ? "transparent" : "black"
                        Image {
                            id: invitePlayersImage
                            source: Style.invitePlayer
                            sourceSize.height: parent.height - invitePLayersImageBorder.width
                            sourceSize.width: parent.heigh - invitePLayersImageBorder.width

                            anchors {
                                fill: parent
                            }
                            MouseArea {
                                id: invitePlayersImageMA
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    client.invitePlayer(index)
                                }
                            }
                        }
                    }
                    Text {
                        anchors {
                            left: invitePLayersImageBorder.right
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                            margins: Style.registerPopup.margins
                        }
                        text: qsTr(client.playersList.playersAliases[index])
                        wrapMode: Text.WordWrap
                        font {
                            family: "SF Mono"
                            pointSize: Math.min(_playerDelegate.height,
                                                _playerDelegate.width) / 8
                            bold: true
                        }
                        color: Style.registerPopup.textColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
