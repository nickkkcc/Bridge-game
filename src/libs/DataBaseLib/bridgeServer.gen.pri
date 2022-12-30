#################################################################################################
## File created by QxEntityEditor 1.2.6 (2022/12/29 03:00) : please, do NOT modify this file ! ##
#################################################################################################

!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
PRECOMPILED_HEADER += $$PWD/include/bridgeServer_precompiled_header.gen.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

HEADERS += $$PWD/include/bridgeServer_precompiled_header.gen.h
HEADERS += $$PWD/include/bridgeServer_export.gen.h

HEADERS += $$PWD/include/Friends.gen.h
HEADERS += $$PWD/include/History.gen.h
HEADERS += $$PWD/include/Users.gen.h

HEADERS += $$PWD/include/QuestionsType.gen.h

contains(DEFINES, _QX_UNITY_BUILD) {

SOURCES += $$PWD/src/bridgeServer_all.gen.cpp

} else {

SOURCES += $$PWD/src/Friends.gen.cpp
SOURCES += $$PWD/src/History.gen.cpp
SOURCES += $$PWD/src/Users.gen.cpp

SOURCES += $$PWD/src/QuestionsType.gen.cpp

} # contains(DEFINES, _QX_UNITY_BUILD)

include($$PWD/custom/bridgeServer.pri)
