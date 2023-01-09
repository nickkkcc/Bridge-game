#pragma once

#include <QJsonArray>
#include <QObject>

class PlayersList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList friendsAliases READ friendsAliases WRITE setFriendsAliases NOTIFY friendsAliasesChanged)
    Q_PROPERTY(QStringList playersAliases READ playersAliases WRITE setPlayersAliases NOTIFY playersAliasesChanged)

  public:
    explicit PlayersList(QObject *parent = nullptr);

    QStringList friendsAliases() const;
    void setFriendsAliases(const QStringList &newFriendsAliases);

    QStringList playersAliases() const;
    void setPlayersAliases(const QStringList &newPlayersAliases);

    void parsePlayersList(const QJsonArray &arrayFriends, const QJsonArray &arrayPlayers);
    QString removeLoginPlayer(int index);
    QString removeLoginFriend(int index);
    void clearAll();
    QString getLoginPlayer(int index);
    QString getLoginFriend(int index);
  signals:

    void friendsAliasesChanged();

    void playersAliasesChanged();

  private:
    void addFriend(QString login, QString alias);
    void addPlayer(QString login, QString alias);

  private:
    QStringList m_players;
    QStringList m_friends;
    QStringList m_playersAliases;
    QStringList m_friendsAliases;
};
