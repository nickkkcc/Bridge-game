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
        headerText: qsTr("Восстановление пароля")
        id: _header
    }
    onClosed: {

    }
}
