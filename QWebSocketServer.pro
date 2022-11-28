QT += core
QT += gui
QT += network
QT += sql
QT += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    inc/database/database.cpp \
    inc/main.cpp \
    inc/messagehandler/msghandler.cpp \
    inc/serverclass/server.cpp \
    inc/network/servernetwork.cpp

HEADERS += \
    inc/messagehandler/MsgType.h \
    inc/database/database.h \
    inc/messagehandler/msghandler.h \
    inc/serverclass/server.h \
    inc/network/servernetwork.h

FORMS += \
    inc/serverclass/server.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../QxEntityEditor/cpp/bin/ -lbridgeServer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../QxEntityEditor/cpp/bin/ -lbridgeServerd
else:unix: LIBS += -L$$PWD/../../../../../../QxEntityEditor/cpp/bin/ -lbridgeServer

INCLUDEPATH += $$PWD/../../../../../../QxEntityEditor/cpp/include
DEPENDPATH += $$PWD/../../../../../../QxEntityEditor/cpp/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../QT/libraries/QxOrm/lib/ -lQxOrm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../QT/libraries/QxOrm/lib/ -lQxOrmd
else:unix: LIBS += -L$$PWD/../../../../../../QT/libraries/QxOrm/lib/ -lQxOrm

INCLUDEPATH += $$PWD/../../../../../../QT/libraries/QxOrm/include
DEPENDPATH += $$PWD/../../../../../../QT/libraries/QxOrm/include
