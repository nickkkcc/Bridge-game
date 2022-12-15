#include "gamestate.h"

GameState::GameState() {}

// Конструктор копирования
GameState::GameState(const GameState &gameState)
{
    this->gamePhase = gameState.getGamePhase();
    if(gameState.getCurrentBid() == nullptr)
        currentBid = nullptr;
    else
        currentBid = new Bid(*gameState.getCurrentBid());
    if(gameState.getContractBid() == nullptr)
        contractBid = nullptr;
    else
        contractBid = new Bid(*gameState.getContractBid());
    this->rubberNumber = gameState.getRubberNumber();
    this->gameNumber = gameState.getGameNumber();
    this->dealNumber = gameState.getDealNumber();
    this->trickNumber = gameState.getTrickNumber();
    this->tricks = gameState.getTricks();
    this->tricksWon[NORTH] = gameState.tricksWon[NORTH];
    this->tricksWon[EAST] = gameState.tricksWon[EAST];
    this->tricksWon[SOUTH] = gameState.tricksWon[SOUTH];
    this->tricksWon[WEST] = gameState.tricksWon[WEST];
    this->playerTurnPosition = gameState.getPlayerTurnPosition();
    this->handToPlayPosition = gameState.getHandToPlayPosition();
    this->dealerPosition = gameState.getDealerPosition();
    this->declarerPosition = gameState.getDeclarerPosition();
    this->matchScore = gameState.getScore();
}

// Copy assignment operator
GameState& GameState::operator = (const GameState &gameState)
{
    this->gamePhase = gameState.getGamePhase();
    if(gameState.getCurrentBid() == nullptr)
        currentBid = nullptr;
    else
        currentBid = new Bid(*gameState.getCurrentBid());
    if(gameState.getContractBid() == nullptr)
        contractBid = nullptr;
    else
        contractBid = new Bid(*gameState.getContractBid());
    this->rubberNumber = gameState.getRubberNumber();
    this->gameNumber = gameState.getGameNumber();
    this->dealNumber = gameState.getDealNumber();
    this->trickNumber = gameState.getTrickNumber();
    this->tricks = gameState.getTricks();
    this->tricksWon[NORTH] = gameState.tricksWon[NORTH];
    this->tricksWon[EAST] = gameState.tricksWon[EAST];
    this->tricksWon[SOUTH] = gameState.tricksWon[SOUTH];
    this->tricksWon[WEST] = gameState.tricksWon[WEST];
    this->playerTurnPosition = gameState.getPlayerTurnPosition();
    this->handToPlayPosition = gameState.getHandToPlayPosition();
    this->dealerPosition = gameState.getDealerPosition();
    this->declarerPosition = gameState.getDeclarerPosition();
    this->matchScore = gameState.getScore();
    return *this;
}

GameState::~GameState() {
    delete this->currentBid;
    delete this->contractBid;
}

// Получить текущую фазу игры.
GamePhase GameState::getGamePhase() const
{
    return this->gamePhase;
}

// Получить последнюю действующую ставку. Возвращает nullptr, если ставка не сделана.
const Bid* GameState::getCurrentBid() const
{
    return this->currentBid;
}

// Поулчить заявку, выбранную в качестве контракта.
const Bid* GameState::getContractBid() const
{
    return this->contractBid;
}

// Получить номер текущего роббера.
qint32 GameState::getRubberNumber() const
{
    return this->rubberNumber;
};


// Получить текущий номер игры.
qint8 GameState::getGameNumber() const
{
    return this->gameNumber;
}

// Получить номер текущей сделки.
qint8 GameState::getDealNumber() const
{
    return this->dealNumber;
}

// Получить номер текущего trick-a.
qint8 GameState::getTrickNumber() const
{
    return this->trickNumber;
}

// Получить последнюю карту, сыгранную на этапе карточной игры.
// Возвращает nullptr во время фазы торгов и до того, как будет мыграна первая карта.
// Когда в новом trick возвращает последнюю карту, то возвращается новый объект карты, отдельный от внутренней карты trick и должен упраляться памятью.
const Card* GameState::getLastCardPlayed() const
{
    // Не возвращаем карту, если ине была сыграна ни одна trick и в первом trick  не разыграна первая карта.
    if(this->tricks.length() == 0 || this->tricks[0].getCardCount() == 0)
        return nullptr;
    else if(this->tricks.last().getCardCount() == 0)
        return new Card(this->tricks[this->tricks.length() - 2].getCardFromPosition(3));
    else
        return new Card(this->tricks.last().getCardFromPosition(this->tricks.last().getCardCount() - 1));
}

// Получить набор trick-ов, которые уже были разыграны в данной игре.
const QVector<CardKit> GameState::getTricks() const
{
    return this->tricks;
}

// Получить позицию игрока, чья очередь делать ход (позиция игрока, от которого требуется действие).
PlayerPosition GameState::getPlayerTurnPosition() const
{
    return this->playerTurnPosition;
}

// Поулчить позицию игрока с чьей руки должна быть розыграна карта.
PlayerPosition GameState::getHandToPlayPosition() const
{
    return this->handToPlayPosition;
}

// Получить позицию дилера.
PlayerPosition GameState::getDealerPosition() const
{
    return this->dealerPosition;
}

// Получить позицию разыгрывающего игрока.
PlayerPosition GameState::getDeclarerPosition() const
{
    return this->declarerPosition;
}

// Поулчить позицию дурочка.
PlayerPosition GameState::getDummyPosition() const
{
    switch (this->declarerPosition) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
        default:
            return EAST;

    }
}

// Получить количество взяток, выгранных в текущей сделке, указанным игроком.
qint8 GameState::getTricksWonFromPosition(PlayerPosition playerPosition) const
{
    return this->tricksWon[playerPosition];
}

// Поулчить количество взяток, выигранных в текущей сделке, указанной командой.
qint8 GameState::getTricksWonFromTeam(Team team) const
{
    if(team == N_S)
        return this->tricksWon[NORTH] + this->tricksWon[SOUTH];
    else
        return this->tricksWon[EAST] + this->tricksWon[WEST];
}

// Вернуть является ли команда уязвимой.
bool GameState::getTeamVulnerable(Team team) const
{
    return this->matchScore.getTeamVulnerable(team);
}

// Получить объект счета для текущего матча.
const Score GameState::getScore() const
{
    return this->matchScore;
}
