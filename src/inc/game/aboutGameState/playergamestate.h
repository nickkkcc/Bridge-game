#ifndef PLAYERGAMESTATE_H
#define PLAYERGAMESTATE_H

#include "gamestate.h"
#include "inc/enumeration/Enumiration.h"

// Класс предоставляет состояние игры для конкретного игрока в текущем состоянии игры.
class PlayerGameState: public GameState
{
public:
    PlayerGameState();
    PlayerGameState(GamePhase phase, const Bid* currentBid, const Bid* contractBid,
                    qint8 gameNumber,qint8 dealNumber, qint8 trickNumber,
                    QVector<CardKit> tricks, qint8 tricksWon[4], PlayerPosition playerTurn,
                    PlayerPosition handToPlay, PlayerPosition dealer,
                    PlayerPosition declarer, Score score, GameEvent gameEvent, QMap<PlayerPosition,
                    QString> playerPositions, QMap<PlayerPosition, qint8> playerCardCount,
                    CardKit playerHand, CardKit dummyHand);
    PlayerGameState(const GameState &gameState, GameEvent gameEvent,
                    QMap<PlayerPosition, QString> playerPositions,
                    QMap<PlayerPosition, qint8> playerCardCount,
                    CardKit playerHand, CardKit dummyHand);
    GameEvent getEvent();
    CardKit getPlayerHandCardKit();
    CardKit getDummyHandCardKit();
    QString getPlayerNameFromPosition(PlayerPosition position);
    qint8 getPlayerCardCountFromPosition(PlayerPosition position);
    void readFromJson(const QJsonObject &playerGameState);
    void writeToJson(QJsonObject &playerGameState) const;
    bool operator ==(const PlayerGameState& playerGameState)const;

    // Сопоставление позиций игроков с именами (логинами) игроков в каждой позиции.
    QMap<PlayerPosition, QString> playerPositions;
private:

    // Самое последнее событие в игре, которое можно использовать для определения того, какие атрибуты, вероятно, изменились
    GameEvent gameEvent;

    // Сопоставление позиций игроков с количеством карт, которые держит каждый игрок
    QMap<PlayerPosition, qint8> playerCardCount;

    // Карты в руке игрока, для которого создавалось игровое состояние
    CardKit playerHandCardKit;

    // Карты в руке манекена. Nullptr, если состояние игры BIDDING
    CardKit dummyHandCardKit;
};
Q_DECLARE_METATYPE(PlayerGameState);

#endif // PLAYERGAMESTATE_H
