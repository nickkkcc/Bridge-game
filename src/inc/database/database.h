#ifndef DATABASE_H
#define DATABASE_H

#include "inc/network/clientnetwork.h"
#include <QObject>
#include <QxOrm.h>
#include <bridgeServer_all_include.gen.h>
#include <bridgeServer_export.gen.h>
#include <bridgeServer_precompiled_header.gen.h>

class DataBase : public QObject
{
    Q_OBJECT
  public:
    explicit DataBase(QString dataBaseName, QString hostName, int port, QString userName, QString password,
                      QString driverName = "QPSQL", QObject *parent = nullptr);
    bool writeUserToBase(QString login, QUuid alias, QString password, QuestionsType::enum_QuestionsType questionType,
                         QString answer, double score);
    bool readUserFromBase(QString login);
    bool readUserFromBaseWithRelations(QString login);
    bool addFriend(ClientNetwork *const client, ClientNetwork *const newFriend);
    bool addHistory(ClientNetwork *const client, History history);

    const QString getUserLogin() const;
    const QString getUserPassword() const;
    const QUuid getUserAlias() const;
    const int getUserQuestionType() const;
    const QString getUseranswer() const;
    double getUserScore();
    const QJsonObject getHistory(ClientNetwork *const user);
    const QHash<QString, QUuid> getFriendsList(ClientNetwork *const user);

    ~DataBase();

  private:
    QSharedPointer<Users> users_ptr;
    QSharedPointer<Friends> friend_ptr;
    QSharedPointer<History> history;
  signals:
};

#endif // DATABASE_H
