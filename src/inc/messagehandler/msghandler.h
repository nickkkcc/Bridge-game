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
    DataBase *base = nullptr;
    QStringList questions{"Какой у вас был первый номер телефона?", "Кто ваш любимый учитель?",
                          "Кто ваш любимый певец?", "Как зовут вашу мать?"};
    bool *bAllowNewClientConnection = nullptr;
    int *maxPlayers = nullptr;
    int *maxLoginLength = nullptr;
    int *minLoginLength = nullptr;
    QVector<ClientNetwork *> *clients = nullptr;

  public:
    bool checkTryReg(QJsonObject &obj);
    bool tryLogin(QJsonObject &obj);
    explicit MsgHandler(DataBase *base, bool *bAllowNewClientConnection, int *maxPlayers, int *maxLoginLength,
                        int *minLoginLength, QVector<ClientNetwork *> *clients, QObject *parent);
    ~MsgHandler();

  private:
    QJsonObject generateAnswer(const MsgType &type, bool successful = true, const QString &error = "",
                               const QString &uid = "0");
};

#endif // MSGHANDLER_H
