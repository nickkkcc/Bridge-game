pragma Singleton

import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    readonly property alias pagesBack: pagesBack
    readonly property alias baseButton: baseButton
    readonly property alias mainPage: mainPageSection
    readonly property alias loginPopup: loginPopup
    readonly property alias registerPopup: registerPopup
    readonly property real radius: 20
    readonly property string deleteFriend: "qrc:/resources/others/delete_friend.svg"
    readonly property string addFriend: "qrc:/resources/others/add_friend.svg"
    readonly property string invitePlayer: "qrc:/resources/others/invite_to_lobby.svg"
    readonly property string acceptInvite: "qrc:/resources/others/accept_invite.svg"
    readonly property string rejectInvite: "qrc:/resources/others/reject_invite.svg"

    // pages background gradient color
    Item {
        id: pagesBack
        readonly property color pageBackColorS: "#0B3800"
        readonly property color pageBackColorE: "black"
    }

    // base button text  colors
    Item {
        id: baseButton
        readonly property color btnTextColorDown: "#1E5128"
        readonly property color btnTextColorUp: Qt.lighter("#1E5128", 2)
        readonly property color btnBackColorDown: "#283d62"
        readonly property color btnBackColorUp: "#1b2d48"
        readonly property color btnBorderColorDown: "#0a1110"
        readonly property color btnBorderColorUp: "#afbda7"
    }
    // mainPage property
    Item {
        id: mainPageSection
        readonly property color backColor: "#D8E9A8"
        readonly property color borderColor: "#191A19"
        readonly property alias header: headerSection
        Item {
            id: headerSection
            readonly property color backColor: "#F0CAA3"
            readonly property color borderColor: "#191A19"
            readonly property real borderWidth: 5
            readonly property real partheaderHeight: 18
            readonly property string pingImage: "qrc:/resources/others/ping.svg"
            readonly property string statusGoodImage: "qrc:/resources/others/status_good.svg"
            readonly property string statusNegativeImage: "qrc:/resources/others/status_negative.svg"
            readonly property real radius: 15
            readonly property color textColor: "#1E5128"
            readonly property real margins: 10
        }
    }
    Item {
        id: loginPopup
        readonly property real radius: 5
        readonly property color backColor: "#F0CAA3"
        readonly property color borderColor: "#191A19"
        readonly property real borderWidth: 5
        readonly property real partheaderHeight: 10
        readonly property color textColor: "#1E5128"
        readonly property real margins: 10
    }
    Item {
        id: registerPopup
        readonly property real radius: 5
        readonly property color backColor: "#F0CAA3"
        readonly property color borderColor: "#191A19"
        readonly property real borderWidth: 5
        readonly property real partheaderHeight: 10
        readonly property color textColor: "#1E5128"
        readonly property real margins: 10
    }
}
