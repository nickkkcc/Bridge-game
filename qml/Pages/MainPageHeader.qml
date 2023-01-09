import QtQuick 2.15
import Style 1.0

Rectangle {
    id: root
    width: parent.width
    height: parent.height / Style.mainPage.header.partheaderHeight
    radius: Style.radius
    color: Style.mainPage.header.backColor
    border.width: Style.mainPage.header.borderWidth
    border.color: Style.mainPage.header.borderColor

    Image {
        id: pingImage
        source: Style.mainPage.header.pingImage
        sourceSize.height: parent.height
        sourceSize.width: parent.height
        anchors {
            left: parent.left
            top: parent.top
            verticalCenter: parent.verticalCenter
            margins: Style.mainPage.header.margins
        }
    }

    Text {
        id: pingText
        height: parent.height
        text: qsTr("Пинг: " + client.ping + " мс")
        font {
            family: "SF Mono"
            pointSize: Math.min(parent.height, parent.width) / 6
            bold: true
        }
        color: Style.mainPage.header.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        anchors {
            left: pingImage.right
            top: parent.top
            verticalCenter: parent.verticalCenter
            margins: Style.mainPage.header.margins
        }
    }

    Text {
        id: statusText
        height: parent.height
        text: qsTr("| Статус: ")
        font {
            family: "SF Mono"
            pointSize: Math.min(parent.height, parent.width) / 6
            bold: true
        }
        color: Style.mainPage.header.textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        anchors {
            left: pingText.right
            top: parent.top
            verticalCenter: parent.verticalCenter
            margins: Style.mainPage.header.margins
        }
    }
    Image {
        id: statusImage
        visible: client.statusVisible
        source: client.status ? Style.mainPage.header.statusGoodImage : Style.mainPage.header.statusNegativeImage
        height: parent.height / 2
        width: parent.height / 2
        anchors {
            left: statusText.right
            top: parent.top
            verticalCenter: parent.verticalCenter
            margins: Style.mainPage.header.margins
        }
    }
    Text {
        id: messageText
        height: parent.height
        text: qsTr("| Message: " + client.error)
        font {
            family: "SF Mono"
            pointSize: Math.min(parent.height, parent.width) / 6
            bold: true
        }
        color: Style.mainPage.header.textColor
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        elide: Text.ElideRight
        anchors {
            left: statusImage.right
            top: parent.top
            verticalCenter: parent.verticalCenter
            margins: Style.mainPage.header.margins
        }
    }
}
