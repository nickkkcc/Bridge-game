#include "lobbymanager.h"
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"

LobbyManager::LobbyManager(QObject *parent, int maxLobbyCount, QVector<ClientNetwork*> *clients)
    : QObject{parent}
{
    this->maxLobbyCount = maxLobbyCount;
    this->clients = clients;
    timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, &QTimer::timeout, this, &LobbyManager::onPlayerCount);
    connect(timer, &QTimer::timeout, this, &LobbyManager::onPlayersOnline);
}

// Функция возвращает кол-во лобби на сервере.
int LobbyManager::getLobbyCount()
{
    return lobbies.count();
}

// Поиск лобби по id лобби.
Lobby *LobbyManager::findLobby(QUuid uuidLobby)
{
    for (Lobby* lobby : qAsConst(lobbies)) {
        if(lobby->getUuid() == uuidLobby){
            return lobby;
        }
    }
    return nullptr;
}

// Поиск лобби по владельцу комнаты.
Lobby *LobbyManager::findLobby(ClientNetwork *owner)
{
    for (Lobby* lobby : qAsConst(lobbies)) {
        if(lobby->getOwner() != nullptr && lobby->getOwner() == owner){
            return lobby;
        }
    }
    return nullptr;
}

// Поиск лобби по по имени игрока.
Lobby *LobbyManager::findLobby(QString playerName)
{
    for (Lobby* lobby : qAsConst(lobbies)) {
        if(lobby->getPlayerNames().contains(playerName)){
            return lobby;
        }
    }
    return nullptr;
}

ClientNetwork *LobbyManager::findPlayer(QString playerName)
{
    for (ClientNetwork* client : qAsConst(*clients)) {
        if(client->getName() == playerName){
            return client;
        }
    }
    return nullptr;
}

ClientNetwork *LobbyManager::findPlayer(QUuid playerUuid)
{
    for (ClientNetwork* client : qAsConst(*clients)) {
        if(client->getUuid()->toString(QUuid::WithoutBraces) == playerUuid.toString(QUuid::WithoutBraces)){
            return client;
        }
    }
    return nullptr;
}

// функция создания лобби.
void LobbyManager::createLobby(ClientNetwork *client)
{
    if(client != nullptr){
        QJsonObject tx;
        QJsonObject data;

        tx["type"] = "create_lobby";
        tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);

        if(getLobbyCount() < maxLobbyCount && maxLobbyCount != 0){
            lobbies.append(new Lobby(this,
                                    client));

            client->setLobbyOwnerUuid(&lobbies.last()->getUuid());
            lobbies.last()->setOwnerName(client->getName());

                data["successful"] = true;
                data["error"] = "";
                data["lobby_id"] = lobbies.last()->getUuid().toString(QUuid::WithoutBraces);
            }else{
                data["successful"] = false;
                data["error"] = "The maximum number of lobbies that can be created has been exceeded";
                data["lobby_id"] = "0";
            }
            tx["data"] = data;
            txAll(tx, client);
    }
}

// Функция удаления лобби со сервера.
void LobbyManager::closeLobby(QUuid uuidLobby, ClientNetwork* sender)
{
    if(sender){
        Lobby* lobby = findLobby(uuidLobby);
        QJsonObject tx;
        QJsonObject data;
        tx["type"] = "exit_lobby";

        if(lobby){
            if(lobby->getOwner() == sender){
                for (ClientNetwork* client : qAsConst(lobby->getPlayers())) {
                    if(client){
                        if(client == lobby->getOwner()){
                            client->setLobbyOwnerUuid(nullptr);
                        }
                        client->setFinder(false);
                        data["successful"] = true;
                        data["error"] = "";
                        data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                        tx["data"] = data;
                        tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
                        txAll(tx, client);
                    }
                }
                lobbyClose(lobby);
            }else{
                if(lobby->deletePlayer(sender->getTeam(), sender)){
                    sender->setTeam(Team::NONE_TEAM);
                    data["successful"] = true;
                    data["error"] = "";
                    data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                    tx["data"] = data;
                    tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
                }else{
                    data["successful"] = false;
                    data["error"] = "Failed to close the lobby (no such player in this team), please try again";
                    data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                    tx["data"] = data;
                    tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
                }
                txAll(tx, sender);
            }
        }else{
            data["successful"] = false;
            data["error"] = "Failed to close the lobby (lobby not found), please try again";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            tx["data"] = data;
            tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
            txAll(tx, sender);
        }
    }
}

// Функция добавления в определенную команду по id лобби.
void LobbyManager::acceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork* client)
{
    Lobby* tempLobby = findLobby(uuidLobby);
    if(tempLobby != nullptr){
        QJsonObject tx;
        QJsonObject data;
        tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
        if(tempLobby->addPlayer(team, client)){
            client->setTeam(team);
            client->setFinder(false);
            tx["type"] = "accept_select_team";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            data["successful"] = true;
            data["error"] = "";
            tx["data"] = data;
            txAll(tx, client);
        }else{
            tx["type"] = "accept_select_team";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            data["successful"] = false;
            data["error"] = "Error";
            tx["data"] = data;
            txAll(tx, client);
        }
    }
}

// Функция добавления в определенную команду для админа лобби.
void LobbyManager::selectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork *sender)
{
    if(sender){
        Lobby* tempLobby = findLobby(uuidLobby);
        QJsonObject tx;
        QJsonObject data;
        tx["type"] = "accept_select_team";
        tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
        data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

        if(tempLobby && tempLobby->addPlayer(team, sender)){
            sender->setTeam(team);
            sender->setFinder(false);

            data["error"] = "";
            data["successful"] = true;
        }else{
            data["error"] = "Something went wrong when adding to the team.";
            data["successful"] = false;
        }
        tx["data"] = data;
        txAll(tx, sender);
    }
}

// Функция оповещение клиентов, которых хотят пригласить в лобби.
void LobbyManager::invitePlayers(QString login,  QUuid uuidLobby, ClientNetwork* sender)
{
    if(sender != nullptr){
        ClientNetwork* sendClient = findPlayer(login);
        if(sendClient != nullptr){
            QJsonObject tx;
            QJsonObject data;
            tx["type"] = "invite_players";
            tx["id"] = sendClient->getUuid()->toString(QUuid::WithoutBraces);
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            data["alias"] = sender->getUuid()->toString(QUuid::WithoutBraces);
            tx["data"] = data;
            txAll(tx, sendClient);
        }
    }
}

// Функция оповещения игроков лобби о начале игры.
void LobbyManager::startGame(QUuid uuidLobby, ClientNetwork* sender)
{
    if(sender != nullptr){
        Lobby *tempLobby = findLobby(uuidLobby);
        if(tempLobby != nullptr){
            QJsonObject tx;
            QJsonObject data;
            tx["type"] = "start_game_players";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

            if(!tempLobby->getPlayers().contains(nullptr)){
                tempLobby->setGameStarted(true);

                for (ClientNetwork* client : qAsConst(tempLobby->getPlayers())) {
                    tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
                    data["successful"] = true;
                    data["error"] = "";
                }
            }else{
                tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
                data["successful"] = false;
                data["error"] = "Not enough players or one of the participants disconnected unexpectedly.";
            }
            tx["data"] = data;
            txAll(tx, sender);
        }
    }
}

// Функция отсылки игроков в онлайне.
void LobbyManager::onPlayersOnline()
{
    if(!lobbies.empty()){
        // Инициилизируем и заполняем список игроков online.
        QStringList invitePlayersLogins;
        QStringList invitePlayersAliases;

        for (ClientNetwork *client : qAsConst(*clients)) {
            if(client->getFinder()){
                invitePlayersLogins.append(client->getName());
                invitePlayersAliases.append(client->getUuid()->toString(QUuid::WithoutBraces));
            }
        }

        for (Lobby *lobby : qAsConst(lobbies)) {
            if(lobby->getOwner() != nullptr && !lobby->getGameStarted() && lobby->getPlayerCount() >= 1){
                // Доделать фичу с списком друзей.
                QStringList inviteFriendsLogins;
                QStringList inviteFriendsAliases;

                QJsonObject tx;
                QJsonObject data;
                QJsonArray arrPlayers;
                QJsonArray arrFriends;
                QJsonObject temp;

                // Заполняем json списком друзей (доделать фичу).
                for(int i = 0; i < inviteFriendsLogins.count(); i++){
                    temp["login"] = inviteFriendsLogins[i];
                    temp["alias"] = inviteFriendsAliases[i];
                    arrFriends.append(temp);
                }

                // Заполняем json списком игроков online.
                for(int i = 0; i < invitePlayersLogins.count(); i++){
                    temp["login"] = invitePlayersLogins[i];
                    temp["alias"] = invitePlayersAliases[i];
                    arrPlayers.append(temp);
                }

                data["players"] = arrPlayers;
                data["friends"] = arrFriends;
                tx["data"] = data;
                tx["type"] = "players_online";
                tx["id"] = lobby->getOwner()->getUuid()->toString(QUuid::WithoutBraces);

                txAll(tx, lobby->getOwner());
            }
        }
    }
}

// Функция отссылки количества игроков в лобби.
void LobbyManager::onPlayerCount()
{
    if(!lobbies.empty()){
        for (Lobby *lobby : qAsConst(lobbies)) {
            if(!lobby->getGameStarted() && lobby->getPlayerCount() >= 1){
                QJsonObject tx;
                QJsonObject data;
                data["lobby_id"] = lobby->getUuid().toString(QUuid::WithoutBraces);
                data["count"] = lobby->getPlayerCount();
                data["error"] = "";
                tx["data"] = data;
                tx["type"] = "players_count_lobby";
                for (ClientNetwork *client : qAsConst(lobby->getPlayers())) {
                    if(client != nullptr){
                        tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
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

void LobbyManager::clientDisconnected(ClientNetwork* sender)
{
    if(sender){
        Lobby *tempLobby = findLobby(sender);
        if(tempLobby){
            // Если во время самой игры админ отключается - лобби не закрывается.
            // Предполагается, что клиенты могут присоединиться, если утратили соединение.
            if(tempLobby->getOwner() == sender){
                if(tempLobby->getGameStarted()){
                    tempLobby->setOwner(nullptr);
                }else{
                    closeLobby(tempLobby->getUuid(), sender);
                }
            }else{
                int index = tempLobby->getPlayers().indexOf(sender);

                //Случай, когда отключается клиент не находящийся в какой-либо лобби.
                if(index != -1){
                    tempLobby->getPlayers().replace(index, nullptr);
                }
            }
        }
        emit disconnectClient(sender);
    }
}

void LobbyManager::joinLobby(bool join, ClientNetwork *sender)
{
    if(sender != nullptr){
        sender->setFinder(join);
        QJsonObject tx;
        QJsonObject data;

        tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
        tx["type"] = "join";
        data["successful"] = true;
        tx["data"] = data;

        txAll(tx, sender);
    }
}

void LobbyManager::acceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork *sender)
{
    if(sender){
        Lobby* lobby = findLobby(uuidLobby);
        if(lobby){
            QJsonObject tx;
            QJsonObject data;
            tx["type"] = "accept_invite_players";
            tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

            if(lobby->getAcceptedPlayerCount() < lobby->getMaxPlayerSize()){
                if(successful){
                    lobby->setAcceptedPlayerCount(lobby->getAcceptedPlayerCount() + 1);
                    data["successful"] = true;
                    data["error"] = "";
                    tx["data"] = data;
                    txAll(tx, sender);
                    sendSelectTeamClient(uuidLobby, sender);
                }else{
                    data["successful"] = true;
                    data["error"] = "";
                    tx["data"] = data;
                    txAll(tx, sender);
                }
            }else{
                data["successful"] = false;
                data["error"] = "This lobby is no longer available";
                tx["data"] = data;
                txAll(tx, sender);
            }
        }
    }
}

void LobbyManager::lobbyClose(Lobby *lobby)
{
    qInfo() << "Lobby:" << lobby->getUuid().toString() << "is closed.";
    lobbies.removeAll(lobby);
    delete lobby;
}

void LobbyManager::txAll(QJsonObject tx, ClientNetwork *sender)
{
    QJsonDocument txDoc(tx);
    sender->getClientSoc()->sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
}

void LobbyManager::sendSelectTeamClient(QUuid uuidLobby, ClientNetwork *client)
{
    if(client){
        Lobby* tempLobby = findLobby(uuidLobby);
        if(tempLobby){
            QJsonObject tx;
            QJsonObject data;
            tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
            tx["type"] = "select_team";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            data["team_1"] = 2 - tempLobby->getFreeSpotsNS();
            data["team_2"] = 2 - tempLobby->getFreeSpotsEW();
            tx["data"] = data;
            txAll(tx, client);
        }
    }
}
