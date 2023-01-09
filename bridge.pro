QT += quick \
    websockets \
    core

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/findlobbymanager.cpp \
        src/mainclient.cpp \
        src/playerslist.cpp \
        src/socket.cpp

HEADERS += \
    inc/findlobbymanager.h \
        inc/flags.h \
        inc/mainclient.h \
        inc/playerslist.h \
        inc/socket.h \

RESOURCES += qml.qrc \




# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/./qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    main.qml \
    qml/Buttons/BaseButton.qml \
    qml/Buttons/MainPageButtonGroup.qml \
    qml/Buttons/PrelobbyButtonGroup.qml \
    qml/Buttons/StartPageButtonGroup.qml \
    qml/Buttons/qmldir \
    qml/Images/StartPageImage.qml \
    qml/Images/qmldir \
    qml/Pages/BaseBackground.qml \
    qml/Pages/FindLobbyPopup.qml \
    qml/Pages/LobbyPopup.qml \
    qml/Pages/MainPage.qml \
    qml/Pages/MainPageHeader.qml \
    qml/Pages/PopupHeader.qml \
    qml/Pages/PrelobbyPage.qml \
    qml/Pages/StartPage.qml \
    qml/Pages/forgottenPaswordPopup.qml \
    qml/Pages/loginPopup.qml \
    qml/Pages/qmldir \
    qml/Pages/registerPopup.qml \
    qml/Style/Style.qml \
    qml/Style/qmldir \
    resources/backs/1.svg \
    resources/backs/2.svg \
    resources/backs/3.svg \
    resources/backs/4.svg \
    resources/backs/5.svg \
    resources/backs/6.svg \
    resources/bids/1NT.svg \
    resources/bids/2NT.svg \
    resources/bids/3NT.svg \
    resources/bids/4NT.svg \
    resources/bids/5NT.svg \
    resources/bids/6NT.svg \
    resources/bids/7NT.svg \
    resources/bids/B1C.svg \
    resources/bids/B1D.svg \
    resources/bids/B1H.svg \
    resources/bids/B1S.svg \
    resources/bids/B2C.svg \
    resources/bids/B2D.svg \
    resources/bids/B2H.svg \
    resources/bids/B2S.svg \
    resources/bids/B3C.svg \
    resources/bids/B3D.svg \
    resources/bids/B3H.svg \
    resources/bids/B3S.svg \
    resources/bids/B4C.svg \
    resources/bids/B4D.svg \
    resources/bids/B4H.svg \
    resources/bids/B4S.svg \
    resources/bids/B5C.svg \
    resources/bids/B5D.svg \
    resources/bids/B5H.svg \
    resources/bids/B5S.svg \
    resources/bids/B6C.svg \
    resources/bids/B6D.svg \
    resources/bids/B6H.svg \
    resources/bids/B6S.svg \
    resources/bids/B7C.svg \
    resources/bids/B7D.svg \
    resources/bids/B7H.svg \
    resources/bids/B7S.svg \
    resources/bids/BNTB.svg \
    resources/bids/BNTR.svg \
    resources/cards/10C.svg \
    resources/cards/10D.svg \
    resources/cards/10H.svg \
    resources/cards/10S.svg \
    resources/cards/2C.svg \
    resources/cards/2D.svg \
    resources/cards/2H.svg \
    resources/cards/2S.svg \
    resources/cards/3C.svg \
    resources/cards/3D.svg \
    resources/cards/3H.svg \
    resources/cards/3S.svg \
    resources/cards/4C.svg \
    resources/cards/4D.svg \
    resources/cards/4H.svg \
    resources/cards/4S.svg \
    resources/cards/5C.svg \
    resources/cards/5D.svg \
    resources/cards/5H.svg \
    resources/cards/5S.svg \
    resources/cards/6C.svg \
    resources/cards/6D.svg \
    resources/cards/6H.svg \
    resources/cards/6S.svg \
    resources/cards/7C.svg \
    resources/cards/7D.svg \
    resources/cards/7H.svg \
    resources/cards/7S.svg \
    resources/cards/8C.svg \
    resources/cards/8D.svg \
    resources/cards/8H.svg \
    resources/cards/8S.svg \
    resources/cards/9C.svg \
    resources/cards/9D.svg \
    resources/cards/9H.svg \
    resources/cards/9S.svg \
    resources/cards/AC.svg \
    resources/cards/AD.svg \
    resources/cards/AH.svg \
    resources/cards/AS.svg \
    resources/cards/JC.svg \
    resources/cards/JD.svg \
    resources/cards/JH.svg \
    resources/cards/JS.svg \
    resources/cards/KC.svg \
    resources/cards/KD.svg \
    resources/cards/KH.svg \
    resources/cards/KS.svg \
    resources/cards/QC.svg \
    resources/cards/QD.svg \
    resources/cards/QH.svg \
    resources/cards/QS.svg \
    resources/others/account.svg \
    resources/others/friend.svg \
    resources/others/ping.svg \
    resources/others/starticon.svg \
    resources/others/status_good.svg \
    resources/others/status_negative.svg


