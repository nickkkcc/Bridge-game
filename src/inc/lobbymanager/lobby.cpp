#include "lobby.h"
#include "qcoreapplication.h"

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

ServerGameState *Lobby::getGameState() const
{
    return gameState;
}

Lobby::~Lobby()
{
    delete gameState;
}

// Проверить.
void Lobby::rxBidSelected(Bid bid)
{
    ClientNetwork* senderClient = qobject_cast<ClientNetwork*>(sender());

    if(senderClient){
        if(gameState->isBidValid(bid)){
            gameState->updateBidState(bid);
            moveComplete = true;
            nextPlayerTurn();
        }else{
            senderClient->bidRejected("Bid is an invalid");
        }
    }
}

// Проверить не на этапе торгов.
void Lobby::rxMoveSelected(Card card)
{
    ClientNetwork* senderClient = qobject_cast<ClientNetwork*>(sender());

    if(senderClient){
        if(gameState->isCardValid(card)){
            gameState->updatePlayState(card);
            moveComplete = true;
            nextPlayerTurn();
        }else{
            senderClient->moveRejected("Bid is an invalid");
        }
    }
}

// Добавлены кейсы только для стадии торгов. Проверить.
void Lobby::gameEventOccured(GameEvent event)
{
    switch (event) {
        case INITIALIZE:{
        qInfo() << "Lobby:" << uuid.toString() << "---> at state --->" << "INITIALIZE";
            break;
        }
        case BID_START:{
            qInfo() << "Lobby:" << uuid.toString() << "---> at state --->" << "BID_START";
            sendUpdatedGameStateToClients(event);
            break;
        }
        case BID_RESTART:{
            qInfo() << "Lobby:" << uuid.toString() << "---> at state --->" << "BID_RESTART";
            sendUpdatedGameStateToClients(event);
            break;
        }
        case PLAYER_BID:{
            qInfo() << "Lobby:" << uuid.toString() << "---> at state --->" << "PLAYER_BID";
            sendUpdatedGameStateToClients(event);
            break;
        }
        case BID_END:{
            qInfo() << "Lobby:" << uuid.toString() << "---> at state --->" << "BID_END";
            sendUpdatedGameStateToClients(event);
            break;
        }
    }
}

// Проверить.
void Lobby::setMatchEnded(bool isCompleted)
{
    gameComplete = isCompleted;
}

// Проверить.
void Lobby::sendUpdatedGameStateToClients(GameEvent event)
{
    for(ClientNetwork* client : qAsConst(players)){
        PlayerGameState state = gameState->getPlayerGameState(client->getPosition(),
                                                              players,
                                                              event);
        client->updateGameState(state);
    }
}

// Проверить.
ClientNetwork *Lobby::getClient(PlayerPosition position)
{
    for(ClientNetwork* client : qAsConst(players)){
        if(client->getPosition() == position){
            return client;
        }
    }
    return nullptr;
}

// Проверить.
void Lobby::nextPlayerTurn()
{
    ClientNetwork* clientTurn = getClient(gameState->getPlayerTurnPosition());
    if(gameState->getGamePhase() == BIDDING){
        clientTurn->bidTurn();
    }else{
        clientTurn->moveTurn();
    }
}

// Проверить с коннектом к сигналам.
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
                    client->setPosition(PlayerPosition::NORTH);
                    client->setTeam(Team::N_S);
                    connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                    connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                    break;
                }else if(players[PlayerPosition::SOUTH] == nullptr){
                    players[PlayerPosition::SOUTH] = client;
                    playerNames[PlayerPosition::SOUTH] = client->getName();
                    freeSpotsNS--;
                    size++;
                    total = true;
                    client->setPosition(PlayerPosition::SOUTH);
                    client->setTeam(Team::N_S);
                    connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                    connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
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
                    client->setPosition(PlayerPosition::EAST);
                    client->setTeam(Team::E_W);
                    connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                    connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                    break;
                }else if(players[PlayerPosition::WEST] == nullptr){
                    players[PlayerPosition::WEST] = client;
                    playerNames[PlayerPosition::WEST] = client->getName();
                    freeSpotsEW--;
                    size++;
                    total = true;
                    client->setPosition(PlayerPosition::WEST);
                    client->setTeam(Team::E_W);
                    connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                    connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                    break;
                }
                break;
            }
            default:{
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
                disconnect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                disconnect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
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
                disconnect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                disconnect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                break;
            }
        }
    }
    return total;
}

// Проверить.
void Lobby::startMatch()
{
    gameState = new ServerGameState();

    if(this->maxRubber <= 0){
        qInfo() << "Lobby -->" << uuid.toString() << "ERROR -->"
                << "maxRubbers must be >= 1.(current value -->"
                << QString::number(maxRubber);
        return;
    }

    qRegisterMetaType<GameEvent>("GameEvent");
    connect(gameState, &ServerGameState::gameEvent, this, &Lobby::gameEventOccured);

    sendUpdatedGameStateToClients(INITIALIZE);

    gameState->startMatch(this->maxRubber);

    gameComplete = false;
        moveComplete = false;
        nextPlayerTurn();
//        QCoreApplication::processEvents(QEventLoop::AllEvents);
//            QCoreApplication::processEvents(QEventLoop::AllEvents);
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

// Возвращает количество игроков, котрые уже приняли приглашение.
int Lobby::getAcceptedPlayerCount() const
{
    return acceptedPlayerCount;
}

// Установить количество игроков, которые уже приняли приглашение.
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

