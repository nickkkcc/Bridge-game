#ifndef LOBBY_H
#define LOBBY_H

#include "bridgeServer_all_include.gen.h"
#include "inc/enumeration/Enumiration.h"
#include "inc/network/clientnetwork.h"
#include <QObject>
#include <inc/game/aboutGameState/servergamestate.h>

class Lobby : public QObject
{
    Q_OBJECT
  public:
    static QString bidCallToString(BidCall bid);
    static QString cardRankToString(CardRank rank);
    static QString cardSuitToString(CardSuit suit);
    static QString gameEventToString(GameEvent event);
    static QString gamePhaseToString(GamePhase phase);
    static QString playerPositionToString(PlayerPosition position);
    static QString teamToString(Team team);

    explicit Lobby(QObject *const parent = nullptr, ClientNetwork *const owner = nullptr);

    bool addPlayer(Team team, ClientNetwork *const client);
    bool deletePlayer(Team team, ClientNetwork *const client);

    void startMatch();

    void setFreeSpotsNS(int newFreeSpotsNS);
    void setFreeSpotsEW(int newFreeSpotsEW);
    void setOwner(ClientNetwork *const newOwner);
    void setOwnerName(const QString &newOwnerName);
    void setGameStarted(bool newGameStarted);
    void setAcceptedPlayerCount(int newAcceptedPlayerCount);
    void setMaxPlayerSize(int newMaxPlayerSize);
    void setMaxRubber(int newMaxRubber);

    const QUuid &getUuid() const;
    const int getFreeSpotsNS() const;
    const int getFreeSpotsEW() const;
    const bool getGameStarted() const;
    const int getPlayerCount() const;
    QVector<ClientNetwork *> &getPlayers();
    QVector<QString> &getPlayerNames();
    const int getAcceptedPlayerCount() const;
    const int getMaxPlayerSize() const;
    const QString &getOwnerName() const;
    ClientNetwork *const getOwner() const;
    ServerGameState *const getGameState() const;

    ~Lobby();

    QSet<ClientNetwork *> &getTempPlayers();

    const int getSize() const;
    void setSize(int newSize);

    const GameEvent getLastGameEvent() const;

    QHash<QString, PlayerPosition> &getDisconnectedPlayers();

    History &getGameHistory();
    void setGameHistory(History newGameHistory);

  signals:
    void sendMatchEndToLM(Lobby *const sender);
    void sendHistoryToLM(const History &history);

  public slots:
    void rxBidSelected(Bid bid);
    void rxMoveSelected(Card card);
    void gameEventOccured(GameEvent event);
    void setMatchEnded(bool isCompleted);
    void nextPlayerTurn();

  private:
    void sendUpdatedGameStateToClients(GameEvent event);
    ClientNetwork *getClient(PlayerPosition position) const;

  private:
    // Класс игровой логики для всего лобби в общем.
    ServerGameState *gameState = nullptr;
    // Поля для параметров игроков.
    QVector<ClientNetwork *> players{4};
    QVector<QString> playerNames{4};
    // Сохраняем игроков, которые находят на стадии выбора команды.
    QSet<ClientNetwork *> tempPlayers;
    QHash<QString, PlayerPosition> disconnectedPlayers;

    // Поля для создателя лобби.
    ClientNetwork *owner = nullptr;
    QString ownerName;

    // Идентификатор лобби.
    QUuid uuid;

    // Свободные места для команд.
    int freeSpotsNS = 2;
    int freeSpotsEW = 2;

    // Максимальное количество рубберов.
    int maxRubber = 1;

    // Началась ли игра.
    bool gameStarted = false;

    // Максимальное кол-во игроков в лобби (сделать статическим)
    int maxPlayerSize = 4;

    // Количество согласившихся к подключению к лобби игроков.
    int acceptedPlayerCount = 0;

    // Флаг "Ход завершен"
    bool moveComplete;

    // Флаг "Матч завершен"
    bool gameComplete;

    // Количество игроков в лобби на данный момент.
    int size = 0;

    // Последнее игровое событие (перед PLAY_STOP)
    GameEvent lastGameEvent;

    History gameHistory;
};

#endif // LOBBY_H
