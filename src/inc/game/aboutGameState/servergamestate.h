#ifndef SERVERGAMESTATE_H
#define SERVERGAMESTATE_H

#include "gamestate.h"
#include <inc/network/clientnetwork.h>
#include "inc/game/player.h"
#include "playergamestate.h"
#include <QObject>

// Представляет полное состояние игры для всех игроков в конкртеный момент в игре.
class ServerGameState: public QObject, public GameState
{
    Q_OBJECT
public:
    explicit ServerGameState(QObject *parent = nullptr);
    void updateBidState(const Bid &bid);
    void updatePlayState(const Card &card);
    bool isBidValid(const Bid &bid) const;
    bool isCardValid(const Card &card) const;
    const CardKit& getDeck();
    PlayerGameState getPlayerGameState(PlayerPosition player, QVector<ClientNetwork*> players,
                                       GameEvent gameEvent);
    void startMatch(qint32 maxRubbers);
    // Functions for unit testing purposes
    const QMap<PlayerPosition, CardKit>& getPlayerHands() const;
    void setPlayerHands(const QMap<PlayerPosition, CardKit> &playerHands);
signals:
    void gameEvent(GameEvent gameEvent);
private:
    // Список карт в руке каждого игрока.
    QMap<PlayerPosition, CardKit> playerHands;

    // Снимок карт в руке каждого игрока в начале раздачи.
    QMap<PlayerPosition, CardKit> playerHandsSnapshot;

    // Набор из 52 уникальных карт, которые раздаются в начале каждого раунда
    CardKit deck;

    // Количество последовательных проходов, сделанных на этапе торгов.
    qint8 passCount;
    qint32 maxRubbers;
    PlayerPosition determineTrickWinner() const;
    static Team getPlayerTeam(PlayerPosition position);
    void nextDeal();
    void nextTrick();
    void nextGame();
    void nextRubber();
};

#endif // SERVERGAMESTATE_H
