#include "database.h"

DataBase::DataBase(QString dataBaseName, QString hostName, int port, QString userName, QString password,
                   QString driverName, QObject *parent)
    : QObject(parent)
{
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(dataBaseName);
    qx::QxSqlDatabase::getSingleton()->setDriverName(driverName);
    qx::QxSqlDatabase::getSingleton()->setHostName(hostName);
    qx::QxSqlDatabase::getSingleton()->setUserName(userName);
    qx::QxSqlDatabase::getSingleton()->setPassword(password);
    qx::QxSqlDatabase::getSingleton()->setPort(port);
}

bool DataBase::writeUserToBase(QString login, QUuid alias, QString password,
                               QuestionsType::enum_QuestionsType questionType, QString answer, double score)
{

    users_ptr.reset(new Users());
    users_ptr->setLogin(login);
    users_ptr->setAlias(alias);
    users_ptr->setPassword(password);
    users_ptr->setQuestion_answer(answer);
    users_ptr->setQuestion_type(questionType);
    users_ptr->setScore(score);

    QSqlError err = qx::dao::insert(users_ptr);
    switch (err.type())
    {
    case QSqlError::NoError:
        return true;
    case QSqlError::ConnectionError:
    case QSqlError::StatementError:
    case QSqlError::TransactionError:
    case QSqlError::UnknownError:
        qInfo() << "Server: @writeUserToBase" << err.text();
        break;
    }
    return false;
}

// Считывает клиента из базы данных без связей.
bool DataBase::readUserFromBase(QString login)
{

    users_ptr.reset(new Users());
    qx_query query;
    query.where("t_users.login").isEqualTo(login);
    QSqlError err = qx::dao::fetch_by_query(query, users_ptr);
    switch (err.type())
    {
    case QSqlError::NoError:
        break;
    case QSqlError::ConnectionError:
    case QSqlError::StatementError:
    case QSqlError::TransactionError:
    case QSqlError::UnknownError:
        qInfo() << "Server @readUserFromBase:" << err.text();
        return false;
    }

    if (users_ptr->getLogin().isNull())
    {
        return false;
    }
    return true;
}

// Считывает клиента из базы данных со всеми связями.
bool DataBase::readUserFromBaseWithRelations(QString login)
{

    users_ptr.reset(new Users());
    qx_query query;
    query.where("t_users.login").isEqualTo(login);
    QSqlError err = qx::dao::fetch_by_query_with_all_relation(query, users_ptr);
    switch (err.type())
    {
    case QSqlError::NoError:
        break;
    case QSqlError::ConnectionError:
    case QSqlError::StatementError:
    case QSqlError::TransactionError:
    case QSqlError::UnknownError:
        qInfo() << "Server @readUserFromBaseWithRelations:" << err.text();
        break;
    }
    if (users_ptr->getLogin().isNull())
    {
        return false;
    }
    return true;
}

// Добавление друзей по указанному клиенту.
bool DataBase::addFriend(ClientNetwork *const client, ClientNetwork *const newFriend)
{
    if (readUserFromBase(newFriend->getName()))
    {
        QSharedPointer<Users> newFriendClient = users_ptr;

        readUserFromBase(client->getName());
        QSharedPointer<Friends> friends = QSharedPointer<Friends>(new Friends());

        friends->setLogin(newFriendClient->getLogin());
        friends->setAlias(newFriendClient->getAlias());
        friends->setPassword(newFriendClient->getPassword());
        friends->setQuestion_anwer(newFriendClient->getQuestion_answer());
        friends->setQuestion_type(newFriendClient->getQuestion_type());
        friends->setScore(newFriendClient->getScore());
        friends->setUsers_id(users_ptr);
        QVector<QSharedPointer<Friends>> friendVec;
        friendVec.append(friends);
        users_ptr->setlist_of_Friends(friendVec);

        QSqlError err = qx::dao::update_with_all_relation(users_ptr);
        switch (err.type())
        {
        case QSqlError::NoError:
            break;
        case QSqlError::ConnectionError:
        case QSqlError::StatementError:
        case QSqlError::TransactionError:
        case QSqlError::UnknownError:
            qInfo() << "Server: @addFriend --->" << err.text();
            return false;
        }
        return true;
    }
    return false;
}

// Функция добавляет историю для конкретного клиента.
bool DataBase::addHistory(ClientNetwork *const client, History history)
{
    if (client)
    {
        if (readUserFromBase(client->getName()))
        {
            QVector<QSharedPointer<History>> historyList;
            QVector<QSharedPointer<Users>> usersList;
            usersList.append(users_ptr);
            history.setlist_of_Users(usersList);
            historyList.append(QSharedPointer<History>(&history));
            users_ptr->setlist_of_History(historyList);
            users_ptr->setScore(users_ptr->getScore() + 1);
            QSqlError err = qx::dao::update_with_all_relation(users_ptr);
            switch (err.type())
            {
            case QSqlError::NoError:
                break;
            case QSqlError::ConnectionError:
            case QSqlError::StatementError:
            case QSqlError::TransactionError:
            case QSqlError::UnknownError:
                qInfo() << "Server: @addHistory --->" << err.text();
                return false;
            }
            return true;
        }
    }
    return false;
}

const QString DataBase::getUserLogin() const
{

    return users_ptr->getLogin();
}

const QString DataBase::getUserPassword() const
{
    return users_ptr->getPassword();
}

const QUuid DataBase::getUserAlias() const
{
    return users_ptr->getAlias();
}

const int DataBase::getUserQuestionType() const
{
    return users_ptr->getQuestion_type();
}

const QString DataBase::getUseranswer() const
{
    return users_ptr->getQuestion_answer();
}

double DataBase::getUserScore()
{
    return users_ptr->getScore();
}

// Функция считывает указанного клиента из базы данных и дает историю его игр в виде Json объекта.
const QJsonObject DataBase::getHistory(ClientNetwork *const user)
{
    if (user->getName() != users_ptr->getLogin())
    {
        bool result = readUserFromBaseWithRelations(user->getName());
        if (!result)
        {
            return QJsonObject();
        }
    }
    QJsonObject tx;
    QJsonArray history_list;
    QJsonObject history_obj;
    for (const QSharedPointer<History> &history : users_ptr->getlist_of_History())
    {
        history_obj["game_start"] = history->getGame_start().toString();
        history_obj["game_end"] = history->getGame_end().toString();
        history_obj["owner_login"] = history->getOwner_login();
        history_obj["owner_alias"] = history->getOwner_alias().toString(QUuid::WithoutBraces);
        history_obj["winner_team"] = history->getWinner_team();
        history_obj["player_n_login"] = history->getPlayer_N_login();
        history_obj["player_n_alias"] = history->getPlayer_N_alias().toString(QUuid::WithoutBraces);
        history_obj["player_s_login"] = history->getPlayer_S_login();
        history_obj["player_s_alias"] = history->getPlayer_S_alias().toString(QUuid::WithoutBraces);
        history_obj["player_e_login"] = history->getPlayer_E_login();
        history_obj["player_e_alias"] = history->getPlayer_E_alias().toString(QUuid::WithoutBraces);
        history_obj["player_w_login"] = history->getPlayer_W_login();
        history_obj["player_w_alias"] = history->getPlayer_W_alias().toString(QUuid::WithoutBraces);
        history_obj["total_score_ns"] = history->getTotal_score_NS();
        history_obj["total_score_ew"] = history->getTotal_score_EW();
        history_list.append(history_obj);
    }
    tx["history_list"] = history_list;
    return tx;
}

const QHash<QString, QUuid> DataBase::getFriendsList(ClientNetwork *const user)
{
    QHash<QString, QUuid> friendList;
    if (readUserFromBaseWithRelations(user->getName()))
    {
        QVector<QSharedPointer<Friends>> friends = users_ptr->getlist_of_Friends();
        for (const QSharedPointer<Friends> &userFriend : qAsConst(friends))
        {
            friendList.insert(userFriend->getLogin(), userFriend->getAlias());
        }
    }
    return friendList;
}

DataBase::~DataBase()
{

    qx::QxSqlDatabase::getSingleton()->closeAllDatabases();
    qInfo() << "Server: DataBase ---> deleted";
}
