#ifndef LOBBY_H
#define LOBBY_H

#include <QObject>
#include "inc/network/clientnetwork.h"
#include "inc/enumeration/Enumiration.h"


class Lobby : public QObject
{
    Q_OBJECT
public:
    explicit Lobby(QObject *parent = nullptr,
                   ClientNetwork *owner = nullptr);
    QUuid &getUuid();

    void setMaxRubber(int newMaxRubber);

    int getFreeSpots() const;

    ClientNetwork *getOwner() const;

    bool addPlayer(Team team, ClientNetwork* client);
    bool deletePlayer(Team team, ClientNetwork* client);

    int getFreeSpotsNS() const;

    void setFreSpotsEW(int newFreSpotsEW);

    void setFreeSpotsNS(int newFreeSpotsNS);

    void setFreeSpotsEW(int newFreeSpotsEW);

    int getFreeSpotsEW() const;

    bool getGameStarted() const;

    int getPlayerCount();

    QVector<ClientNetwork*> &getPlayers();

    void setOwner(ClientNetwork *newOwner);

    const QVector<QString> &getPlayerNames() const;

    const QString &getOwnerName() const;
    void setOwnerName(const QString &newOwnerName);

    void setGameStarted(bool newGameStarted);

    int getAcceptedPlayerCount() const;
    void setAcceptedPlayerCount(int newAcceptedPlayerCount);

    int getMaxPlayerSize() const;
    void setMaxPlayerSize(int newMaxPlayerSize);

public slots:


signals:
private slots:
private:
    QVector<ClientNetwork*> players{4};
    QVector<QString> playerNames{4};
    ClientNetwork *owner;
    QString ownerName;
    QUuid uuid;
    int freeSpotsNS = 2;
    int freeSpotsEW = 2;
    int maxRubber = 1;
    bool gameStarted = false;
    int maxPlayerSize = 4;
    int acceptedPlayerCount = 0;
    int size = 0;
};

#endif // LOBBY_H
