#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QtWebSockets>
#include <QDebug>

#include "inc/database/database.h"
#include "inc/messagehandler/msghandler.h"

class ServerNetwork : public QObject
{
    Q_OBJECT
public:
    explicit ServerNetwork(QObject *parent = nullptr);
    ~ServerNetwork();
    //void forceError();

    QTcpSocket* getPlayerSoc(QString playerName);
    void setPassword(QString password);
    bool initServer(quint16 port,
                    int maxPlayers,
                    int maxLoginLength,
                    int minLoginLength);
    void stopListening();
    void deleteAllClients();

private slots:
    void connectClient();
//    void validateClientText(const QString &message);
    void validateClientBinary(const QByteArray &message);
    void disconnectClient();

signals:
    void connectionResult(int status,quint16 port, QString errorMsg);
    void generalError(QString errorMsg);

    void playerJoined(QString playerName);
    void playerDisconnected(QString playerName);
    void started(bool start);

private:
    QVector<QString> playerNames;
    QVector<QWebSocket*> clientSoc;
    QVector<QWebSocket*> clientSocTemp;
    QVector<QUuid> uidClients;
    QWebSocketServer* server;
    bool bAllowNewClientConnection;
    int maxPlayers = 100;
    int maxLoginLength = 100;
    int minLoginLength = 3;
    quint16 port;
    DataBase *base;
    MsgHandler *msgHandler;


};

#endif // SERVERNETWORK_H
