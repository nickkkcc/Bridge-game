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
        headerText: qsTr("Войти в аккаунт")
        id: _header
    }
    onClosed: {

        loginTextEdit.clear()
        passwordTextEdit.clear()
        loginTextEdit.focus = false
    }
    Rectangle {
        id: animationSections
        anchors {
            left: parent.left
            top: _header.bottom
            right: columnSection.left
            bottom: parent.bottom
            margins: Style.registerPopup.margins
        }
        height: parent.height - _header.height
        width: parent.width - columnSection.width
        color: "transparent"
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
        radius: Style.registerPopup.radius

        ListView {
            id: _cardView
            anchors.centerIn: parent
            width: parent.width - 200
            height: parent.height - 30
            highlight: highlight
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 489
            focus: true
            clip: true

            model: client.cardPreView
            delegate: Rectangle {
                id: delegateR
                width: _cardView.width
                height: _cardView.height
                color: index === _cardView.currentIndex ? Qt.lighter(
                                                              Style.registerPopup.backColor,
                                                              1.2) : Style.registerPopup.backColor
                radius: Style.registerPopup.radius
                border.width: Style.registerPopup.borderWidth
                border.color: Style.registerPopup.borderColor

                Image {
                    id: imageCard
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom

                        margins: Style.registerPopup.margins
                    }
                    source: client.cardPreView[index]
                    sourceSize.height: parent.height
                    sourceSize.width: parent.width
                }
            }
        }
    }

    Rectangle {
        id: columnSection
        anchors {
            right: parent.right
            top: _header.bottom
            bottom: parent.bottom
            margins: Style.registerPopup.margins
        }
        color: Style.registerPopup.backColor
        radius: Style.registerPopup.radius
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth

        height: parent.height - _header.height
        width: parent.width - (parent.width / 2)
        Column {
            id: cols
            height: 0.5 * columnSection.height
            width: columnSection.width
            spacing: Style.registerPopup.margins
            anchors {
                top: columnSection.top
                left: columnSection.left
                right: columnSection.right
                rightMargin: Style.registerPopup.margins
                leftMargin: Style.registerPopup.margins
                topMargin: Style.registerPopup.margins
            }
            Row {
                id: login
                width: parent.width
                height: (parent.height - parent.spacing - 2 * Style.registerPopup.margins) / 2
                anchors {
                    right: parent.right
                    left: parent.left
                    rightMargin: Style.registerPopup.margins
                    leftMargin: Style.registerPopup.margins
                }
                spacing: Style.registerPopup.margins
                Text {
                    id: loginText
                    height: parent.height
                    width: 0.5 * parent.width
                    text: qsTr("Ваш логин: ")
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(loginText.height,
                                            loginText.width) / 8
                        bold: true
                    }
                    color: Style.registerPopup.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                TextField {
                    id: loginTextEdit
                    height: parent.height
                    width: parent.width - loginText.width
                    onTextChanged: acceptableInput ? color = Style.registerPopup.textColor : color
                                                     = "#A10035"
                    placeholderText: qsTr("Введите логин...")
                    placeholderTextColor: Qt.lighter(
                                              Style.registerPopup.textColor,
                                              1.5)
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(loginTextEdit.height,
                                            loginTextEdit.width) / 8
                        bold: true
                    }
                    color: Style.registerPopup.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle {
                        color: Style.registerPopup.backColor
                        border.color: Style.registerPopup.borderColor
                        border.width: Style.registerPopup.borderWidth / 2
                    }
                    validator: RegularExpressionValidator {
                        regularExpression: /(?=^.{5,15}$)[0-9A-Za-zА-Яа-я]+/
                    }
                }
            }
            Row {
                id: password
                width: parent.width
                height: (parent.height - parent.spacing - 2 * Style.registerPopup.margins) / 2
                spacing: Style.registerPopup.margins
                anchors {
                    right: parent.right
                    left: parent.left
                    rightMargin: Style.registerPopup.margins
                    leftMargin: Style.registerPopup.margins
                }
                Text {
                    id: passwordText
                    height: parent.height
                    width: 0.5 * parent.width
                    text: qsTr("Ваш пароль: ")
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(passwordText.height,
                                            passwordText.width) / 8
                        bold: true
                    }
                    color: Style.registerPopup.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                TextField {
                    id: passwordTextEdit
                    height: password.height
                    width: password.width - passwordText.width
                    placeholderText: qsTr("Введите пароль...")
                    placeholderTextColor: Qt.lighter(
                                              Style.registerPopup.textColor,
                                              1.5)
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(password.height, password.width) / 8
                        bold: true
                    }
                    color: acceptableInput ? Style.registerPopup.textColor : "#A10035"
                    validator: RegularExpressionValidator {
                        regularExpression: /(?=^.{5,15}$)[0-9A-Za-zА-Яа-я]+/
                    }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    background: Rectangle {
                        color: Style.registerPopup.backColor
                        border.color: Style.registerPopup.borderColor
                        border.width: Style.registerPopup.borderWidth / 2
                    }
                }
            }
        }

        Rectangle {
            id: btnSection
            anchors {
                top: cols.bottom
                bottom: columnSection.bottom
                left: columnSection.left
                right: columnSection.right
                margins: Style.loginPopup.margins
            }
            color: "transparent"
            BaseButton {
                id: _loginBtn
                anchors.centerIn: btnSection
                width: btnSection.width / 2
                height: btnSection.height / 2.5
                buttonText: qsTr("Войти")
                borderRadius: Style.loginPopup.radius
                btnTextColorDown: Qt.darker(Style.registerPopup.textColor, 2)
                btnTextColorUp: Style.registerPopup.textColor
                btnBackColorDown: Qt.darker(Style.registerPopup.backColor, 2)
                btnBackColorUp: Style.registerPopup.backColor
                btnBorderColorDown: Qt.lighter(Style.registerPopup.borderColor,
                                               2)
                btnBorderColorUp: Style.registerPopup.borderColor
                enabled: loginTextEdit.acceptableInput
                         && passwordTextEdit.acceptableInput
                onClicked: client.sendLogin(loginTextEdit.text,
                                            passwordTextEdit.text)
            }
        }
    }
}
