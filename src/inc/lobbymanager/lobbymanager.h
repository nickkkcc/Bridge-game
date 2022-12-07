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
    explicit LobbyManager(QObject *parent = nullptr, int maxLobbyCount = 1, QVector<ClientNetwork*> *clients = nullptr);
    int getLobbyCount();
    Lobby *findLobby(QUuid uuidLobby);
    Lobby *findLobby(ClientNetwork* owner);
    Lobby *findLobby(QString playerName);
    ClientNetwork* findPlayer(QString playerName);
    ClientNetwork *findPlayer(QUuid plaerUuid);

public slots:
    void createLobby(ClientNetwork* client);
    void closeLobby(QUuid uuidLobby, ClientNetwork *sender);
    void acceptSelectTeam(Team team, QUuid uuidLobby, ClientNetwork* client);
    void selectTeamAdmin(Team team, QUuid uuidLobby, ClientNetwork* client);
    void invitePlayers(QString logins, QUuid uuidLobby, ClientNetwork *sender);
    void startGame(QUuid uuidLobby, ClientNetwork *sender);
    void startTimer();
    void clientDisconnected(ClientNetwork* sender);
    void joinLobby(bool join, ClientNetwork* sender);
    void acceptInvitePlayers(QUuid uuidLobby, bool successful, ClientNetwork* sender);

signals:
    void disconnectClient(ClientNetwork* sender);

private slots:
    void onPlayersOnline();
    void onPlayerCount();


private:
    int maxLobbyCount;
    QVector<ClientNetwork*> *clients;
    QVector<Lobby*> lobbies;
    QTimer *timer;
private:
    void lobbyClose(Lobby* lobby);
    void txAll(const QJsonObject &tx);
    QHash<QString, QString> getFinderPlayers();
    void txAll(QJsonObject tx, ClientNetwork* sender);
    void sendSelectTeamClient(QUuid uuidLobby, ClientNetwork* client);
};

#endif // LOBBYMANAGER_H
