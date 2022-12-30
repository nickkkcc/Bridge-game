#################################################################################################
## File created by QxEntityEditor 1.2.6 (2022/12/29 03:00) : please, do NOT modify this file ! ##
#################################################################################################

include($$PWD/../../../../Qt/libraries/QxOrm/QxOrm.pri)

TEMPLATE = lib
CONFIG += dll
DEFINES += _BUILDING_BRIDGESERVER
INCLUDEPATH += $$PWD/../../../../Qt/libraries/QxOrm/include
DESTDIR = $$PWD/bin/

CONFIG(debug, debug|release) {
TARGET = bridgeServerd
} else {
TARGET = bridgeServer
} # CONFIG(debug, debug|release)

LIBS += -L"$$PWD/../../../../Qt/libraries/QxOrm/lib"

CONFIG(debug, debug|release) {
LIBS += -l"QxOrmd"
} else {
LIBS += -l"QxOrm"
} # CONFIG(debug, debug|release)

include($$PWD/bridgeServer.gen.pri)

!contains(DEFINES, _QX_UNITY_BUILD) {
SOURCES += $$PWD/src/bridgeServer_main.gen.cpp
} # !contains(DEFINES, _QX_UNITY_BUILD)
