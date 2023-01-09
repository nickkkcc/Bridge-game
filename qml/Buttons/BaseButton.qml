import QtQuick 2.15
import QtQuick.Controls 2.15
import Style 1.0

Button {
    property string buttonText: text
    property int borderRadius: _background.radius
    property int borderWidth: _background.border.width
    property color btnTextColorDown: Style.baseButton.btnTextColorDown
    property color btnTextColorUp: Style.baseButton.btnTextColorUp
    property color btnBackColorDown: Style.baseButton.btnBackColorDown
    property color btnBackColorUp: Style.baseButton.btnBackColorUp
    property color btnBorderColorDown: Style.baseButton.btnBorderColorDown
    property color btnBorderColorUp: Style.baseButton.btnBorderColorUp

    id: root

    text: buttonText

    contentItem: Text {
        id: _text
        text: root.text
        font {
            family: "SF Mono"
            pointSize: Math.min(parent.height, parent.width) / 7
            bold: true
        }
        opacity: enabled ? 1.0 : 0.3
        color: root.down ? btnTextColorDown : btnTextColorUp
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        id: _background
        anchors.fill: root
        color: root.down ? btnBackColorDown : btnBackColorUp
        opacity: enabled ? 1 : 0.3
        border.color: root.down ? btnBorderColorDown : btnBorderColorUp
        border.width: 5
        radius: 20
    }
}
