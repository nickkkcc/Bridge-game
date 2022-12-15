QT += core
QT += gui
QT += network
QT += sql
QT += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    inc/database/database.cpp \
    inc/game/player.cpp \
    inc/game/aboutCard/bid.cpp \
    inc/game/aboutCard/card.cpp \
    inc/game/aboutCard/cardkit.cpp \
    inc/game/aboutGameState/gamestate.cpp \
    inc/game/aboutGameState/playergamestate.cpp \
    inc/game/aboutGameState/score.cpp \
    inc/game/aboutGameState/servergamestate.cpp \
    inc/lobbymanager/lobby.cpp \
    inc/lobbymanager/lobbymanager.cpp \
    inc/main.cpp \
    inc/messagehandler/msghandler.cpp \
    inc/network/clientnetwork.cpp \
    inc/serverclass/server.cpp \
    inc/network/servernetwork.cpp

HEADERS += \
    inc/enumeration/Enumiration.h \
    inc/game/player.h \
    inc/game/aboutCard/bid.h \
    inc/game/aboutCard/card.h \
    inc/game/aboutCard/cardkit.h \
    inc/game/aboutGameState/gamestate.h \
    inc/game/aboutGameState/playergamestate.h \
    inc/game/aboutGameState/score.h \
    inc/game/aboutGameState/servergamestate.h \
    inc/lobbymanager/lobby.h \
    inc/lobbymanager/lobbymanager.h \
    inc/messagehandler/MsgType.h \
    inc/database/database.h \
    inc/messagehandler/msghandler.h \
    inc/network/clientnetwork.h \
    inc/serverclass/server.h \
    inc/network/servernetwork.h

FORMS += \
    inc/serverclass/server.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../QxEntityEditor/cpp/bin/ -lbridgeServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../QxEntityEditor/cpp/bin/ -lbridgeServerd
else:unix: LIBS += -L$$PWD/../../../../../../../QxEntityEditor/cpp/bin/ -lbridgeServer

INCLUDEPATH += $$PWD/../../../../../../../QxEntityEditor/cpp/include
DEPENDPATH += $$PWD/../../../../../../../QxEntityEditor/cpp/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../QT/libraries/QxOrm/lib/ -lQxOrm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../QT/libraries/QxOrm/lib/ -lQxOrmd
else:unix: LIBS += -L$$PWD/../../../../../../../QT/libraries/QxOrm/lib/ -lQxOrm

INCLUDEPATH += $$PWD/../../../../../../../QT/libraries/QxOrm/include
DEPENDPATH += $$PWD/../../../../../../../QT/libraries/QxOrm/include
