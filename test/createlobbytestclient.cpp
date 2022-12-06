#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QtTest/QTest>

#include "createlobbytestclient.h"
QT_USE_NAMESPACE

CreateLobbyTest::CreateLobbyTest(const QUrl &url, bool debug , QObject *parent , QString name):
    QObject(parent),
    m_url(url),
    m_debug(debug),
    name(name)
{
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &CreateLobbyTest::onConnected);
    m_webSocket.open(QUrl(url));
    currentType = CreateLobbyTest::LOGIN_TRUE;
}

void CreateLobbyTest::sendLogin()
{
    qInfo() << "LoginSend";
    emit send(currentType);
}

void CreateLobbyTest::sendCreateLobbyTrue(bool trueFalse)
{
    qInfo() << "LoginSend";
    if(trueFalse){
        emit send(currentType);
    }else{
        emit send(currentType);
    }
}

void CreateLobbyTest::sendSelectTeam(bool trueFalse)
{
    if(trueFalse){
        send(CreateLobbyTest::SELECT_TEAM_TRUE);
    }else{
         send(CreateLobbyTest::SELECT_TEAM_FALSE);
    }
}

void CreateLobbyTest::sendInvitePlayers(bool trueFalse)
{
    if(trueFalse){
        currentType = CreateLobbyTest::INVITE_PLAYERS_TRUE;
        emit send(currentType);
    }else{
        currentType = CreateLobbyTest::INVITE_PLAYERS_EMPTY;
        emit send(currentType);
    }
}

void CreateLobbyTest::sendStartGamePlayers()
{
    currentType = CreateLobbyTest::START_GAME_PLAYERS;
    emit send(currentType);
}

void CreateLobbyTest::sendExit()
{
    currentType = CreateLobbyTest::EXIT_LOBBY;
    emit send(currentType);
}

void CreateLobbyTest::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &CreateLobbyTest::onTextMessageReceived);
    connect(this, &CreateLobbyTest::send,
            this, &CreateLobbyTest::sendD);
    qInfo() << "Connected";
    emit send(currentType);
}


void CreateLobbyTest::onTextMessageReceived(const QString& message)
{
    checkAnswerFromServer(QJsonDocument::fromJson(message.toUtf8()).object());
}

void CreateLobbyTest::sendD(const Type &type)
{
    QJsonObject m = createJsonD(type);
    QJsonDocument n(m);
    m_webSocket.sendTextMessage(n.toJson(QJsonDocument::JsonFormat::Compact));

}

void CreateLobbyTest::onDo(CreateLobbyTest::Type type)
{
    switch(type){
        case CreateLobbyTest::LOGIN_TRUE:{
            sendLogin();
            break;
        }
        case CreateLobbyTest::CREATE_LOBBY_TRUE:{
            sendCreateLobbyTrue(true);
            break;
        }
        case CreateLobbyTest::SELECT_TEAM_TRUE:{
            sendSelectTeam(true);
            break;
        }
        case CreateLobbyTest::EXIT_LOBBY:{
            sendExit();
            break;
        }
    }
}

void CreateLobbyTest::checkAnswerFromServer(const QJsonObject &rx)
{
    Type oldType = Type::LOGIN_TRUE;
    QJsonObject answer;
    QJsonObject data;
    switch (currentType) {
       case Type::LOGIN_TRUE:{
        uid =  rx["data"].toObject()["token"].toString();
        oldType = currentType;
        currentType = CreateLobbyTest::Type::CREATE_LOBBY_TRUE;
        onDo(currentType);
        break;
    }
       case Type::CREATE_LOBBY_TRUE:{
           lobbyUid = rx["data"].toObject()["lobby_id"].toString();
           data["lobby_id"] = rx["data"].toObject()["lobby_id"];
           data["successful"] = true;
           data["error"] = "";

           answer["data"] = data;
           answer["id"] = uid.toString();
           answer["type"] = "create_lobby";
           oldType = currentType;
           currentType = CreateLobbyTest::Type::SELECT_TEAM_TRUE;
           onDo(currentType);
           break;
       }
       case Type::CREATE_LOBBY_FALSE:{
           data["lobby_id"] = rx["data"].toObject()["lobby_id"];
           data["successful"] = false;
           data["error"] = "The maximum number of lobbies that can be created has been exceeded";

           answer["data"] = data;
           answer["id"] = uid.toString();
           answer["type"] = "create_lobby";
           break;
       }
       case Type::SELECT_TEAM_TRUE:{
           data["lobby_id"] = lobbyUid.toString();
           data["team_1"] = 1;
           data["team_2"] = 2;

           answer["data"] = data;
           answer["id"] = uid.toString();
           answer["type"] = "select_team";
           oldType = currentType;
           currentType = CreateLobbyTest::Type::EXIT_LOBBY;
           onDo(currentType);
           break;
       }
       case Type::SELECT_TEAM_FALSE:{
           data["lobby_id"] = rx["data"].toObject()["lobby_id"];
           data["successful"] = false;
           data["error"] = "The maximum number of lobbies that can be created has been exceeded";

           answer["data"] = data;
           answer["id"] = uid.toString();
           answer["type"] = "create_lobby";
           break;
       }
       case Type::EXIT_LOBBY:{
           data["lobby_id"] = lobbyUid.toString();
           data["successful"] = true;
           data["error"] = "";

           answer["data"] = data;
           answer["id"] = uid.toString();
           answer["type"] = "exit_lobby";
           oldType = currentType;
           break;
       }
    }
    qWarning() << "Test " << oldType << ": " << (rx == answer);
}

QJsonObject CreateLobbyTest::createJsonD(const CreateLobbyTest::Type& name){

    QJsonObject obj;
    QJsonObject data;

    switch (name) {
    case CreateLobbyTest::Type::CREATE_LOBBY_TRUE:{
        obj["data"] = data;
        obj["id"] = uid.toString();
        obj["type"] = "create_lobby";
        return obj;
    }
    case CreateLobbyTest::Type::CREATE_LOBBY_FALSE:{
        obj["data"] = data;
        obj["id"] = uid.toString();
        obj["type"] = "create_lobby";
        return obj;
    }

    case CreateLobbyTest::Type::LOGIN_TRUE:{
        data["login"] = "Kolya";
        data["password"] = "123456";

        obj["id"] = "0";
        obj["type"] = "login";
        obj["data"] = data;
        return obj;
    }

    case CreateLobbyTest::Type::SELECT_TEAM_TRUE:{
        data["lobby_id"] = lobbyUid.toString();
        data["team"] = 0;

        obj["id"] = uid.toString();
        obj["type"] = "select_team";
        obj["data"] = data;
        return obj;
    }
    case CreateLobbyTest::Type::EXIT_LOBBY:{
        data["lobby_id"] = lobbyUid.toString();
        data["successful"] = true;

        obj["id"] = uid.toString();
        obj["type"] = "exit_lobby";
        obj["data"] = data;
        return obj;
    }
    default:return obj;
    }
}

