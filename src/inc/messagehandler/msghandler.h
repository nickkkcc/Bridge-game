#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <QObject>
#include "MsgType.h"
#include <QJsonObject>
#include <QJsonArray>
#include "inc/database/database.h"
#include "inc/network/clientnetwork.h"

class MsgHandler : public QObject
{
    Q_OBJECT
private:
    DataBase *base;
    QStringList questions{
                          "How Are You",
                          "GOR",
                          "DA"
                          };
    bool *bAllowNewClientConnection;
    int *maxPlayers;
    int *maxLoginLength;
    int *minLoginLength;
    QVector<ClientNetwork*> *clients;

    public:
        bool checkTryReg(QJsonObject &obj);
        bool tryLogin(QJsonObject &obj);
        MsgHandler(DataBase *base,
                   bool *bAllowNewClientConnection,
                   int *maxPlayers,
                   int *maxLoginLength,
                   int *minLoginLength,
                   QVector<ClientNetwork*> *clients,
                   QObject *parent);
    private:
       QJsonObject generateAnswer(const MsgType &type,
                            bool successful = true,
                            const QString &error = "",
                            const QString &uid = "0");

    };

#endif // MSGHANDLER_H
