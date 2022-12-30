#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include "clientnetwork.h"

ClientNetwork::ClientNetwork(QObject *parent, QString name, QWebSocket *clientSoc, const QUuid &uuid,
                             const QUuid &alias)
{

    this->setParent(parent);
    this->name = name;
    this->uuid = uuid;
    this->alias = alias;
    this->clientSoc = clientSoc;

    connect(clientSoc, &QWebSocket::disconnected, this, &ClientNetwork::onClientDisconnected);
    connect(clientSoc, &QWebSocket::textMessageReceived, this, &ClientNetwork::rxAll);
}

void ClientNetwork::bidTurn()
{

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "notify_bid_turn";
    QJsonObject data;
    tx["data"] = data;
    txAll(tx);
}

void ClientNetwork::moveTurn()
{

    QJsonObject tx;
    tx["id"] = uuid.toString(QUuid::WithoutBraces);
    tx["type"] = "notify_move_turn";
    QJsonObject data;
    tx["data"] = data;
    txAll(tx);
}

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
        //         Кейс для добавления игрока в друзья. Ответ клиенту добавлен ли указанный клиент в друзья
        //         в базе данных.
        if (rx["type"] == "add_friend")
        {
            emit rxAddToFriends(rx["data"].toObject()["login"].toString(), this);
        }

        // Кейс для удаления игрока из друзей. Ответ клиенту удален ли указанный клиент из друзей
        // в базе данных.
        if (rx["type"] == "delete_from_friends")
        {
            emit rxDeleteToFriends(rx["data"].toObject()["login"].toString(), this);
        }

        // Кейс для запроса истории игр клиента с базы данных.
        // Если запрос выполнен успешно - отправка клиенту списка сыгранных игр.
        // Если запрос не выполнен успешно - отправка клиенту причину ошибки.
        if (rx["type"] == "request_history_list")
        {
            emit rxRequestHistoryList(this);
        }

        // Кейс для запроса на удаление клиента из базы данных.
        // Если запрос выполнен успешно - отправка клиенту статус успешного удаления клиента из базы данных.
        // Если запрос не выполнен успешно - отправка клиенту причину ошибки.
        //        if (rx["type"] == "delete_client")
        //        {
        //            rxDeleteClient();
        //        }
    }
}

void ClientNetwork::rxMoveSelectedDeserialization(QJsonObject card)
{

    Card tempCard;
    tempCard.readFromJson(card);
    emit rxMoveSelected(tempCard);
}

void ClientNetwork::rxBidSelectedDeserialization(QJsonObject bid)
{

    Bid tempbid;
    tempbid.readFromJson(bid);
    emit rxBidSelected(tempbid);
}

const QHash<QString, QUuid> &ClientNetwork::getClientFriendLogins() const
{
    return clientFriendLogins;
}

void ClientNetwork::setClientFriendLogins(const QHash<QString, QUuid> &newClientFriendLogins)
{
    clientFriendLogins = newClientFriendLogins;
}

const QUuid ClientNetwork::getAlias() const
{
    return alias;
}

void ClientNetwork::setAlias(const QUuid &newAlias)
{
    alias = newAlias;
}

void ClientNetwork::setClientSoc(QWebSocket *newClientSoc)
{
    clientSoc = newClientSoc;
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

QString ClientNetwork::getName() const
{
    return name;
}

PlayerPosition ClientNetwork::getPosition() const
{
    return position;
}

void ClientNetwork::setName(QString name)
{
    this->name = name;
}

const QUuid &ClientNetwork::getUuid() const
{
    return uuid;
}

void ClientNetwork::setTeam(Team newTeam)
{
    team = newTeam;
}

const Team ClientNetwork::getTeam() const
{
    return team;
}

const Team ClientNetwork::getTeam(PlayerPosition position) const
{

    switch (position)
    {
    case NORTH:
    case SOUTH:
        return N_S;
    case EAST:
    case WEST:
        return E_W;
    case NONE_POSITION:
        return NONE_TEAM;
    }
}

void ClientNetwork::setPosition(PlayerPosition position)
{
    this->position = position;
}
