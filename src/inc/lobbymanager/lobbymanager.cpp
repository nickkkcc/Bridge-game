#include "lobbymanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

LobbyManager::LobbyManager(QObject *parent, int maxLobbyCount, QVector<ClientNetwork *> *const clients,
                           DataBase *const db)
    : QObject(parent)
{
    this->db = db;
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
            delete client;
        }
    }

    for (Lobby *lobby : qAsConst(lobbies))
    {
        if (lobby)
        {
            delete lobby;
        }
    }

    if (timer)
    {
        timer->deleteLater();
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

    for (Lobby *const lobby : qAsConst(lobbies))
    {

        if (lobby->getUuid() == uuidLobby)
        {

            return lobby;
        }
    }
    return nullptr;
}

// Поиск лобби по владельцу лобби.
// Если лобби не найдено, то вовращается nullptr.
Lobby *const LobbyManager::findLobby(ClientNetwork *const owner) const
{
    for (Lobby *const lobby : qAsConst(lobbies))
    {

        if (lobby->getOwner() != nullptr && lobby->getOwner() == owner)
        {

            return lobby;
        }
    }
    return nullptr;
}

// Поиск лобби по по имени игрока.
// Если лобби не найдено, то вовращается nullptr.
Lobby *const LobbyManager::findLobby(QString playerName) const
{

    for (auto lobby : qAsConst(lobbies))
    {

        if (lobby->getPlayerNames().contains(playerName) || lobby->getOwnerName() == playerName)
        {

            return lobby;
        }
    }
    return nullptr;
}

// Функция для нахождения комнаты и восстановления игрока в игре при неожиданном отключении.
Lobby *const LobbyManager::findLobbyFromTempClient(QString playerName) const
{

    for (Lobby *const lobby : lobbies)
    {

        if (lobby->getDisconnectedPlayers().contains(playerName))
        {

            return lobby;
        }
    }
    return nullptr;
}

// Поиск клиента по логину.
// Если клиент не найден, то вовращается nullptr.
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

// Поиск клиента по временному идентивикатору.
// Если клиент не найден, то вовращается nullptr.
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

bool LobbyManager::setReturnedClient(Lobby *const lobby, PlayerPosition position, ClientNetwork *const client)
{
    if (lobby)
    {
        if (lobby->getPlayers()[position])
        {

            qInfo() << "Server: setReturnedClient ---> position in vector isn't nullptr";
            return false;
        }
        else if (position == NONE_POSITION)
        {

            qInfo() << "Server: setReturnedClient ---> position is" << Lobby::playerPositionToString(position);
            return false;
        }

        lobby->getPlayers()[position] = client;
        lobby->getPlayerNames()[position] = client->getName();
        client->setPosition(position);
        client->setTeam(client->getTeam(position));
        switch (position)
        {
        case NORTH:
        case SOUTH:
            lobby->setFreeSpotsNS(lobby->getFreeSpotsNS() - 1);
            break;
        case EAST:
        case WEST:
            lobby->setFreeSpotsEW(lobby->getFreeSpotsEW() - 1);
            break;
        case NONE_POSITION:
            break;
        }

        lobby->setSize(lobby->getSize() + 1);
        connect(client, &ClientNetwork::rxMoveSelected, lobby, &Lobby::rxMoveSelected);
        connect(client, &ClientNetwork::rxBidSelected, lobby, &Lobby::rxBidSelected);
        lobby->getDisconnectedPlayers().remove(client->getName());
        if (lobby->getOwnerName() == client->getName())
        {

            client->setLobbyOwnerUuid(lobby->getUuid());
        }
        return true;
    }

    qInfo() << "Server: setReturnedClient ---> lobby is nullptr";
    return false;
}

// функция создания лобби.
void LobbyManager::createLobby(ClientNetwork *const client)
{
    if (client)
    {

        QJsonObject tx;
        QJsonObject data;

        tx["type"] = "create_lobby";
        tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);

        if (getLobbyCount() < maxLobbyCount && maxLobbyCount != 0)
        {
            Lobby *const lobby = new Lobby(this, client);
            lobbies.append(lobby);
            connect(this, &LobbyManager::sendUpdateGameState, lobby, &Lobby::gameEventOccured);
            connect(lobby, &Lobby::sendMatchEndToLM, this, &LobbyManager::matchEnd);
            connect(this, &LobbyManager::sendNextPlayerTurn, lobby, &Lobby::nextPlayerTurn);
            connect(lobby, &Lobby::sendHistoryToLM, this, &LobbyManager::updateHistory);
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
// Функция закрытия лобби. Эта функция используется только в том случае,
// если клиенты иизъявили желание выйти из
// комнаты. Это равносильно тому, что:
// 1. Если эта функция вызывается админом вне начала игры или
// во время игры - Лобби удаляется, а игроки выходят из лобби.
// 2. Если клиент выходит из игры
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

                if (lobby->getGameStarted() || lobby->getOwner() == sender)
                {
                    data["successful"] = true;
                    data["error"] = "";
                    tx["data"] = data;
                    for (ClientNetwork *const client : qAsConst(lobby->getPlayers()))
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
                                    << "--->" << client->getName() << "exit from lobby {"
                                    << "game started:" << lobby->getGameStarted() << "}";

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
                                    << "exit from lobby (as temp client) {"
                                    << "game started:" << lobby->getGameStarted() << "}";
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
                    else if (lobby->getTempPlayers().remove(sender))
                    {
                        data["successful"] = true;
                        data["error"] = "";
                        tx["data"] = data;
                        tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                        qInfo() << "Server: client --->" << sender->getUuid().toString() << "--->" << sender->getName()
                                << "exit from lobby (as tempclient) --->" << uuidLobby.toString();
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
                tempLobby->getTempPlayers().remove(client);

                qInfo() << "Lobby:" << tempLobby->getUuid().toString()
                        << "{ temp:" << tempLobby->getTempPlayers().count()
                        << ", clients:" << tempLobby->getAcceptedPlayerCount() << "}";
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
                data["error"] = "Выбранная вами команда уже заполнена!";
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
                sendClient->setFinder(false);
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
                    data["error"] =
                        "Недостаточно игроков для начала игры: {" + QString::number(tempLobby->getSize()) + " : 4}.";
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
                    invitePlayersAliases.append(client->getAlias().toString(QUuid::WithoutBraces));
                }
            }

            for (Lobby *lobby : qAsConst(lobbies))
            {

                if (lobby->getOwner() != nullptr && !lobby->getGameStarted() && lobby->getPlayerCount() >= 1)
                {

                    // Доделать фичу с списком друзей.
                    QStringList inviteFriendsLogins;
                    QStringList inviteFriendsAliases;

                    for (ClientNetwork *const client : *clients)
                    {
                        if (lobby->getOwner()->getName() != client->getName())
                        {
                            if (client->getFinder() &&
                                lobby->getOwner()->getClientFriendLogins().contains(client->getName()))
                            {
                                inviteFriendsLogins.append(client->getName());
                                inviteFriendsAliases.append(client->getAlias().toString(QUuid::WithoutBraces));
                            }
                        }
                    }

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

// Сигнал от определенного лобби о том, что игра закончена. Можно закрывать лобби.
void LobbyManager::matchEnd(Lobby *const sender)
{

    if (sender)
    {

            closeLobby(sender->getUuid(), sender->getOwner());
    }
}

void LobbyManager::updateHistory(const History &history)
{
    Lobby *const lobby = qobject_cast<Lobby *>(sender());
    if (lobby)
    {
            int winnerTeam = history.getWinner_team();
            for (ClientNetwork *const client : qAsConst(lobby->getPlayers()))
            {
                if (winnerTeam == client->getTeam())
                {
                    db->addHistory(client, history, true);
                }
                else
                {
                    db->addHistory(client, history, false);
                }
            }
    }
}

void LobbyManager::startTimer()
{

    timer->start();
}

// Функция обработки, когда клиент неожиданно отключился (не через кнопку выйти).
void LobbyManager::clientDisconnected(ClientNetwork *const sender)
{

    if (sender)
    {

            Lobby *const tempLobby = findLobby(sender->getName());
            if (tempLobby)
            {
                // Если игра еще не началась.
                if (!tempLobby->getGameStarted())
                {
                    tempLobby->deletePlayer(sender->getTeam(), sender);
                    // Если игрок админ лобби.
                    if (tempLobby->getOwner() == sender)
                    {
                        // Оповещаем клиентов, что лобби закрыто.
                        QJsonObject tx;
                        QJsonObject data;
                        tx["type"] = "exit_lobby";
                        data["lobby_id"] = tempLobby->getUuid().toString(QUuid::WithoutBraces);
                        data["successful"] = true;
                        data["error"] = "";
                        tx["data"] = data;
                        for (ClientNetwork *const client : qAsConst(tempLobby->getPlayers()))
                        {
                            if (client)
                            {

                                tx["id"] = client->getUuid().toString(QUuid::WithoutBraces);
                                qInfo() << "Admin:"
                                        << "close lobby" << tempLobby->getUuid().toString() << "--->"
                                        << client->getUuid().toString() << "--->" << client->getName()
                                        << "exit from lobby (admin disconnected)";

                                txAll(tx, client);
                            }
                        }

                        for (ClientNetwork *tempClient : qAsConst(tempLobby->getTempPlayers()))
                        {
                            if (tempClient)
                            {

                                tempClient->setFinder(false);
                                tx["id"] = tempClient->getUuid().toString(QUuid::WithoutBraces);
                                qInfo() << "Admin:"
                                        << "close lobby" << tempLobby->getUuid().toString() << "--->"
                                        << tempClient->getUuid().toString() << "--->" << tempClient->getName()
                                        << "exit from lobby (as temp client) (admin disconnected)";
                                txAll(tx, tempClient);
                            }
                        }
                        lobbyClose(tempLobby);
                    }
                }
                else
                {
                    // Если количество игроков в игре == 0, то
                    // рушим лобби. Очки не сохраняются.
                    int lobbySize = tempLobby->getSize() - 1;

                    if (lobbySize == 0)
                    {
                        qInfo() << "Server: lobby --->" << tempLobby->getUuid().toString()
                                << "---> closed (all clients disconnected)";
                        lobbyClose(tempLobby);
                    }
                    else
                    {

                        tempLobby->getDisconnectedPlayers().insert(sender->getName(), sender->getPosition());
                        tempLobby->deletePlayer(sender->getTeam(), sender);
                        emit sendUpdateGameState(PLAY_STOP);
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
                        lobby->getTempPlayers().insert(sender);
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
                        sender->setFinder(true);
                        txAll(tx, sender);
                    }
                }
                else
                {

                    data["successful"] = false;
                    data["error"] = "Этого лобби больше не существует!";
                    tx["data"] = data;
                    txAll(tx, sender);
                }
            }
    }
}

void LobbyManager::addFriend(QString login, ClientNetwork *const sender)
{
    if (sender)
    {
                    QJsonObject tx;
                    QJsonObject data;
                    tx["type"] = "add_friend";
                    tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
                    if (db->addFriend(sender, login))
                    {

                sender->setClientFriendLogins(db->getFriendsList(sender));
                data["successful"] = true;
                data["error"] = "";
                qInfo() << "Server: client --->" << sender->getUuid().toString() << "--->" << sender->getName()
                        << " add client --->" << login << "to friend";
                    }
                    else
                    {

                data["successful"] = false;
                data["error"] = "Данный клиент уже в друзьях!";
                qInfo() << "Server: client --->" << sender->getUuid().toString() << "--->" << sender->getName()
                        << "try to add client --->" << login << "to friend (fail)";
                    }
                    tx["data"] = data;
                    txAll(tx, sender);
                    return;
    }
}

void LobbyManager::deleteFriend(QString login, ClientNetwork *const sender)
{
    if (sender)
    {
            QJsonObject tx;
            QJsonObject data;
            tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
            tx["type"] = "delete_friend";

            if (db->deleteFriend(login, sender))
            {
                data["successful"] = true;
                data["error"] = "";
                qInfo() << "Server: client" << sender->getUuid().toString(QUuid::WithoutBraces) << "--->"
                        << sender->getName() << "deleted friend --->" << login;
                sender->setClientFriendLogins(db->getFriendsList(sender));
            }
            else
            {
                data["successful"] = false;
                data["error"] = "Такого клиента нет в базе данных!";
                qInfo() << "Server: client" << sender->getUuid().toString(QUuid::WithoutBraces) << "--->"
                        << sender->getName() << "couldn't delete friend --->" << login;
            }
            tx["data"] = data;
            txAll(tx, sender);
    }
}

void LobbyManager::requestHistoryList(ClientNetwork *const sender)
{
    QJsonObject tx;
    QJsonObject data;
    tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
    tx["type"] = "request_history_list";
    data["history_list"] = db->getHistory(sender);
    tx["data"] = data;
    txAll(tx, sender);
}

void LobbyManager::deleteAccount(ClientNetwork *const sender)
{
    if (sender)
    {
            QJsonObject tx;
            QJsonObject data;
            tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
            tx["type"] = "delete_account";
            if (db->deleteClient(sender))
            {
                data["successful"] = true;
                data["error"] = "";
                qInfo() << "Server: client" << sender->getUuid().toString() << "--->" << sender->getName()
                        << "deleted account.";
            }
            else
            {
                data["successful"] = false;
                data["error"] = "Такого аккаунта нет в базе данных!";
                qInfo() << "Server: client" << sender->getUuid().toString() << "--->" << sender->getName()
                        << "couldn't delete account.";
            }
            tx["data"] = data;
            txAll(tx, sender);
    }
}

void LobbyManager::requestScore(ClientNetwork *const sender)
{
    if (sender)
    {
            QJsonObject tx;
            QJsonObject data;
            tx["id"] = sender->getUuid().toString(QUuid::WithoutBraces);
            tx["type"] = "request_score";
            data["score"] = QString::number(sender->getScore());
            data["all_game_count"] = QString::number(sender->getAllGameCount());
            data["win_game_count"] = QString::number(sender->getWinGameCount());
            tx["data"] = data;
            txAll(tx, sender);
    }
}

void LobbyManager::lobbyClose(Lobby *const lobby)
{

    qInfo() << "Lobby:" << lobby->getUuid().toString() << "is closed.";
    lobbies.removeAll(lobby);
    lobby->deleteLater();
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
