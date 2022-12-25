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
    explicit ClientNetwork(QObject *parent = nullptr, QString playerName = QString("Player"),
                           QWebSocket *clientSoc = nullptr, const QUuid &uuid = nullptr);
    void bidTurn();
    void moveTurn();
    void bidRejected(QString reason);
    void moveRejected(QString reason);
    void updateGameState(PlayerGameState state);
    // void gameTerminated(QString reason);
    QWebSocket *const getClientSoc() const;
    const bool getFinder() const;
    const QUuid &getLobbyOwnerUuid() const;
    void setLobbyOwnerUuid(const QUuid &newLobbyOwnerUuid);
    void setFinder(bool newFinder);
    ~ClientNetwork();

    QVector<ClientNetwork *> &getClientFriends();
    void setClientFriends(const QVector<ClientNetwork *> &newClientFriends);

  private slots:
    void rxAll(const QString &message);

  public slots:
    void onClientDisconnected();

  signals:
    void generalError(QString errorMsg);
    void clientDisconnected(ClientNetwork *const sender);
    void rxCreateLobby(ClientNetwork *const client);
    void rxCloseLobby(const QUuid &uuidLobby, ClientNetwork *const sender);
    void rxAcceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork *const client);
    void rxSelectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork *const client);
    void rxInvitePlayers(QString logins, QUuid uuidLobby, ClientNetwork *const sender);
    void rxStartGamePlayers(QUuid uuidLobby, ClientNetwork *const sender);
    void rxJoinLobby(bool join, ClientNetwork *const sender);
    void rxAcceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork *const sender);
    void rxMoveSelected(Card card);
    void rxBidSelected(Bid bid);

  private:
    void txAll(QJsonObject data);
    void rxMessage(QJsonObject msgObj);
    void rxMoveSelectedDeserialization(QJsonObject card);
    void rxBidSelectedDeserialization(QJsonObject bid);

  private:
    QWebSocket *clientSoc = nullptr;
    QUuid lobbyOwnerUuid;
    QVector<ClientNetwork *> clientFriends;

    bool gameTerminatedOnce;
    bool finder = false;
};

#endif // CLIENTNETWORK_H
