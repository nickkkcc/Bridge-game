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
    modal: false
    opacity: 0.9

    PopupHeader {
        headerText: (client.findLobbyManager.headerText())
        id: _header
    }
    onClosed: {
        if (findLobbymanager.stateInt > 2) {
            client.exitInviterLobby()
        }
        findLobbymanager.selfExit = true
        client.stopFindGame()
    }

    Rectangle {
        id: _animatedSection
        anchors {
            top: _header.bottom
            left: parent.left
            bottom: parent.bottom
            margins: Style.registerPopup.margins
        }
        color: "transparent"
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
        height: parent.height - _header.height
        width: 0.6 * parent.width
        radius: Style.registerPopup.radius

        Image {
            id: _spadeImage
            source: "qrc:/resources/others/Ace of spades.svg"
            sourceSize.width: parent.width / 1.7
            sourceSize.height: parent.height / 1.5
            anchors {
                centerIn: parent

                margins: Style.registerPopup.margins
            }
        }
    }

    Rectangle {
        id: _teamSelectionSection
        anchors {
            top: _header.bottom
            right: parent.right
            left: _animatedSection.right
            margins: Style.registerPopup.margins
        }
        width: parent.width - _animatedSection.width
        height: 0.45 * (parent.height - _header.height)
        color: "transparent"
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
        radius: Style.registerPopup.radius

        Text {
            id: _selectTeamText
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: Style.registerPopup.margins
            }
            height: 0.5 * parent.height
            width: parent.width
            text: qsTr(client.findLobbyManager.selectTeamText())
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
            height: parent.height - _selectTeamText.height
            width: parent.width
            radius: Style.registerPopup.radius
            enabled: client.findLobbyManager.listEnabled()
            anchors {
                top: _selectTeamText.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: Style.registerPopup.margins
            }
            color: "transparent"
            border.color: Style.registerPopup.borderColor
            border.width: 3

            ListView {
                id: _teamBox
                enabled: client.findLobbyManager.listEnabled()
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
                            client.selectTeam(index)
                            findLobbymanager.selectedTeam = index
                        }
                    }
                }
            }
        }
    }
    Rectangle {
        id: _answerSection
        enabled: client.findLobbyManager.acceptBtnEnabled()
        anchors {
            top: _teamSelectionSection.bottom
            right: parent.right
            left: _animatedSection.right
            margins: Style.registerPopup.margins
        }
        width: parent.width - _animatedSection.width
        height: 0.25 * (parent.height - (_header.height + _teamSelectionSection.height))
        color: "transparent"
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
        radius: Style.registerPopup.radius

        Rectangle {
            id: acceptImageBorder

            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
                leftMargin: parent.width / 4
                margins: Style.registerPopup.margins
            }
            height: parent.height
            width: height
            color: "transparent"
            border.width: parent.height / 20
            border.color: acceptImageMA.containsMouse ? "transparent" : "black"
            radius: width / 2
            Image {
                id: acceptImage
                source: Style.acceptInvite
                sourceSize.height: parent.height - acceptImageBorder.border.width
                sourceSize.width: parent.width - acceptImageBorder.border.width
                anchors {
                    fill: parent
                }
            }
            MouseArea {
                id: acceptImageMA
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    client.acceptedInvite(true)
                }
            }
        }

        Rectangle {
            id: rejectImageBorder
            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
                rightMargin: parent.width / 4
                margins: Style.registerPopup.margins
            }
            height: parent.height
            width: height
            color: "transparent"
            border.width: parent.height / 20
            border.color: rejectImageMA.containsMouse ? "transparent" : "black"
            radius: width / 2
            Image {
                id: rejectImage
                source: Style.rejectInvite
                sourceSize.height: parent.height - rejectImageBorder.border.width
                sourceSize.width: parent.width - rejectImageBorder.border.width
                anchors {
                    fill: parent
                }
            }
            MouseArea {
                id: rejectImageMA
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    client.acceptedInvite(false)
                }
            }
        }
    }
    Rectangle {
        id: _statusSection
        anchors {
            top: _answerSection.bottom
            right: parent.right
            left: _animatedSection.right
            bottom: parent.bottom
            margins: Style.registerPopup.margins
        }
        width: parent.width - _animatedSection.width
        height: parent.height - (_header.height + _teamSelectionSection.height
                                 + _answerSection.height)
        color: "transparent"
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
        radius: Style.registerPopup.radius

        Rectangle {
            id: _statusTextSection
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
                margins: Style.registerPopup.margins
            }
            height: parent.height / 2.5
            width: parent.width
            color: "transparent"

            Text {
                id: _statusText
                anchors {
                    fill: parent
                }
                text: qsTr(client.findLobbyManager.statusText())
                wrapMode: Text.WordWrap
                font {
                    family: "SF Mono"
                    pointSize: Math.min(_statusText.height,
                                        _statusText.width) / 8
                    bold: true
                }
                color: Style.registerPopup.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        Rectangle {
            id: _playerCountTextSection
            anchors {
                top: _statusTextSection.bottom
                right: parent.right
                left: parent.left
                bottom: parent.bottom
                margins: Style.registerPopup.margins
            }
            height: parent.height / 2.5
            width: parent.width
            color: "transparent"

            Text {
                id: _playerCountText
                anchors {
                    fill: parent
                }
                text: qsTr(client.findLobbyManager.counPlayerText())
                wrapMode: Text.WordWrap
                font {
                    family: "SF Mono"
                    pointSize: Math.min(_playerCountText.height,
                                        _playerCountText.width) / 8
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
