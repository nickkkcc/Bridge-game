#include "lobby.h"
#include "inc/lobbymanager/lobbymanager.h"

Lobby::Lobby(QObject *parent, ClientNetwork *owner):
    QObject{parent}
{
    this->owner = owner;
    this->uuid = QUuid::createUuid();
}

QUuid &Lobby::getUuid()
{
    return uuid;
}

void Lobby::setMaxRubber(int newMaxRubber)
{
    maxRubber = newMaxRubber;
}


ClientNetwork *Lobby::getOwner() const
{
    return owner;
}

bool Lobby::addPlayer(Team team, ClientNetwork* client)
{
    bool total = false;
    if(size <= maxPlayerSize){
        switch (team) {
            case Team::N_S:{
                if(players[PlayerPosition::NORTH] == nullptr){
                    players[PlayerPosition::NORTH] = client;
                    playerNames[PlayerPosition::NORTH] = client->getName();
                    freeSpotsNS--;
                    size++;
                    total = true;
                    break;
                }else if(players[PlayerPosition::SOUTH] == nullptr){
                    players[PlayerPosition::SOUTH] = client;
                    playerNames[PlayerPosition::SOUTH] = client->getName();
                    freeSpotsNS--;
                    size++;
                    total = true;
                    break;
                }
                break;
            }
            case Team::E_W:{
                if(players[PlayerPosition::EAST] == nullptr){
                    players[PlayerPosition::EAST] = client;
                    playerNames[PlayerPosition::EAST] = client->getName();
                    freeSpotsEW--;
                    size++;
                    total = true;
                    break;
                }else if(players[PlayerPosition::WEST] == nullptr){
                    players[PlayerPosition::WEST] = client;
                    playerNames[PlayerPosition::WEST] = client->getName();
                    freeSpotsEW--;
                    size++;
                    total = true;
                    break;
                }
                break;
            }
        }
    }
    return total;
}

bool Lobby::deletePlayer(Team team, ClientNetwork* client){
    bool total = false;
    if(size > 0){
        switch(team){
            case Team::N_S:{
                if(players[PlayerPosition::NORTH] == client){
                    players.replace(PlayerPosition::NORTH, nullptr);
                    playerNames.replace(PlayerPosition::NORTH, nullptr);
                    freeSpotsNS++;
                    size--;
                    total = true;
                    break;
                }else if(players[PlayerPosition::SOUTH] == client){
                    players.replace(PlayerPosition::SOUTH, nullptr);
                    playerNames.replace(PlayerPosition::SOUTH, nullptr);
                    freeSpotsNS++;
                    size--;
                    total = true;
                    break;
                }
                break;
            }
            case Team::E_W:{
                if(players[PlayerPosition::EAST] == client){
                    players.replace(PlayerPosition::EAST, nullptr);
                    playerNames.replace(PlayerPosition::EAST, nullptr);
                    freeSpotsEW++;
                    size--;
                    total = true;
                    break;
                }else if(players[PlayerPosition::WEST] == client){
                    players.replace(PlayerPosition::WEST, nullptr);
                    playerNames.replace(PlayerPosition::WEST, nullptr);
                    freeSpotsEW++;
                    size--;
                    total = true;
                    break;
                }
                break;
            }
        }
    }
    return total;
}
int Lobby::getFreeSpotsNS() const
{
    return freeSpotsNS;
}

void Lobby::setFreeSpotsNS(int newFreeSpotsNS)
{
    freeSpotsNS = newFreeSpotsNS;
}

void Lobby::setFreeSpotsEW(int newFreeSpotsEW)
{
    freeSpotsEW = newFreeSpotsEW;
}

int Lobby::getFreeSpotsEW() const
{
    return freeSpotsEW;
}

bool Lobby::getGameStarted() const
{
    return gameStarted;
}

int Lobby::getPlayerCount()
{
    return size;
}

QVector<ClientNetwork *> &Lobby::getPlayers()
{
    return players;
}

void Lobby::setOwner(ClientNetwork *newOwner)
{
    owner = newOwner;
}

const QVector<QString> &Lobby::getPlayerNames() const
{
    return playerNames;
}

const QString &Lobby::getOwnerName() const
{
    return ownerName;
}

void Lobby::setOwnerName(const QString &newOwnerName)
{
    ownerName = newOwnerName;
}

void Lobby::setGameStarted(bool newGameStarted)
{
    gameStarted = newGameStarted;
}

int Lobby::getAcceptedPlayerCount() const
{
    return acceptedPlayerCount;
}

void Lobby::setAcceptedPlayerCount(int newAcceptedPlayerCount)
{
    acceptedPlayerCount = newAcceptedPlayerCount;
}

int Lobby::getMaxPlayerSize() const
{
    return maxPlayerSize;
}

void Lobby::setMaxPlayerSize(int newMaxPlayerSize)
{
    maxPlayerSize = newMaxPlayerSize;
}

