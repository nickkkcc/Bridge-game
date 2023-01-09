import QtQuick 2.15

Image {
    id: startImage
    width: parent.width / 2
    height: parent.height / 3
    anchors {
        top: parent.top
        topMargin: parent.height / 8
        horizontalCenter: parent.horizontalCenter
    }
    source: "qrc:/resources/others/starticon.svg"
}
