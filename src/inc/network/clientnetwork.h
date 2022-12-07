#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <QObject>
#include "inc/enumeration/Enumiration.h"
#include "inc/game/player.h"
#include <QWebSocket>

class ClientNetwork : public Player
{
    Q_OBJECT
public:
    explicit ClientNetwork(QObject *parent = nullptr, QString playerName = QString("Player"), QWebSocket* clientSoc = nullptr, QUuid* uuid = nullptr);

    QWebSocket *getClientSoc() const;
    ~ClientNetwork();

    bool getFinder() const;

    QUuid *getLobbyOwnerUuid() const;

    void setLobbyOwnerUuid(QUuid *newLobbyOwnerUuid);

    void setFinder(bool newFinder);

private slots:
    void rxAll(const QString& message);
    void socketError(QAbstractSocket::SocketError socError);
public slots:
        void onClientDisconnected();

signals:
    void generalError(QString errorMsg);
//    void bidSelected(Bid bid);
//    void moveSelected(Card card);
    void messageGenerated(QString msg);
    void clientDisconnected(ClientNetwork* sender);

    void rxCreateLobby(ClientNetwork* client);
    void rxCloseLobby(QUuid uuidLobby, ClientNetwork *sender);
    void rxAcceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork* client);
    void rxSelectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork* client);
    void rxInvitePlayers(QString logins, QUuid uuidLobby, ClientNetwork *sender);
    void rxStartGamePlayers(QUuid uuidLobby,  ClientNetwork *sender);
    // New
    void rxJoinLobby(bool join, ClientNetwork* sender);
    void rxAcceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork* sender);

private:
    void txAll(QJsonObject data);
    void rxBidSelected(QJsonObject bidObj);
    void rxMoveSelected(QJsonObject moveObj);
    void rxMessage(QJsonObject msgObj);


    QWebSocket* clientSoc;
    QUuid *lobbyOwnerUuid;
    bool gameTerminatedOnce;
    bool finder = false;
};

#endif // CLIENTNETWORK_H
