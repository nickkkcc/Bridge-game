#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include "inc/enumeration/Enumiration.h"
#include "inc/game/aboutGameState/playergamestate.h"
#include <QObject>
#include <QWebSocket>

class ClientNetwork : public QObject
{
    Q_OBJECT
  public:
    explicit ClientNetwork(QObject *parent = nullptr, QString playerName = QString("Player"),
                           QWebSocket *clientSoc = nullptr, const QUuid &uuid = nullptr, const QUuid &alias = nullptr);
    void bidTurn();
    void moveTurn();
    void bidRejected(QString reason);
    void moveRejected(QString reason);
    void updateGameState(PlayerGameState state);

    QWebSocket *const getClientSoc() const;
    const bool getFinder() const;
    const QUuid &getLobbyOwnerUuid() const;
    QString getName() const;
    PlayerPosition getPosition() const;
    const Team getTeam() const;
    const Team getTeam(PlayerPosition position) const;
    const QUuid &getUuid() const;
    const QUuid getAlias() const;
    const QHash<QString, QUuid> &getClientFriendLogins() const;

    void setLobbyOwnerUuid(const QUuid &newLobbyOwnerUuid);
    void setFinder(bool newFinder);
    void setName(QString name);
    void setPosition(PlayerPosition position);
    void setTeam(Team newTeam);
    void setUuid(const QUuid &newUuid);
    void setClientSoc(QWebSocket *newClientSoc);
    void setAlias(const QUuid &newAlias);
    void setClientFriendLogins(const QHash<QString, QUuid> &newClientFriendLogins);

    ~ClientNetwork();
    double getScore() const;
    void setScore(double newScore);

    long getAllGameCount() const;
    void setAllGameCount(long newAllGameCount);

    long getWinGameCount() const;
    void setWinGameCount(long newWinGameCount);

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
    void rxAddFriend(QString login, ClientNetwork *const sender);
    void rxDeleteFriend(QString login, ClientNetwork *const sender);
    void rxRequestHistoryList(ClientNetwork *const sender);
    void rxDeleteAccount(ClientNetwork *const sender);
    void rxRequestScore(ClientNetwork *const sender);

  private:
    void txAll(QJsonObject data);
    void rxMessage(QJsonObject msgObj);
    void rxMoveSelectedDeserialization(QJsonObject card);
    void rxBidSelectedDeserialization(QJsonObject bid);

  private:
    QWebSocket *clientSoc = nullptr;
    QString name;
    QUuid uuid;
    PlayerPosition position = NONE_POSITION;
    Team team = Team::NONE_TEAM;
    QUuid lobbyOwnerUuid;
    QUuid alias;
    QHash<QString, QUuid> clientFriendLogins;
    double score;
    long allGameCount;
    long winGameCount;

    bool finder = false;
};

#endif // CLIENTNETWORK_H
