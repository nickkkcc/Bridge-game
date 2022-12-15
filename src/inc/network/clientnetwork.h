#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <QObject>
#include "inc/enumeration/Enumiration.h"
#include "inc/game/player.h"
#include "inc/game/aboutGameState/playergamestate.h"
#include <QWebSocket>

class ClientNetwork : public Player
{
    Q_OBJECT
public:
    explicit ClientNetwork(QObject *parent = nullptr, QString playerName = QString("Player"), QWebSocket* clientSoc = nullptr, QUuid* uuid = nullptr);

    void bidTurn();
    void moveTurn();
    void bidRejected(QString reason);
    void moveRejected(QString reason);
    void updateGameState(PlayerGameState state);
   // void gameTerminated(QString reason);
    QWebSocket *getClientSoc() const;
    bool getFinder() const;
    QUuid *getLobbyOwnerUuid() const;
    void setLobbyOwnerUuid(QUuid *newLobbyOwnerUuid);
    void setFinder(bool newFinder);
    ~ClientNetwork();

private slots:
    void rxAll(const QString& message);

public slots:
    void onClientDisconnected();

signals:
    void generalError(QString errorMsg);
    void clientDisconnected(ClientNetwork* sender);
    void rxCreateLobby(ClientNetwork* client);
    void rxCloseLobby(QUuid uuidLobby, ClientNetwork *sender);
    void rxAcceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork* client);
    void rxSelectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork* client);
    void rxInvitePlayers(QString logins, QUuid uuidLobby, ClientNetwork *sender);
    void rxStartGamePlayers(QUuid uuidLobby,  ClientNetwork *sender);
    void rxJoinLobby(bool join, ClientNetwork* sender);
    void rxAcceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork* sender);
    void rxMoveSelected(Card card);
    void rxBidSelected(Bid bid);

private:
    void txAll(QJsonObject data);
    void rxMessage(QJsonObject msgObj);
    void rxMoveSelectedDeserialization(QJsonObject card);
    void rxBidSelectedDeserialization(QJsonObject bid);

private:
    QWebSocket* clientSoc;
    QUuid *lobbyOwnerUuid;
    bool gameTerminatedOnce;
    bool finder = false;
};

#endif // CLIENTNETWORK_H
