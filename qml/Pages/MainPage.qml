import QtQuick 2.15
import QtQuick.Controls 2.15
import Style 1.0
import Pages 1.0
import Buttons 1.0

Page {
    id: root
    visible: false
    property string time: "0"
    background: BaseBackground {}

    header: MainPageHeader {
        anchors.top: parent.top
        anchors.topMargin: Style.registerPopup.margins
        id: _header
    }

    MainPageButtonGroup {
        id: btnGroup
        anchors.centerIn: parent

        height: parent.height / 2.5
        width: parent.width / 5
    }

    Component.onCompleted: {
        visible: true
    }
}
