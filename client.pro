

QT += core gui
QT += widgets
QT += websockets
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

DISTFILES += \
    json/accept_invite_players(TC).json \
    json/accept_invite_players(TS).json \
    json/accept_invite_players_to_client.json \
    json/accept_invite_players_to_server.json \
    json/accept_select_team(TC).json \
    json/accept_select_team_to_client.json \
    json/create_lobby(TC).json \
    json/create_lobby(TS).json \
    json/create_lobby_to_client.json \
    json/create_lobby_to_server.json \
    json/exit_lobby(TS).json \
    json/exit_lobby_to_server.json \
    json/invite_players(TC).json \
    json/invite_players(TS).json \
    json/invite_players_to_client.json \
    json/invite_players_to_server.json \
    json/join(TS).json \
    json/join_to_server.json \
    json/login(TC).json \
    json/login(TS).json \
    json/login_to_client.json \
    json/login_to_server.json \
    json/players_count_lobby(TC).json \
    json/players_count_lobby_to_client.json \
    json/players_online(TC).json \
    json/players_online_to_client.json \
    json/registration(TC).json \
    json/registration(TS).json \
    json/registration_questions(TC).json \
    json/registration_questions(TS).json \
    json/registration_questions_to_client.json \
    json/registration_questions_to_server.json \
    json/registration_to_client.json \
    json/registration_to_server.json \
    json/select_team(TC).json \
    json/select_team(TS).json \
    json/select_team_to_client.json \
    json/select_team_to_server.json \
    json/start_game_players(TC).json \
    json/start_game_players_to_client.json \
    json/start_game_players_to_server.json
