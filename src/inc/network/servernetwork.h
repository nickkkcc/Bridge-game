#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QtWebSockets>
#include <QDebug>
#include <inc/lobbymanager/lobbymanager.h>

#include "inc/database/database.h"
#include "inc/messagehandler/msghandler.h"
#include "inc/network/clientnetwork.h"


class ServerNetwork : public QObject
{
    Q_OBJECT
  public:
    explicit ServerNetwork(QObject *parent = nullptr);
    ~ServerNetwork();
    void setPassword(QString password);
    bool initServer(quint16 port, int maxPlayers, int maxLoginLength, int minLoginLength);
    void stopListening();
    void deleteAllClients();

  private slots:
    void connectClient();
    void validateClientText(const QString &message);
    void disconnectTempClient();
    void disconnectClient(ClientNetwork *const sender);

  signals:
    void connectionResult(int status,quint16 uport, QString errorMsg);
    void generalError(QString errorMsg);
    void playerDisconnected(QString playerName);
    void startTimer();

  private:
    QVector<ClientNetwork *> clients;
    QVector<QWebSocket *> clientSocTemp;
    QWebSocketServer *server = nullptr;
    LobbyManager *lobbyManager = nullptr;
    DataBase *dataBase = nullptr;
    MsgHandler *msgHandler = nullptr;

    bool bAllowNewClientConnection;
    int maxPlayers = 100;
    int maxLoginLength = 15;
    int minLoginLength = 3;
    quint16 port;

  private:
    void connectClientToLobbyManager(ClientNetwork *const client);
    void returnClientToLobby(Lobby *const lobby, LobbyManager *const lobbyManager, QWebSocket *client,
                             QString clientName, const QUuid &clientUuid);
};

#endif // SERVERNETWORK_H
