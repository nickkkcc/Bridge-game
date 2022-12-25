#include "lobbymanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

LobbyManager::LobbyManager(QObject *parent, int maxLobbyCount, QVector<ClientNetwork *> *const clients)
    : QObject(parent)
{

    this->maxLobbyCount = maxLobbyCount;
    this->clients = clients;
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, this, &LobbyManager::onPlayerCount);
    connect(timer, &QTimer::timeout, this, &LobbyManager::onPlayersOnline);
}

LobbyManager::~LobbyManager()
{

    for (int var = 0; var < clients->count(); ++var)
    {
        ClientNetwork *client = clients->takeAt(var);
        if (client)
        {
            client = nullptr;
            delete client;
        }
    }

    for (Lobby *lobby : qAsConst(lobbies))
    {
        if (lobby)
        {
            lobby = nullptr;
            delete lobby;
        }
    }

    if (timer)
    {
        timer = nullptr;
        delete timer;
    }
    qInfo() << "Server: LobbyManager ---> deleted.";
}

// Функция возвращает кол-во лобби на сервере.
const int LobbyManager::getLobbyCount() const
{
    return lobbies.count();
}

// Поиск лобби по id лобби.
Lobby *const LobbyManager::findLobby(QUuid uuidLobby) const
{

    for (auto lobby : qAsConst(lobbies))
    {

        if (lobby->getUuid() == uuidLobby)
        {

            return lobby;
        }
    }
    return nullptr;
}

// Поиск лобби по владельцу комнаты.
Lobby *const LobbyManager::findLobby(ClientNetwork *const owner) const
{
    for (auto lobby : qAsConst(lobbies))
    {

        if (lobby->getOwner() != nullptr && lobby->getOwner() == owner)
        {

            return lobby;
        }
    }
    return nullptr;
}

// Поиск лобби по по имени игрока.
Lobby *const LobbyManager::findLobby(QString playerName) const
{

    for (auto lobby : qAsConst(lobbies))
    {

        if (lobby->getPlayerNames().contains(playerName))
        {

            return lobby;
        }
    }
    return nullptr;
}

ClientNetwork *const LobbyManager::findPlayer(QString playerName) const
{

    for (auto client : qAsConst(*clients))
    {

        if (client->getName() == playerName)
        {

            return client;
        }
    }
    return nullptr;
}

ClientNetwork *const LobbyManager::findPlayer(QUuid playerUuid) const
{

    for (auto client : qAsConst(*clients))
    {

        if (client->getUuid().toString(QUuid::WithoutBraces) == playerUuid.toString(QUuid::WithoutBraces))
        {

            return client;
        }
    }
    return nullptr;
}

// функция создания лобби.
void LobbyManager::createLobby(ClientNetwork *const client)
{
    if (client != nullptr)
    {

        QJsonObject tx;
        QJsonObject data;

        tx["type"] = "create_lobby";
        tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);

        if (getLobbyCount() < maxLobbyCount && maxLobbyCount != 0)
        {

            lobbies.append(new Lobby(this, client));

            client->setLobbyOwnerUuid(lobbies.last()->getUuid());
            lobbies.last()->setOwnerName(client->getName());

                data["successful"] = true;
                data["error"] = "";
                data["lobby_id"] = lobbies.last()->getUuid().toString(QUuid::WithoutBraces);
                qInfo() << "Client:" << client->getUuid().toString() << "--->" << client->getName()
                        << "created lobby --->" << client->getLobbyOwnerUuid().toString();
        }
        else
        {
                data["successful"] = false;
                data["error"] = "The maximum number of lobbies that can be created has been exceeded";
                data["lobby_id"] = "0";
                qInfo() << "Client:" << client->getUuid().toString() << "--->" << client->getName()
                        << "tried created lobby --->"
                        << "fail";
        }
            tx["data"] = data;
            txAll(tx, client);
    }
}

// Функция удаления лобби со сервера.
void LobbyManager::closeLobby(const QUuid &uuidLobby, ClientNetwork *const sender)
{
    if (sender)
    {
            Lobby *const lobby = findLobby(uuidLobby);
            QJsonObject tx;
            QJsonObject data;
            tx["type"] = "exit_lobby";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            if (lobby)
            {

                if (lobby->getOwner() == sender)
                {
                    data["successful"] = true;
                    data["error"] = "";
                    tx["data"] = data;
                    for (ClientNetwork *client : qAsConst(lobby->getPlayers()))
                    {
                        if (client)
                        {
                            if (client == lobby->getOwner())
                            {
                                client->setLobbyOwnerUuid(QUuid());
                            }

                            client->setFinder(false);
                            tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);
                            qInfo() << "Admin:"
                                    << "close lobby" << uuidLobby.toString() << "--->" << client->getUuid().toString()
                                    << "--->" << client->getName() << "exit from lobby.";

                            txAll(tx, client);
                        }
                    }

                    for (ClientNetwork *tempClient : qAsConst(lobby->getTempPlayers()))
                    {
                        if (tempClient)
                        {

                            tempClient->setFinder(false);
                            tx["id"] = tempClient->getUuid().toString(QUuid::WithoutBraces);
                            qInfo() << "Admin:"
                                    << "close lobby" << uuidLobby.toString() << "--->"
                                    << tempClient->getUuid().toString() << "--->" << tempClient->getName()
                                    << "exit from lobby (as temp client).";
                            txAll(tx, tempClient);
                        }
                    }
                    lobbyClose(lobby);
                }
                else
                {

                    if (lobby->deletePlayer(sender->getTeam(), sender))
                    {

                        sender->setTeam(Team::NONE_TEAM);
                        data["successful"] = true;
                        data["error"] = "";
                        tx["data"] = data;
                        tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                        qInfo() << "Server: client --->" << sender->getUuid().toString() << "--->" << sender->getName()
                                << "exit from lobby --->" << uuidLobby.toString();
                    }
                    else
                    {

                        data["successful"] = false;
                        data["error"] = "Failed to close the lobby (no such player in this team), please try again";
                        tx["data"] = data;
                        tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                    }
                    txAll(tx, sender);
                }
            }
            else
            {
                data["successful"] = false;
                data["error"] = "Failed to close the lobby (lobby not found), please try again";
                tx["data"] = data;
                tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                txAll(tx, sender);
            }
    }
}

// Оповещения игрока, который выбрал команду о том, что он добавлен в эту команду.
// Игрок переходит из сосотяния временных клиентов лобби в действующие игроки.
// Если команда уже переполнена повторно вызывается sendSelectTeamClient(...)
void LobbyManager::acceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork *const client)
{
    Lobby *const tempLobby = findLobby(uuidLobby);

    if (tempLobby)
    {

            QJsonObject tx;
            QJsonObject data;
            tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);
            if (tempLobby->addPlayer(team, client))
            {
                tempLobby->getTempPlayers().removeAll(client);

                client->setTeam(team);
                client->setFinder(false);
                tx["type"] = "accept_select_team";
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                data["successful"] = true;
                data["error"] = "";
                tx["data"] = data;
                qInfo() << "Client:" << client->getUuid().toString() << "--->" << client->getName()
                        << "joined to lobby --->" << tempLobby->getUuid().toString();
                txAll(tx, client);
            }
            else
            {

                tx["type"] = "accept_select_team";
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                data["successful"] = false;
                data["error"] = "Error";
                tx["data"] = data;
                txAll(tx, client);
                sendSelectTeamClient(uuidLobby, client);
            }
    }
}

// Функция добавления в определенную команду для админа лобби.
void LobbyManager::selectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork *const sender)
{
    if (sender)
    {

            Lobby *const tempLobby = findLobby(uuidLobby);
            QJsonObject tx;
            QJsonObject data;
            tx["type"] = "accept_select_team";
            tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

            if (tempLobby && tempLobby->addPlayer(team, sender))
            {

                sender->setTeam(team);
                sender->setFinder(false);

                data["error"] = "";
                data["successful"] = true;
                qInfo() << "Admin:" << sender->getUuid().toString() << "--->" << sender->getName()
                        << "is joined to lobby --->" << tempLobby->getUuid().toString();
            }
            else
            {

                data["error"] = "Something went wrong when adding to the team.";
                data["successful"] = false;
            }
            tx["data"] = data;
            txAll(tx, sender);
    }
}

// Функция оповещение клиентов, которых хотят пригласить в лобби.
void LobbyManager::invitePlayers(QString login, QUuid uuidLobby, ClientNetwork *const sender)
{
    if (sender)
    {

            ClientNetwork *const sendClient = findPlayer(login);
            if (sendClient)
            {

                QJsonObject tx;
                QJsonObject data;
                tx["type"] = "invite_players";
                tx["id"] = sendClient->getUuid().toString(QUuid::WithoutBraces);
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                data["alias"] = sender->getUuid().toString(QUuid::WithoutBraces);
                tx["data"] = data;
                qInfo() << "Admin:" << sender->getUuid().toString() << "--->" << sender->getName()
                        << "invited player --->" << sendClient->getName();
                txAll(tx, sendClient);
            }
    }
}

// Функция оповещения игроков лобби о начале игры.
void LobbyManager::startGame(QUuid uuidLobby, ClientNetwork *const sender)
{
    if (sender)
    {

            Lobby *const tempLobby = findLobby(uuidLobby);
            if (tempLobby)
            {
                QJsonObject tx;
                QJsonObject data;
                tx["type"] = "start_game_players";
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                if (!tempLobby->getPlayers().contains(nullptr))
                {

                    tempLobby->setGameStarted(true);

                    // Отправка оповещений игрокам о том, что игра началась.
                    for (auto client : qAsConst(tempLobby->getPlayers()))
                    {

                        tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);
                        data["successful"] = true;
                        data["error"] = "";
                        tx["data"] = data;
                        txAll(tx, client);
                    }
                    qInfo() << "Lobby:" << uuidLobby.toString() << "---> is started";
                    tempLobby->startMatch();
                }
                else
                {
                    // Отправка сообщения админу комнаты, о том, что начать игру не удалось.
                    tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                    data["successful"] = false;
                    data["error"] = "Not enough players or one of the participants disconnected unexpectedly.";
                    tx["data"] = data;
                    txAll(tx, sender);
                }
            }
    }
}

// Функция отсылки игроков в онлайне.
void LobbyManager::onPlayersOnline()
{

    if (!lobbies.empty())
    {
            // Инициилизируем и заполняем список игроков online.
            QStringList invitePlayersLogins;
            QStringList invitePlayersAliases;

            for (ClientNetwork *client : qAsConst(*clients))
            {
                if (client->getFinder())
                {

                    invitePlayersLogins.append(client->getName());
                    invitePlayersAliases.append(client->getUuid().toString(QUuid::WithoutBraces));
                }
            }

            for (Lobby *lobby : qAsConst(lobbies))
            {

                if (lobby->getOwner() != nullptr && !lobby->getGameStarted() && lobby->getPlayerCount() >= 1)
                {

                    // Доделать фичу с списком друзей.
                    QStringList inviteFriendsLogins;
                    QStringList inviteFriendsAliases;

                    QJsonObject tx;
                    QJsonObject data;
                    QJsonArray arrPlayers;
                    QJsonArray arrFriends;
                    QJsonObject temp;

                    // Заполняем json списком друзей (доделать фичу).
                    for (int i = 0; i < inviteFriendsLogins.count(); i++)
                    {

                        temp["login"] = inviteFriendsLogins[i];
                        temp["alias"] = inviteFriendsAliases[i];
                        arrFriends.append(temp);
                    }

                    // Заполняем json списком игроков online.
                    for (int i = 0; i < invitePlayersLogins.count(); i++)
                    {

                        temp["login"] = invitePlayersLogins[i];
                        temp["alias"] = invitePlayersAliases[i];
                        arrPlayers.append(temp);
                    }

                    data["players"] = arrPlayers;
                    data["friends"] = arrFriends;
                    tx["data"] = data;
                    tx["type"] = "players_online";
                    tx["id"] = lobby->getOwner()->getUuid().toString(QUuid::WithoutBraces);

                    txAll(tx, lobby->getOwner());
                }
            }
    }
}

// Функция отссылки количества игроков в лобби.
void LobbyManager::onPlayerCount()
{

    if (!lobbies.empty())
    {

            for (Lobby *lobby : qAsConst(lobbies))
            {
                if (!lobby->getGameStarted() && lobby->getPlayerCount() >= 1)
                {

                    QJsonObject tx;
                    QJsonObject data;
                    data["lobby_id"] = lobby->getUuid().toString(QUuid::WithoutBraces);
                    data["count"] = lobby->getPlayerCount();
                    data["error"] = "";
                    tx["data"] = data;
                    tx["type"] = "players_count_lobby";
                    for (auto client : qAsConst(lobby->getPlayers()))
                    {

                        if (client != nullptr)
                        {

                            tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);
                            txAll(tx, client);
                        }
                    }
                }
            }
    }
}

void LobbyManager::startTimer()
{

    timer->start();
}

void LobbyManager::clientDisconnected(ClientNetwork *const sender)
{

    if (sender)
    {

            Lobby *const tempLobby = findLobby(sender);
            if (tempLobby)
            {
                // Если во время самой игры админ отключается - лобби не закрывается.
                // Предполагается, что клиенты могут присоединиться, если утратили соединение.
                if (tempLobby->getOwner() == sender)
                {

                    if (tempLobby->getGameStarted())
                    {

                        tempLobby->setOwner(nullptr);
                    }
                    else
                    {

                        closeLobby(tempLobby->getUuid(), sender);
                    }
                }
                else
                {

                    int index = tempLobby->getPlayers().indexOf(sender);

                    // Случай, когда отключается клиент не находящийся в какой-либо лобби.
                    if (index != -1)
                    {

                        tempLobby->getPlayers().replace(index, nullptr);
                    }
                }
            }
            emit disconnectClient(sender);
    }
}

void LobbyManager::joinLobby(bool join, ClientNetwork *const sender)
{

    if (sender)
    {

            sender->setFinder(join);
            QJsonObject tx;
            QJsonObject data;

            tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
            tx["type"] = "join";
            data["successful"] = true;
            tx["data"] = data;

            txAll(tx, sender);
    }
}

// Отправка клиентам, которые согласились зайти в лобби информации о том, что сервер это учел.
// Зачисление данных клиентов в список временных клиентов данного лобби.
void LobbyManager::acceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork *const sender)
{

    if (sender)
    {

            Lobby *const lobby = findLobby(uuidLobby);
            if (lobby)
            {

                QJsonObject tx;
                QJsonObject data;
                tx["type"] = "accept_invite_players";
                tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                if (lobby->getAcceptedPlayerCount() < lobby->getMaxPlayerSize())
                {

                    if (successful)
                    {

                        lobby->setAcceptedPlayerCount(lobby->getAcceptedPlayerCount() + 1);
                        lobby->getTempPlayers().append(sender);
                        data["successful"] = true;
                        data["error"] = "";
                        tx["data"] = data;
                        txAll(tx, sender);
                        sendSelectTeamClient(uuidLobby, sender);
                    }
                    else
                    {

                        data["successful"] = true;
                        data["error"] = "";
                        tx["data"] = data;
                        txAll(tx, sender);
                    }
                }
                else
                {

                    data["successful"] = false;
                    data["error"] = "This lobby is no longer available";
                    tx["data"] = data;
                    txAll(tx, sender);
                }
            }
    }
}

void LobbyManager::lobbyClose(Lobby *const lobby)
{

    qInfo() << "Lobby:" << lobby->getUuid().toString() << "is closed.";
    lobbies.removeAll(lobby);
    delete lobby;
}

void LobbyManager::txAll(QJsonObject tx, ClientNetwork *const sender)
{

    QJsonDocument txDoc(tx);
    sender->getClientSoc()->sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
}

// Функция для отправки клинту, который согласился зайти в лобби данных о занятых местах.
// Клиент по-прежнему находится в списке временных клиентов лобби.
void LobbyManager::sendSelectTeamClient(QUuid uuidLobby, ClientNetwork *const client)
{

    if (client)
    {

            Lobby *const tempLobby = findLobby(uuidLobby);
            if (tempLobby)
            {

                QJsonObject tx;
                QJsonObject data;
                tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);
                tx["type"] = "select_team";
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                data["team_1"] = LobbyManager::maxTeamPLayers - tempLobby->getFreeSpotsNS();
                data["team_2"] = LobbyManager::maxTeamPLayers - tempLobby->getFreeSpotsEW();
                tx["data"] = data;
                txAll(tx, client);
            }
    }
}
