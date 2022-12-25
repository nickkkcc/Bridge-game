#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include "clientnetwork.h"

ClientNetwork::ClientNetwork(QObject *parent, QString name, QWebSocket *clientSoc, const QUuid &uuid)
{

    this->name = name;
    this->uuid = uuid;
    this->clientSoc = clientSoc;
    gameTerminatedOnce = false;

    connect(clientSoc, &QWebSocket::disconnected, this, &ClientNetwork::onClientDisconnected);
    connect(clientSoc, &QWebSocket::textMessageReceived, this, &ClientNetwork::rxAll);
}

// Проверить работоспособность.
void ClientNetwork::bidTurn()
{

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "notify_bid_turn";
    QJsonObject data;
    tx["data"] = data;
    txAll(tx);
}

// Проверить работоспособность.
void ClientNetwork::moveTurn()
{

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "notify_move_turn";
    QJsonObject data;
    tx["data"] = data;
    txAll(tx);
}

// Проверить работоспособность.
void ClientNetwork::bidRejected(QString reason)
{

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "notify_bid_rejected";
    QJsonObject data;
    data["reason"] = reason;
    tx["data"] = data;
    txAll(tx);
}

// Проверить работоспособность.
void ClientNetwork::moveRejected(QString reason)
{

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "notify_move_rejected";
    QJsonObject data;
    data["reason"] = reason;
    tx["data"] = data;
    txAll(tx);
}

// Проверить работоспособность.
void ClientNetwork::updateGameState(PlayerGameState state)
{

    QJsonObject gameState;
    state.writeToJson(gameState);

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "update_game_state";
    QJsonObject data;
    data["game_state"] = gameState;
    tx["data"] = data;
    txAll(tx);
}

// Проверить работоспособность.
void ClientNetwork::rxAll(const QString& message)
{

    QJsonObject rx = QJsonDocument::fromJson(message.toUtf8()).object();
    if (rx["id"] == uuid.toString(QUuid::WithoutBraces))
    {

        if (rx["type"] == "create_lobby")
        {

            emit rxCreateLobby(this);
        }

        if (rx["type"] == "exit_lobby")
        {

            emit rxCloseLobby(QUuid(rx["data"].toObject()["lobby_id"].toString()), this);
        }

        if (rx["type"] == "select_team_admin")
        {

            emit rxSelectTeamAdmin((Team)rx["data"].toObject()["team"].toInt(),
                                   rx["data"].toObject()["lobby_id"].toString(), this);
        }
        if (rx["type"] == "select_team")
        {

            emit rxAcceptSelectTeam((Team)rx["data"].toObject()["team"].toInt(),
                                    rx["data"].toObject()["lobby_id"].toString(), this);
        }

        if (rx["type"] == "invite_players")
        {

            QJsonObject data = rx["data"].toObject();
            emit rxInvitePlayers(data["login"].toString(), data["lobby_id"].toString(), this);
        }

        if (rx["type"] == "start_game_players")
        {

            QJsonObject data = rx["data"].toObject();
            emit rxStartGamePlayers(data["lobby_id"].toString(), this);
        }
        if (rx["type"] == "join")
        {

            QJsonObject data = rx["data"].toObject();
            emit rxJoinLobby(data["join"].toBool(), this);
        }

        if (rx["type"] == "accept_invite_players")
        {

            QJsonObject data = rx["data"].toObject();
            emit rxAcceptInvitePlayers(data["lobby_id"].toString(), data["successful"].toBool(), this);
        }

        if (rx["type"] == "move_send")
        {

            rxMoveSelectedDeserialization(rx["data"].toObject()["card"].toObject());
        }

        if (rx["type"] == "bid_send")
        {

            rxBidSelectedDeserialization(rx["data"].toObject()["bid"].toObject());
        }
        // Кейс для случая, если игрок хочет выйти из самой игры. В таком случае игра останавливается у всех.
        // Лобби уничтожается. Очки не сохраняются в базу данных.
        //        if (rx["type"] == "exit_from_game")
        //        {
        //            rxExitFromGame();
        //        }
        // Кейс для приглашения игрока в друзья. Необходимо подтверждение игрока, которого пригласили.
        //        if (rx["type"] == "invite_to_friends")
        //        {
        //            rxInvaiteToFriends();
        //        }
        // Кейс для согласия или отвержения дружбы. Отправляется игроку, который приглашает в друзья.
        // Если ответ положительный игрок записывается в друзья для игрока в базу данных. Оповещение игрока,
        // который пришлашал в друзья.
        // Если ответ отрицательный - оповещение игрока об отказе.
        //        if (rx["type"] == "accept_for_invite_to_friends")
        //        {
        //            rxInvaiteToFriends();
        //        }
    }
}

// Проверить работоспособность.
void ClientNetwork::rxMoveSelectedDeserialization(QJsonObject card)
{

    Card tempCard;
    tempCard.readFromJson(card);
    emit rxMoveSelected(tempCard);
}

// Проверить работоспособность.
void ClientNetwork::rxBidSelectedDeserialization(QJsonObject bid)
{

    Bid tempbid;
    tempbid.readFromJson(bid);
    emit rxBidSelected(tempbid);
}

QVector<ClientNetwork *> &ClientNetwork::getClientFriends()
{
    return clientFriends;
}

void ClientNetwork::setClientFriends(const QVector<ClientNetwork *> &newClientFriends)
{
    clientFriends = newClientFriends;
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

void ClientNetwork::setLobbyOwnerUuid(const QUuid &newLobbyOwnerUuid)
{
    lobbyOwnerUuid = newLobbyOwnerUuid;
}

const QUuid &ClientNetwork::getLobbyOwnerUuid() const
{
    return lobbyOwnerUuid;
}

const bool ClientNetwork::getFinder() const
{

    return finder;
}

QWebSocket *const ClientNetwork::getClientSoc() const
{

    return clientSoc;
}

ClientNetwork::~ClientNetwork()
{

    qInfo() << "Server: client --->" << uuid.toString(QUuid::WithBraces) << "--->" << name << "is disconnected.";
    if (clientSoc)
    {

        clientSoc->abort();
        delete clientSoc;
        clientSoc = nullptr;
    }
}

// Проверить работоспособность.
void ClientNetwork::txAll(QJsonObject data)
{

    QJsonDocument txDoc(data);
    clientSoc->sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
}
