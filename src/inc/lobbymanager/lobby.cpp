#include "lobby.h"
#include <QThread>

Lobby::Lobby(QObject *const parent, ClientNetwork *const owner) : QObject(parent)
{

    this->owner = owner;
    this->uuid = QUuid::createUuid();
}

const QUuid &Lobby::getUuid() const
{

    return uuid;
}

void Lobby::setMaxRubber(int newMaxRubber)
{

    maxRubber = newMaxRubber;
}

ClientNetwork *const Lobby::getOwner() const
{

    return owner;
}

ServerGameState *const Lobby::getGameState() const
{

    return gameState;
}

Lobby::~Lobby()
{

    if (gameState)
    {

        gameState->deleteLater();
    }
    qInfo() << "Server: lobby --->" << uuid.toString() << "---> deleted.";
}

// Проверить.
void Lobby::rxBidSelected(Bid bid)
{

    ClientNetwork *senderClient = qobject_cast<ClientNetwork *>(sender());

    if (senderClient)
    {

        if (gameState->isBidValid(bid))
        {

            gameState->updateBidState(bid);
            moveComplete = true;
            nextPlayerTurn();
        }
        else
        {

            senderClient->bidRejected("Ваша заявка недействительна!");
        }
    }
}

// Проверить не на этапе торгов.
void Lobby::rxMoveSelected(Card card)
{

    ClientNetwork *senderClient = qobject_cast<ClientNetwork *>(sender());

    if (senderClient)
    {

        if (gameState->isCardValid(card))
        {

            gameState->updatePlayState(card);
            moveComplete = true;
            qInfo() << "NOTIFY";
            if (gameState->getCurrentGameEvent() != GameEvent::TRICK_END)
            {
                nextPlayerTurn();
            }
        }
        else
        {
            senderClient->moveRejected("Ваша карта недействительна!");
        }
    }
}

// Добавлены кейсы только для стадии торгов. Проверить.
void Lobby::gameEventOccured(GameEvent event)
{
    switch (event)
    {

    case INITIALIZE:

    {

        break;
    }

    case BID_START:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case BID_RESTART:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case PLAYER_BID:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case BID_END:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case PLAY_START:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case TRICK_START:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case PLAYER_MOVED:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case TRICK_END:

    {
        sendUpdatedGameStateToClients(event);
        break;
    }

    case PLAY_END:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case MATCH_END:

    {

        sendUpdatedGameStateToClients(event);
        gameHistory.setGame_end(QDateTime::currentDateTime());
        gameHistory.setWinner_team(gameState->getScore().getGameWinner());
        gameHistory.setTotal_score_NS(gameState->getScore().getTotalScore(Team::N_S));
        gameHistory.setTotal_score_EW(gameState->getScore().getTotalScore(Team::E_W));
        emit sendHistoryToLM(gameHistory);
        emit sendMatchEndToLM(this);
        break;
    }

    case RUBBER_COMPLETED:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case PLAY_STOP:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }

    case PLAY_CONTINUES:

    {

        sendUpdatedGameStateToClients(event);
        break;
    }
    }

    if (event != PLAY_STOP && event != PLAY_CONTINUES)
    {
        lastGameEvent = event;
        qInfo() << "Lobby:" << uuid.toString() << "---> last gamestate --->" << Lobby::gameEventToString(event);
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

    for (ClientNetwork *client : qAsConst(players))
    {
        if (client)

        {
            PlayerGameState state = gameState->getPlayerGameState(client->getPosition(), players, event);
            client->updateGameState(state);
        }
    }
}

// Проверить.
ClientNetwork *Lobby::getClient(PlayerPosition position) const
{
    for (auto client : qAsConst(players))
    {

        if (client->getPosition() == position)
        {

            return client;
        }
    }
    return nullptr;
}

History &Lobby::getGameHistory()
{
    return gameHistory;
}

void Lobby::setGameHistory(History newGameHistory)
{
    gameHistory = newGameHistory;
}

// Проверить.
void Lobby::nextPlayerTurn()
{

    ClientNetwork *clientTurn = getClient(gameState->getPlayerTurnPosition());
    if (gameState->getGamePhase() == BIDDING)
    {

        clientTurn->bidTurn();
    }
    else
    {

        clientTurn->moveTurn();
    }
}

QHash<QString, PlayerPosition> &Lobby::getDisconnectedPlayers()
{
    return disconnectedPlayers;
}

const GameEvent Lobby::getLastGameEvent() const
{
    return lastGameEvent;
}

const int Lobby::getSize() const
{
    return size;
}

void Lobby::setSize(int newSize)
{
    size = newSize;
}

QSet<ClientNetwork *> &Lobby::getTempPlayers()
{
    return tempPlayers;
}

// Проверить с коннектом к сигналам.
bool Lobby::addPlayer(Team team, ClientNetwork *const client)
{
    bool total = false;
    if (size <= maxPlayerSize)
    {

        switch (team)
        {

        case Team::N_S:

        {
            if (players[NORTH] == nullptr)
            {

                players[NORTH] = client;
                playerNames[NORTH] = client->getName();
                freeSpotsNS--;
                size++;
                total = true;
                client->setPosition(NORTH);
                client->setTeam(N_S);
                connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                break;
            }
            else if (players[SOUTH] == nullptr)
            {

                players[SOUTH] = client;
                playerNames[SOUTH] = client->getName();
                freeSpotsNS--;
                size++;
                total = true;
                client->setPosition(SOUTH);
                client->setTeam(N_S);
                connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                break;
            }
            break;
        }

        case Team::E_W:

        {
            if (players[EAST] == nullptr)
            {

                players[EAST] = client;
                playerNames[EAST] = client->getName();
                freeSpotsEW--;
                size++;
                total = true;
                client->setPosition(EAST);
                client->setTeam(E_W);
                connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                break;
            }
            else if (players[WEST] == nullptr)
            {

                players[WEST] = client;
                playerNames[WEST] = client->getName();
                freeSpotsEW--;
                size++;
                total = true;
                client->setPosition(WEST);
                client->setTeam(E_W);
                connect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
                connect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
                break;
            }
            break;
        }

        case NONE_TEAM:
            break;
        }
    }
    return total;
}

bool Lobby::deletePlayer(Team team, ClientNetwork *const client)
{

    bool total = false;

    if (size > 0)
    {

        switch (team)
        {

        case N_S:

        {
            if (players[NORTH] == client)
            {

                players.replace(NORTH, nullptr);
                playerNames.replace(NORTH, nullptr);
                freeSpotsNS++;
                size--;
                total = true;
                break;
            }
            else if (players[SOUTH] == client)
            {

                players.replace(SOUTH, nullptr);
                playerNames.replace(SOUTH, nullptr);
                freeSpotsNS++;
                size--;
                total = true;
                break;
            }
            disconnect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
            disconnect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
            break;
        }

        case E_W:

        {
            if (players[EAST] == client)
            {

                players.replace(EAST, nullptr);
                playerNames.replace(EAST, nullptr);
                freeSpotsEW++;
                size--;
                total = true;
                break;
            }
            else if (players[WEST] == client)
            {

                players.replace(WEST, nullptr);
                playerNames.replace(WEST, nullptr);
                freeSpotsEW++;
                size--;
                total = true;
                break;
            }
            disconnect(client, &ClientNetwork::rxMoveSelected, this, &Lobby::rxMoveSelected);
            disconnect(client, &ClientNetwork::rxBidSelected, this, &Lobby::rxBidSelected);
            break;
        }
        case NONE_TEAM:
            break;
        }
    }
    return total;
}

// Проверить.
void Lobby::startMatch()
{

    gameHistory.setGame_start(QDateTime::currentDateTime());
    gameHistory.setOwner_login(ownerName);
    gameHistory.setOwner_alias(owner->getAlias());
    gameHistory.setPlayer_N_login(playerNames[PlayerPosition::NORTH]);
    gameHistory.setPlayer_N_alias(players[PlayerPosition::NORTH]->getAlias());

    gameHistory.setPlayer_S_login(playerNames[PlayerPosition::SOUTH]);
    gameHistory.setPlayer_S_alias(players[PlayerPosition::SOUTH]->getAlias());

    gameHistory.setPlayer_E_login(playerNames[PlayerPosition::EAST]);
    gameHistory.setPlayer_E_alias(players[PlayerPosition::EAST]->getAlias());

    gameHistory.setPlayer_W_login(playerNames[PlayerPosition::WEST]);
    gameHistory.setPlayer_W_alias(players[PlayerPosition::WEST]->getAlias());

    gameState = new ServerGameState();
    connect(gameState, &ServerGameState::nexPlayerTurn, this, &Lobby::nextPlayerTurn);
    if (this->maxRubber <= 0)

    {
        qInfo() << "Lobby -->" << uuid.toString() << "ERROR -->"
                << "maxRubbers must be >= 1.(current value -->" << QString::number(maxRubber);
        return;
    }

    qRegisterMetaType<GameEvent>("GameEvent");
    connect(gameState, &ServerGameState::gameEvent, this, &Lobby::gameEventOccured);

    sendUpdatedGameStateToClients(INITIALIZE);

    gameState->startMatch(this->maxRubber);

    gameComplete = false;
    moveComplete = false;
    nextPlayerTurn();
}

const int Lobby::getFreeSpotsNS() const
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

const int Lobby::getFreeSpotsEW() const
{

    return freeSpotsEW;
}

const bool Lobby::getGameStarted() const
{

    return gameStarted;
}

const int Lobby::getPlayerCount() const
{

    return size;
}

QVector<ClientNetwork *> &Lobby::getPlayers()
{

    return players;
}

void Lobby::setOwner(ClientNetwork *const newOwner)
{

    owner = newOwner;
}

QVector<QString> &Lobby::getPlayerNames()
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
const int Lobby::getAcceptedPlayerCount() const
{

    return acceptedPlayerCount;
}

// Установить количество игроков, которые уже приняли приглашение.
void Lobby::setAcceptedPlayerCount(int newAcceptedPlayerCount)
{

    acceptedPlayerCount = newAcceptedPlayerCount;
}

const int Lobby::getMaxPlayerSize() const
{

    return maxPlayerSize;
}

void Lobby::setMaxPlayerSize(int newMaxPlayerSize)
{

    maxPlayerSize = newMaxPlayerSize;
}

QString Lobby::bidCallToString(BidCall bid)
{
    switch (bid)
    {

    case PASS:
        return "PASS (0)";
    case BID:
        return "BID (1)";
    case DOUBLE_BID:
        return "DOUBLE_BID (2)";
    case REDOUBLE_BID:
        return "REDOUBLE_BID (3)";
    }
}

QString Lobby::cardRankToString(CardRank rank)
{
    switch (rank)
    {

    case TWO:
        return "TWO (1)";
    case THREE:
        return "THREE (2)";
    case FOUR:
        return "FOUR (3)";
    case FIVE:
        return "FIVE (4)";
    case SIX:
        return "SIX (5)";
    case SEVEN:
        return "SEVEN (6)";
    case EIGHT:
        return "EIGHT (7)";
    case NINE:
        return "NINE (8)";
    case TEN:
        return "TEN (9)";
    case JACK:
        return "JACK (10)";
    case QUEEN:
        return "QUEEN (11)";
    case KING:
        return "KING (12)";
    case ACE:
        return "ACE (13)";
    }
}

QString Lobby::cardSuitToString(CardSuit suit)
{
    switch (suit)
    {

    case CLUBS:
        return "CLUBS (0)";
    case DIAMONDS:
        return "DIAMONDS (1)";
    case HEARTS:
        return "HEARTS (2)";
    case SPADES:
        return "SPADES (3)";
    case NONE:
        return "NONE (4)";
    }
}

QString Lobby::gameEventToString(GameEvent event)
{
    switch (event)
    {

    case INITIALIZE:
        return "INITIALIZE (0)";
    case BID_START:
        return "BID_START (1)";
    case BID_RESTART:
        return "BID_RESTART (2)";
    case PLAYER_BID:
        return "PLAYER_BID (3)";
    case BID_END:
        return "BID_END (4)";
    case PLAY_START:
        return "PLAY_START (5)";
    case TRICK_START:
        return "TRICK_START (6)";
    case PLAYER_MOVED:
        return "PLAYER_MOVED (7)";
    case TRICK_END:
        return "TRICK_END (8)";
    case PLAY_END:
        return "PLAY_END (9)";
    case MATCH_END:
        return "MATCH_END (10)";
    case RUBBER_COMPLETED:
        return "RUBBER_COMPLETED (11)";
    case PLAY_STOP:
        return "PLAY_STOP (12)";
    case PLAY_CONTINUES:
        return "PLAY_CONTINUES (13)";
    }
}

QString Lobby::gamePhaseToString(GamePhase phase)
{
    switch (phase)
    {

    case BIDDING:
        return "BIDDING (0)";
    case CARDPLAY:
        return "CARDPLAY (1)";
    }
}

QString Lobby::playerPositionToString(PlayerPosition position)
{
    switch (position)
    {
    case NORTH:
        return "NORTH (0)";
    case EAST:
        return "EAST (1)";
    case SOUTH:
        return "SOUTH (2)";
    case WEST:
        return "WEST (3)";
    case NONE_POSITION:
        break;
    }
}

QString Lobby::teamToString(Team team)
{
    switch (team)
    {

    case N_S:
        return "N_S (0)";
    case E_W:
        return "E_W (1)";
    case NONE_TEAM:
        return "NONE_TEAM (2)";
    }
}
