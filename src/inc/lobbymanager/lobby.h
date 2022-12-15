#ifndef LOBBY_H
#define LOBBY_H

#include <QObject>
#include <inc/game/aboutGameState/servergamestate.h>
#include "inc/network/clientnetwork.h"
#include "inc/enumeration/Enumiration.h"


class Lobby : public QObject
{
    Q_OBJECT
public:
    explicit Lobby(QObject *parent = nullptr,
                   ClientNetwork *owner = nullptr);

    bool addPlayer(Team team, ClientNetwork* client);
    bool deletePlayer(Team team, ClientNetwork* client);


    void startMatch();

    void setFreeSpotsNS(int newFreeSpotsNS);
    void setFreeSpotsEW(int newFreeSpotsEW);
    void setOwner(ClientNetwork *newOwner);
    void setOwnerName(const QString &newOwnerName);
    void setGameStarted(bool newGameStarted);
    void setAcceptedPlayerCount(int newAcceptedPlayerCount);
    void setMaxPlayerSize(int newMaxPlayerSize);
    void setMaxRubber(int newMaxRubber);

    QUuid &getUuid();
    int getFreeSpotsNS() const;
    int getFreeSpotsEW() const;
    bool getGameStarted() const;
    int getPlayerCount();
    QVector<ClientNetwork*> &getPlayers();
    const QVector<QString> &getPlayerNames() const;
    int getAcceptedPlayerCount() const;
    int getMaxPlayerSize() const;
    const QString &getOwnerName() const;
    ClientNetwork *getOwner() const;
    ServerGameState *getGameState() const;

    ~Lobby();

public slots:
    void rxBidSelected(Bid bid);
    void rxMoveSelected(Card card);
    void gameEventOccured(GameEvent event);
    void setMatchEnded(bool isCompleted);
private:
    void sendUpdatedGameStateToClients(GameEvent event);
    ClientNetwork* getClient(PlayerPosition position);
    void nextPlayerTurn();
private:
    // Класс игровой логики для всего лобби в общем.
    ServerGameState* gameState = nullptr;

    // Поля для параметров игроков.
    QVector<ClientNetwork*> players{4};
    QVector<QString> playerNames{4};

    // Поля для создателя лобби.
    ClientNetwork *owner;
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
};

#endif // LOBBY_H
