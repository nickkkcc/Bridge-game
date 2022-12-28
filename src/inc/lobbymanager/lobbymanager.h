#ifndef LOBBYMANAGER_H
#define LOBBYMANAGER_H

#include "lobby.h"

#include <QObject>
#include <QTimer>

#include <inc/network/clientnetwork.h>
#include <inc/enumeration/Enumiration.h>

class LobbyManager : public QObject
{
    Q_OBJECT
  public:
    explicit LobbyManager(QObject *const parent = nullptr, int maxLobbyCount = 1,
                          QVector<ClientNetwork *> *const clients = nullptr);
    ~LobbyManager();
    const int getLobbyCount() const;
    Lobby *const findLobby(QUuid uuidLobby) const;
    Lobby *const findLobby(ClientNetwork *const owner) const;
    Lobby *const findLobby(QString playerName) const;
    Lobby *const findLobbyFromTempClient(QString playerName) const;
    ClientNetwork *const findPlayer(QString playerName) const;
    ClientNetwork *const findPlayer(QUuid playerUuid) const;
    bool setReturnedClient(Lobby *const lobby, PlayerPosition position, ClientNetwork *const client);

  public slots:
    void createLobby(ClientNetwork *const client);
    void closeLobby(const QUuid &uuidLobby, ClientNetwork *const sender);
    void acceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork *const client);
    void selectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork *const client);
    void invitePlayers(QString logins, QUuid uuidLobby, ClientNetwork *const sender);
    void startGame(QUuid uuidLobby, ClientNetwork *const sender);
    void startTimer();
    void clientDisconnected(ClientNetwork *const sender);
    void joinLobby(bool join, ClientNetwork *const sender);
    void acceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork *const sender);

  signals:
    void disconnectClient(ClientNetwork *const sender);
    void sendUpdateGameState(GameEvent event);

  private slots:
    void onPlayersOnline();
    void onPlayerCount();
    void matchEnd(Lobby *const sender);

  private:
    static constexpr quint8 maxTeamPLayers = 10;
    int maxLobbyCount;
    QVector<ClientNetwork *> *clients = nullptr;
    QVector<Lobby *> lobbies;
    QTimer *timer = nullptr;

  private:
    void lobbyClose(Lobby *const lobby);
    void txAll(QJsonObject tx, ClientNetwork *const sender);
    void sendSelectTeamClient(QUuid uuidLobby, ClientNetwork *const client);
};

#endif // LOBBYMANAGER_H
