

QT   += core gui
greaterThan(QT_MAJOR_VERSION, 4):
QT += widgets
QT += network
CONFIG += c++17
CONFIG += resources_big
CONFIG(release, debug|release):DEFINES += QT_NO_INFO_OUTPUT
SOURCES += \
    main.cpp \
    network/clientnetwork.cpp \
    windows/lobby.cpp \
    windows/entrywindow.cpp \
    windows/newaccount.cpp

HEADERS += \
    network/clientnetwork.h \
    windows/entrywindow.h \
    windows/lobby.h \
    windows/newaccount.h

FORMS += \
    windows/entrywindow.ui \
    windows/lobby.ui \
    windows/newaccount.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
