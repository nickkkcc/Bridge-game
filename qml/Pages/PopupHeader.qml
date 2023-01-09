import QtQuick 2.15
import Style 1.0

Rectangle {
    property string headerText: ""
    id: root
    width: parent.width
    height: parent.height / Style.registerPopup.partheaderHeight
    radius: Style.registerPopup.radius
    color: Style.registerPopup.backColor
    border.width: Style.registerPopup.borderWidth
    border.color: Style.registerPopup.borderColor
    anchors.top: parent.top

    Text {
        id: popupText
        height: root.height
        width: root.width / 2
        anchors.fill: root
        text: headerText
        font {
            family: "SF Mono"
            pointSize: Math.min(root.height, root.width) / 3
            bold: true
        }
        color: Style.registerPopup.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
