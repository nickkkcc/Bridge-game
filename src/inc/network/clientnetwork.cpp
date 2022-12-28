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
    //    qInfo() << gameState;
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
        // Кейс для добавления игрока в друзья. Ответ клиенту добавлен ли указанный клиент в друзья
        // в базе данных.
        //        if (rx["type"] == "add_to_friends")
        //        {
        //            rxAddToFriends();
        //        }

        // Кейс для удаления игрока из друзья. Ответ клиенту удален ли указанный клиент из друзей
        // в базе данных.
        //        if (rx["type"] == "delete_from_friends")
        //        {
        //            rxDeleteToFriends();
        //        }

        // Кейс для запроса списка друзей клиента с базы данных.
        // Если запрос выполнен успешно - отправка клиенту списка друзей.
        // Если запрос не выполнен успешно - отправка клиенту причину ошибки.
        //        if (rx["type"] == "request_friends_list")
        //        {
        //            rxRequestFriendsList();
        //        }

        // Кейс для запроса истории игр клиента с базы данных.
        // Если запрос выполнен успешно - отправка клиенту списка сыгранных игр.
        // Если запрос не выполнен успешно - отправка клиенту причину ошибки.
        //        if (rx["type"] == "request_history_list")
        //        {
        //            rxRequestHistoryList();
        //        }

        // Кейс для запроса на удаление клиента из базы данных.
        // Если запрос выполнен успешно - отправка клиенту статус успешного удаления клиента из базы данных.
        // Если запрос не выполнен успешно - отправка клиенту причину ошибки.
        //        if (rx["type"] == "delete_client")
        //        {
        //            rxDeleteClient();
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

void ClientNetwork::setClientSoc(QWebSocket *newClientSoc)
{
    clientSoc = newClientSoc;
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
        clientSoc->deleteLater();
    }
}

// Проверить работоспособность.
void ClientNetwork::txAll(QJsonObject data)
{

    QJsonDocument txDoc(data);
    clientSoc->sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
}
