import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
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
        id: _header
        headerText: qsTr("Регистрация")
    }
    onClosed: {
        loginTextEdit.clear()
        passwordTextEdit.clear()
        answerTextEdit.clear()
        loginTextEdit.focus = false
    }

    Rectangle {
        id: animationSections
        anchors {
            left: parent.left
            top: _header.bottom
            right: columnSection.left
            bottom: _registerBtn.top
            margins: Style.registerPopup.margins
        }
        height: parent.height - _header.height
        width: parent.width - columnSection.width
        color: "transparent"
        border.color: Style.registerPopup.borderColor
        border.width: Style.registerPopup.borderWidth
        radius: Style.registerPopup.radius
        Image {
            id: _spadeImage
            source: "qrc:/resources/others/Ace of spades 2.svg"
            sourceSize.width: parent.width
            sourceSize.height: parent.height
            anchors {
                fill: parent
                margins: Style.registerPopup.margins
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
        width: parent.width - (parent.width / 3)
        Column {
            id: cols
            height: columnSection.height
            width: columnSection.width
            spacing: Style.registerPopup.margins
            anchors {
                top: columnSection.top
                left: columnSection.left
                right: columnSection.right
                bottom: columnSection.bottom
                rightMargin: Style.registerPopup.margins
                leftMargin: Style.registerPopup.margins
                topMargin: Style.registerPopup.margins
                bottomMargin: Style.registerPopup.margins
            }
            Row {
                id: login
                width: parent.width
                height: (parent.height - 3 * parent.spacing - 2 * Style.registerPopup.margins) / 4
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
                    placeholderText: "Введите логин..."
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
                height: (parent.height - 3 * parent.spacing - 2 * Style.registerPopup.margins) / 4
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
            Row {
                id: questions
                width: parent.width
                height: (parent.height - 3 * parent.spacing - 2 * Style.registerPopup.margins) / 4
                spacing: Style.registerPopup.margins
                anchors {
                    right: parent.right
                    left: parent.left
                    rightMargin: Style.registerPopup.margins
                    leftMargin: Style.registerPopup.margins
                }
                Text {
                    id: questionsText
                    height: questions.height
                    width: 0.5 * questions.width
                    text: qsTr("Ваш вопрос: ")
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(questionsText.height,
                                            questionsText.width) / 8
                        bold: true
                    }
                    color: Style.registerPopup.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                Rectangle {
                    height: questions.height
                    width: questions.width - questionsText.width
                    color: "transparent"
                    border.color: Style.registerPopup.borderColor
                    border.width: 3

                    ListView {
                        id: questionsBox
                        anchors.centerIn: parent
                        width: parent.width - 10
                        height: parent.height - 10
                        highlight: highlight
                        highlightFollowsCurrentItem: true
                        highlightMoveDuration: 489
                        focus: true
                        clip: true

                        model: client.questions
                        delegate: Rectangle {
                            id: delegateR
                            width: questionsBox.width
                            height: questionsBox.height
                            color: index === questionsBox.currentIndex ? Qt.lighter(Style.registerPopup.backColor, 1.2) : Style.registerPopup.backColor
                            radius: Style.registerPopup.radius
                            border.width: Style.registerPopup.borderWidth
                            border.color: Style.registerPopup.borderColor

                            Text {
                                anchors.fill: parent
                                anchors.margins: Style.registerPopup.margins
                                text: qsTr(client.questions[index])
                                wrapMode: Text.WordWrap
                                font {
                                    family: "SF Mono"
                                    pointSize: Math.min(questionsBox.height,
                                                        questionsBox.width) / 8
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
                                    questionsBox.currentIndex = index
                                    _registerBtn.ready = true
                                }
                            }
                        }
                    }
                }
            }
            Row {
                id: answer
                width: parent.width
                height: (parent.height - 3 * parent.spacing - 2 * Style.registerPopup.margins) / 4
                spacing: Style.registerPopup.margins
                anchors {
                    right: parent.right
                    left: parent.left
                    rightMargin: Style.registerPopup.margins
                    leftMargin: Style.registerPopup.margins
                }
                Text {
                    id: answerText
                    height: answer.height
                    width: 0.5 * answer.width
                    text: qsTr("Ваш ответ: ")
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(answerText.height,
                                            answerText.width) / 8
                        bold: true
                    }
                    color: Style.registerPopup.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                TextField {
                    id: answerTextEdit
                    height: answer.height
                    width: answer.width - answerText.width
                    placeholderText: qsTr("Ответ на вопрос...")
                    placeholderTextColor: Qt.lighter(
                                              Style.registerPopup.textColor,
                                              1.5)
                    font {
                        family: "SF Mono"
                        pointSize: Math.min(answerTextEdit.height,
                                            answerTextEdit.width) / 8
                        bold: true
                    }
                    color: acceptableInput ? Style.registerPopup.textColor : "#A10035"
                    validator: RegularExpressionValidator {
                        regularExpression: /(?=^.{1,20}$)[0-9A-Za-zА-Яа-я]+/
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
    }
    BaseButton {
        id: _registerBtn
        property bool ready: false
        buttonText: qsTr("Зарегистрироваться")
        enabled: loginTextEdit.acceptableInput
                 && passwordTextEdit.acceptableInput
                 && answerTextEdit.acceptableInput && ready
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: columnSection.left
            margins: Style.registerPopup.margins
        }
        width: parent.width - columnSection.width
        height: parent.height / 5
        btnTextColorDown: Qt.darker(Style.registerPopup.textColor, 2)
        btnTextColorUp: Style.registerPopup.textColor
        btnBackColorDown: Qt.darker(Style.registerPopup.backColor, 2)
        btnBackColorUp: Style.registerPopup.backColor
        btnBorderColorDown: Qt.lighter(Style.registerPopup.borderColor, 2)
        btnBorderColorUp: Style.registerPopup.borderColor
        onClicked: client.sendRegister(loginTextEdit.text,
                                       passwordTextEdit.text,
                                       questionsBox.model.index,
                                       answerTextEdit.text)
    }
    //    Image {
    //        id: cardImage
    //    }
}
