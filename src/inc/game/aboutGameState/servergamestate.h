#ifndef SERVERGAMESTATE_H
#define SERVERGAMESTATE_H

#include "gamestate.h"
#include "playergamestate.h"
#include "qtimer.h"
#include <QObject>
#include <inc/network/clientnetwork.h>

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
    PlayerGameState getPlayerGameState(PlayerPosition player, const QVector<ClientNetwork *> &players,
                                       GameEvent gameEvent);
    void startMatch(qint32 maxRubbers);
    const QMap<PlayerPosition, CardKit>& getPlayerHands() const;
    void setPlayerHands(const QMap<PlayerPosition, CardKit> &playerHands);
    GameEvent getCurrentGameEvent() const;
    void setCurrentGameEvent(GameEvent newCurrentGameEvent);

  signals:
    void gameEvent(GameEvent gameEvent);
    void nexPlayerTurn();
  private slots:
    void doTrickDelay();

  private:
    // Список карт в руке каждого игрока.
    QMap<PlayerPosition, CardKit> playerHands;

    // Снимок карт в руке каждого игрока в начале раздачи.
    QMap<PlayerPosition, CardKit> playerHandsSnapshot;

    // Набор из 52 уникальных карт, которые раздаются в начале каждого раунда
    CardKit deck;
    int variantEnd = 0;

    // Количество последовательных проходов, сделанных на этапе торгов.
    qint8 passCount;
    qint32 maxRubbers;
    PlayerPosition determineTrickWinner() const;
    static Team getPlayerTeam(PlayerPosition position);
    void nextDeal();
    void nextTrick();
    void nextGame();
    void nextRubber();
    QTimer trickEnd;
    GameEvent currentGameEvent;
};

#endif // SERVERGAMESTATE_H
