#include "inc/playerslist.h"

PlayersList::PlayersList(QObject *parent) : QObject{parent}
{
}

QStringList PlayersList::friendsAliases() const
{
    return m_friendsAliases;
}

void PlayersList::setFriendsAliases(const QStringList &newFriendsAliases)
{
    if (m_friendsAliases == newFriendsAliases)
        return;
    m_friendsAliases = newFriendsAliases;
    emit friendsAliasesChanged();
}

QStringList PlayersList::playersAliases() const
{
    return m_playersAliases;
}

void PlayersList::setPlayersAliases(const QStringList &newPlayersAliases)
{
    if (m_playersAliases == newPlayersAliases)
        return;
    m_playersAliases = newPlayersAliases;
    emit playersAliasesChanged();
}

void PlayersList::addFriend(QString login, QString alias)
{
    m_friends.append(login);
    m_friendsAliases.append(alias.mid(0, 15));
}

void PlayersList::addPlayer(QString login, QString alias)
{
    if (!m_friends.contains(login))
    {

        m_players.append(login);
        m_playersAliases.append(alias.mid(0, 15));
    }
}

void PlayersList::parsePlayersList(const QJsonArray &arrayFriends, const QJsonArray &arrayPlayers)
{

    if (!m_friends.isEmpty())
    {
        m_friends.clear();
        m_friendsAliases.clear();
    }
    if (!m_players.isEmpty())
    {
        m_players.clear();
        m_playersAliases.clear();
    }
    if (!arrayPlayers.isEmpty())
    {
        for (const auto &obj : arrayFriends)
        {
            addFriend(obj["login"].toString(), obj["alias"].toString());
        }

        for (const auto &obj : arrayPlayers)
        {
            addPlayer(obj["login"].toString(), obj["alias"].toString());
        }
        emit friendsAliasesChanged();
        emit playersAliasesChanged();
        return;
    }
    emit friendsAliasesChanged();
    emit playersAliasesChanged();
}

QString PlayersList::removeLoginPlayer(int index)
{
    QString login = m_players[index];
    m_players.removeAt(index);
    m_playersAliases.removeAt(index);
    emit playersAliasesChanged();
    return login;
}

QString PlayersList::removeLoginFriend(int index)
{
    QString login = m_friends[index];
    m_friends.removeAt(index);
    m_friendsAliases.removeAt(index);
    emit friendsAliasesChanged();
    return login;
}

void PlayersList::clearAll()
{
    m_playersAliases.clear();
    m_players.clear();
    m_friendsAliases.clear();
    m_friends.clear();
    emit playersAliasesChanged();
    emit friendsAliasesChanged();
}

QString PlayersList::getLoginPlayer(int index)
{
    return m_players.at(index);
}

QString PlayersList::getLoginFriend(int index)
{
    return m_friends.at(index);
}
