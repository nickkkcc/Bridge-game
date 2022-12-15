#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "inc/enumeration/Enumiration.h"
#include "inc/game/aboutCard/bid.h"
#include "inc/game/aboutCard//cardkit.h"
#include "inc/game/aboutGameState/score.h"
#include <QVector>

// Определяет состояние игры для каждого из матчей.
class GameState
{
public:
    GameState();
    GameState(const GameState &gameState);
    ~GameState();
    GameState& operator = (const GameState &gameState);
    GamePhase getGamePhase() const;
    const Bid* getCurrentBid() const;
    const Bid* getContractBid() const;
    qint32 getRubberNumber() const;
    qint8 getGameNumber() const;
    qint8 getDealNumber() const;
    qint8 getTrickNumber() const;
    const QVector<CardKit> getTricks() const;
    PlayerPosition getPlayerTurnPosition() const;
    PlayerPosition getHandToPlayPosition() const;
    PlayerPosition getDealerPosition() const;
    PlayerPosition getDeclarerPosition() const;
    PlayerPosition getDummyPosition() const;
    qint8 getTricksWonFromPosition(PlayerPosition position) const;
    qint8 getTricksWonFromTeam(Team team) const;
    bool getTeamVulnerable(Team team) const;
    const Score getScore() const;
    const Card* getLastCardPlayed() const;
protected:
    GamePhase gamePhase;
    // currentBid относится к самой последней действительной ставке, сделанной на этапе торгов
    // nullptr, если ставка не была сделана или фаза CARDPLAY
    Bid* currentBid = nullptr;
    // ContractBid относится к ставке, принятой в качестве контракта на игру
    // nullptr, если фаза BIDDING
    Bid* contractBid = nullptr;
    qint8 rubberNumber;
    qint8 gameNumber;
    qint8 dealNumber; // Ноль перед первой сделкой.
    qint8 trickNumber; // Ноль на протяжении торгов.
    QVector<CardKit> tricks;
    // Количество взяток, выигранных каждым игроком в текущей раздаче
    // Индексируется перечислением PlayerPosition, NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3
    qint8 tricksWon[4] = {0, 0, 0, 0};
    // PlayerTurn относится к позиции игрока, который должен выбрать карту для игры
    // Когда пришло время играть с руки манекена, playerTurn ссылается на позицию разыгрывающего
    PlayerPosition playerTurnPosition;
    // handToPlay относится к позиции игрока, с руки которого должна быть сыграна карта
    // Когда пришло время играть с руки манекена, handToPlay ссылается на позицию манекена
    PlayerPosition handToPlayPosition;
    PlayerPosition dealerPosition;
    PlayerPosition declarerPosition;
    Score matchScore;
};
Q_DECLARE_METATYPE(GameState);
#endif // GAMESTATE_H
