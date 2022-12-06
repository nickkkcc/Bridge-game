// Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifndef CREATELOBBYTEST_H
#define CREATELOBBYTEST_H

#include <QUuid>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>

    class CreateLobbyTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateLobbyTest(const QUrl &url, bool debug = false, QObject *parent = nullptr, QString name = "");
    void sendLogin();
    void sendCreateLobbyTrue(bool trueFalse);
    void sendSelectTeam(bool trueFalse);
    void sendInvitePlayers(bool trueFlase);
    void sendStartGamePlayers();
    void sendExit();

        enum Type{
            LOGIN_TRUE,
            CREATE_LOBBY_TRUE,
            CREATE_LOBBY_FALSE,
            SELECT_TEAM_TRUE,
            SELECT_TEAM_FALSE,
            INVITE_PLAYERS_TRUE,
            INVITE_PLAYERS_EMPTY,
            PLAYERS_COUNT_LOBBY,
            PLAYERS_ONLINE,
            START_GAME_PLAYERS,
            EXIT_LOBBY
            };
Q_SIGNALS:
    void closed();
    void send(const Type& type);


private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(const QString& message);
    void sendD(const Type& type);
    void onDo(CreateLobbyTest::Type type);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
    Type currentType;
    QString name;
    QUuid uid;
    QUuid lobbyUid;

    QJsonObject createJsonD(const Type& name);
    void checkAnswerFromServer(const QJsonObject & rx);
};

#endif // CREATELOBBYTEST_H
