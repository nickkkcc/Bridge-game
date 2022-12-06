#include "clientnetwork.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include <QJsonObject>
#include <QDebug>


ClientNetwork::ClientNetwork(QObject *parent, QString name, QWebSocket *clientSoc, QUuid* uuid)
{
    parent = nullptr;
    this->name = name;
    this->uuid = uuid;
    this->clientSoc = clientSoc;
    gameTerminatedOnce = false;

    connect(clientSoc, &QWebSocket::disconnected, this, &ClientNetwork::onClientDisconnected);
    connect(clientSoc, &QWebSocket::textMessageReceived, this, &ClientNetwork::rxAll);
    //connect(clientSoc, &QAbstractSocket::error, this, &ClientNetwork::socketError);
}

void ClientNetwork::rxAll(const QString& message)
{
    QJsonObject rx = QJsonDocument::fromJson(message.toUtf8()).object();
    if(rx["id"] == uuid->toString(QUuid::WithoutBraces)){

        if(rx["type"] == "create_lobby"){
            emit rxCreateLobby(this);
        }
        if(rx["type"] == "exit_lobby"){
            emit rxCloseLobby(QUuid(rx["data"].toObject()["lobby_id"].toString()));
        }
        if(rx["type"] == "select_team_admin"){
                emit rxSelectTeamAdmin((Team)rx["data"].toObject()["team"].toInt(),
                                  rx["data"].toObject()["lobby_id"].toString(),
                                  this);
        }
        if(rx["type"] == "select_team"){
                emit rxAcceptSelectTeam((Team)rx["data"].toObject()["team"].toInt(),
                                  rx["data"].toObject()["lobby_id"].toString(),
                                  this);
        }

        if(rx["type"] == "invite_players"){
            QJsonObject data = rx["data"].toObject();
            emit rxInvitePlayers(data["login"].toString(),
                                 data["lobby_id"].toString(),
                                 this);
        }

        if(rx["type"] == "start_game_players"){
            QJsonObject data = rx["data"].toObject();
            emit rxStartGamePlayers(data["lobby_id"].toString());
        }

        if(rx["type"] == "exit_lobby"){
            QJsonObject data = rx["data"].toObject();
            emit rxCloseLobby(data["lobby_id"].toString());
        }

        if(rx["type"] == "join"){
            QJsonObject data = rx["data"].toObject();
            emit rxJoinLobby(data["join"].toBool(), this);
        }

        if(rx["type"] == "accept_invite_players"){
            QJsonObject data = rx["data"].toObject();
            emit rxAcceptInvitePlayers(data["lobby_id"].toString(),
                                       data["successful"].toBool(),
                                       this);
        }

    }
}

void ClientNetwork::socketError(QAbstractSocket::SocketError socError)
{
}

void ClientNetwork::rxBidSelected(QJsonObject bidObj)
{
}

void ClientNetwork::rxMoveSelected(QJsonObject moveObj)
{
}

void ClientNetwork::rxMessage(QJsonObject msgObj)
{

}

void ClientNetwork::setFinder(bool newFinder)
{
    finder = newFinder;
}

void ClientNetwork::onClientDisconnected()
{
    clientSoc->abort();
    emit clientDisconnected(this);
}

void ClientNetwork::setLobbyOwnerUuid(QUuid *newLobbyOwnerUuid)
{
    lobbyOwnerUuid = newLobbyOwnerUuid;
}

QUuid *ClientNetwork::getLobbyOwnerUuid() const
{
    return lobbyOwnerUuid;
}


bool ClientNetwork::getFinder() const
{
    return finder;
}

QWebSocket *ClientNetwork::getClientSoc() const
{
    return clientSoc;
}

ClientNetwork::~ClientNetwork()
{
    qInfo() <<  "Client: " << uuid->toString(QUuid::WithBraces) << ", " << name << "is disconnected.";
    delete uuid;
    delete clientSoc;
    uuid = nullptr;
    clientSoc = nullptr;
}
