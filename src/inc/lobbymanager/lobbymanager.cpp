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
    timer->setInterval(1000);
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
    ClientNetwork* senderClient = qobject_cast<ClientNetwork*>(sender());

    QJsonObject tx;
    QJsonObject data;
    if(getLobbyCount() <= maxLobbyCount){
        lobbies.append(new Lobby(this,
                                 client));
        senderClient->setLobbyOwnerUuid(&lobbies.last()->getUuid());
        lobbies.last()->setOwnerName(client->getName());
            data["successful"] = true;
            data["error"] = "";
            data["lobby_id"] = lobbies.last()->getUuid().toString(QUuid::WithoutBraces);

            tx["data"] = data;
            tx["type"] = "create_lobby";
            tx["id"] = senderClient->getUuid()->toString(QUuid::WithoutBraces);
        }else{
            data["successful"] = false;
            data["error"] = "The maximum number of lobbies that can be created has been exceeded";
            data["lobby_id"] = "0";

            tx["data"] = data;
            tx["type"] = "create_lobby";
            tx["id"] = senderClient->getUuid()->toString(QUuid::WithoutBraces);
        }
        txAll(tx, senderClient);
}

// Функция удаления лобби со сервера.
void LobbyManager::closeLobby(QUuid uuidLobby)
{
    // Доделать, чтобы можно было оповестить всех игроков в лобби. Что лобби закрыто админом.
    ClientNetwork* senderClient = qobject_cast<ClientNetwork*>(sender());

    Lobby* lobby = findLobby(uuidLobby);
    if(lobby != nullptr){
        if(lobby->getOwner() == senderClient){
        QJsonObject tx;
        QJsonObject data;
        if(uuidLobby != nullptr){
            for (ClientNetwork* client : lobby->getPlayers()) {
                if(client != nullptr){
                    client->setLobbyOwnerUuid(nullptr);
                    client->setFinder(false);
                    data["successful"] = true;
                    data["error"] = "";
                    data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                    tx["data"] = data;
                    tx["type"] = "exit_lobby";
                    tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
                    txAll(tx, client);

                }
            }
            lobbyClose(lobby);
        }else{
            data["successful"] = false;
            data["error"] = "Failed to close the lobby, please try again";
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

            tx["data"] = data;
            tx["type"] = "exit_lobby";
            tx["id"] = senderClient->getUuid()->toString(QUuid::WithoutBraces);
            txAll(tx, senderClient);
        }

        }else{
            QJsonObject tx;
            QJsonObject data;

            if(uuidLobby != nullptr && lobby->deletePlayer(senderClient->getTeam(), senderClient)){
                senderClient->setTeam(Team::NONE_TEAM);
                data["successful"] = true;
                data["error"] = "";
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                tx["data"] = data;
                tx["type"] = "exit_lobby";
                tx["id"] = senderClient->getUuid()->toString(QUuid::WithoutBraces);
            }else{
                data["successful"] = false;
                data["error"] = "Failed to close the lobby, please try again";
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);

                tx["data"] = data;
                tx["type"] = "exit_lobby";
                tx["id"] = senderClient->getUuid()->toString(QUuid::WithoutBraces);
            }
            txAll(tx, senderClient);
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
void LobbyManager::selectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork *client)
{
    Lobby* tempLobby = findLobby(uuidLobby);
    QJsonObject tx;
    QJsonObject data;
    tx["type"] = "accept_select_team";
    tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);

    if(tempLobby != 0 && tempLobby->addPlayer(team, client)){
        client->setTeam(team);
        client->setFinder(false);
        data["error"] = "";
        data["successful"] = true;
        data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
        tx["data"] = data;
    }else{
        data["error"] = "Something went wrong when adding to the team.";
        data["successful"] = false;
        data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
        tx["data"] = data;
    }
    txAll(tx, client);
}

// Функция оповещение клиентов, которых хотят пригласить в лобби.
void LobbyManager::invitePlayers(QString login,  QUuid uuidLobby, ClientNetwork* sender)
{
    ClientNetwork* sendClient = findPlayer(login);
    if(sender != nullptr && sendClient != nullptr){
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

// Функция оповещения игроков лобби о начале игры.
void LobbyManager::startGame(QUuid uuidLobby)
{
    ClientNetwork* senderClient = qobject_cast<ClientNetwork*>(sender());

    Lobby *tempLobby = findLobby(uuidLobby);
    QJsonObject tx;
    QJsonObject data;
    if(!tempLobby->getPlayers().contains(nullptr)){
        tempLobby->setGameStarted(true);

        for (ClientNetwork* client : tempLobby->getPlayers()) {
            tx["id"] = client->getUuid()->toString(QUuid::WithoutBraces);
            tx["type"] = "start_game_players";

            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            data["successful"] = true;
            data["error"] = "";
            tx["data"] = data;
            txAll(tx,client);
            return;
        }
    }
        tx["id"] = senderClient->getUuid()->toString(QUuid::WithoutBraces);
        tx["type"] = "start_game_players";
        data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
        data["successful"] = false;
        data["error"] = "Not enough players or one of the participants disconnected unexpectedly.";
        tx["data"] = data;
        txAll(tx, senderClient);
}

// Функция отсылки игроков в онлайне.
void LobbyManager::onPlayersOnline()
{
    if(!lobbies.empty()){
            for (Lobby *lobby : qAsConst(lobbies)) {
            if(lobby->getOwner() != nullptr && lobby->getPlayerCount() >= 1){
                        QStringList invitePlayersLogins;
                        QStringList invitePlayersAliases;

                        QStringList inviteFriendsLogins;
                        QStringList inviteFriendsAliases;

                        for (ClientNetwork *client : qAsConst(*clients)) {
                            if(client->getFinder()){
                                invitePlayersLogins.append(client->getName());
                                invitePlayersAliases.append(client->getUuid()->toString(QUuid::WithoutBraces));
                            }
                        }
                        QJsonObject tx;
                        QJsonObject data;
                        QJsonArray arrPlayers;
                        QJsonArray arrFriends;
                        QJsonObject temp;

                        // Переделать, когда будут друзья.

                        for(int i = 0; i < inviteFriendsLogins.count(); i++){
                            temp["login"] = inviteFriendsLogins[i];
                            temp["alias"] = inviteFriendsAliases[i];
                            arrFriends.append(temp);
                        }

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
                        QJsonDocument txDoc(tx);
                        lobby->getOwner()->getClientSoc()->sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
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
                            QJsonDocument txDoc(tx);
                            client->getClientSoc()->sendTextMessage(txDoc.toJson(QJsonDocument::Compact));
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
    Lobby *tempLobby = findLobby(sender);
    if(tempLobby != nullptr){
        tempLobby->setOwner(nullptr);
        int index = tempLobby->getPlayers().indexOf(sender);
        tempLobby->getPlayers().replace(index, nullptr);
    }
    emit disconnectClient(sender);
}

void LobbyManager::joinLobby(bool join, ClientNetwork *sender)
{
    // Обсудить нужен ли ответ на запрос к подключению.
    if(sender != nullptr){
        sender->setFinder(join);
    }
}

void LobbyManager::acceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork *sender)
{
    // Доделать функцию.
    Lobby* lobby = findLobby(uuidLobby);
    if(sender != nullptr && lobby != nullptr){
        QJsonObject tx;
        QJsonObject data;
        if(lobby->getAcceptedPlayerCount() < lobby->getMaxPlayerSize()){
            if(successful){
                lobby->setAcceptedPlayerCount(lobby->getAcceptedPlayerCount() + 1);
                tx["type"] = "accept_invite_players";
                tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                data["successful"] = true;
                data["error"] = "";
                tx["data"] = data;
                txAll(tx, sender);
                sendSelectTeamClient(uuidLobby, sender);
            }else{
                tx["type"] = "accept_invite_players";
                tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
                data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
                data["successful"] = true;
                data["error"] = "";
                tx["data"] = data;
                txAll(tx, sender);
            }
        }else{
            tx["type"] = "accept_invite_players";
            tx["id"] = sender->getUuid()->toString(QUuid::WithoutBraces);
            data["lobby_id"] = uuidLobby.toString(QUuid::WithoutBraces);
            data["successful"] = false;
            data["error"] = "This lobby is no longer available";
            tx["data"] = data;
            txAll(tx, sender);
        }

    }
}

void LobbyManager::lobbyClose(Lobby *lobby)
{
    qInfo() << "Lobby: " << lobby->getUuid().toString(QUuid::WithBraces) << " is closed.";
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
    if(client != nullptr){
        Lobby* tempLobby = findLobby(uuidLobby);
        if(tempLobby != nullptr){
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
