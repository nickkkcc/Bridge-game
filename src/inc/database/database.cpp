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
                               QuestionsType::enum_QuestionsType questionType, QString answer, double score,
                               unsigned long winGameCount, unsigned long allGameCount)
{

    users_ptr.reset(new Users());
    users_ptr->setLogin(login);
    users_ptr->setAlias(alias);
    users_ptr->setPassword(password);
    users_ptr->setQuestion_answer(answer);
    users_ptr->setQuestion_type(questionType);
    users_ptr->setScore(score);
    users_ptr->setAll_game_count(allGameCount);
    users_ptr->setWin_game_count(winGameCount);

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
bool DataBase::addFriend(ClientNetwork *const client, QString friendLogin)
{
    if (readUserFromBase(friendLogin) && !client->getClientFriendLogins().keys().contains(friendLogin))
    {
        QSharedPointer<Users> newFriendClient = users_ptr;

        readUserFromBase(client->getName());
        QSharedPointer<Friends> friends = QSharedPointer<Friends>(new Friends());

        friends->setLogin(newFriendClient->getLogin());
        friends->setAlias(newFriendClient->getAlias());
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
bool DataBase::addHistory(ClientNetwork *const client, const History &history, bool winner)
{
    if (client)
    {
        if (readUserFromBaseWithRelations(client->getName()))
        {
            QVector<QSharedPointer<History>> oldHistoryList = users_ptr->getlist_of_History();

            QSharedPointer<History> newHistory = QSharedPointer<History>(new History());
            newHistory->setGame_start(history.getGame_start());
            newHistory->setGame_end(history.getGame_end());
            newHistory->setOwner_alias(history.getOwner_alias());
            newHistory->setOwner_login(history.getOwner_login());
            newHistory->setPlayer_N_alias(history.getPlayer_N_alias());
            newHistory->setPlayer_N_login(history.getPlayer_N_login());

            newHistory->setPlayer_S_alias(history.getPlayer_S_alias());
            newHistory->setPlayer_S_login(history.getPlayer_S_login());

            newHistory->setPlayer_E_alias(history.getPlayer_E_alias());
            newHistory->setPlayer_E_login(history.getPlayer_E_login());

            newHistory->setPlayer_W_alias(history.getPlayer_W_alias());
            newHistory->setPlayer_W_login(history.getPlayer_W_login());

            newHistory->setWinner_team(history.getWinner_team());
            newHistory->setTotal_score_NS(history.getTotal_score_NS());
            newHistory->setTotal_score_EW(history.getTotal_score_EW());

            // Если игрок находился в команде, которая выиграла. Сетаем ему выигрыш.
            if (winner)
            {
                users_ptr->setWin_game_count(users_ptr->getWin_game_count() + 1);
                client->setWinGameCount(users_ptr->getWin_game_count());
            }

            // Сетаем общее количество игр. И Среднее количество выигранных игр.
            users_ptr->setAll_game_count(users_ptr->getAll_game_count() + 1);
            client->setAllGameCount(users_ptr->getAll_game_count());

            users_ptr->setScore((static_cast<double>(users_ptr->getWin_game_count()) /
                                 static_cast<double>(users_ptr->getAll_game_count())) *
                                100.);
            client->setScore(users_ptr->getScore());

            QVector<QSharedPointer<Users>> usersList;
            usersList.append(users_ptr);
            newHistory->setlist_of_Users(usersList);

            oldHistoryList.append(newHistory);
            users_ptr->setlist_of_History(oldHistoryList);

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

// Удаляет клиента из базы данных со всеми зависимостями.
bool DataBase::deleteClient(ClientNetwork *const client)
{
    if (client)
    {
        if (readUserFromBaseWithRelations(client->getName()))
        {
            QSqlError err = qx::dao::delete_by_id(users_ptr);
            for (QSharedPointer<History> history : users_ptr->getlist_of_History())
            {
                qx::dao::delete_by_id(history);
            }

            switch (err.type())
            {
            case QSqlError::NoError:
                break;
            case QSqlError::ConnectionError:
            case QSqlError::StatementError:
            case QSqlError::TransactionError:
            case QSqlError::UnknownError:
                qInfo() << "Server: @deleteClient --->" << err.text();
                return false;
            }

            return true;
        }
    }
    return false;
}

// Удаляет из друзей указанного клиента.
bool DataBase::deleteFriend(QString login, ClientNetwork *const sender)
{
    if (readUserFromBase(sender->getName()) && sender->getClientFriendLogins().keys().contains(login))
    {
        qx_query query;
        query.where("t_friends.login").isEqualTo(login);
        QSqlError err = qx::dao::delete_by_query<Friends>(query);
        switch (err.type())
        {
        case QSqlError::NoError:
            break;
        case QSqlError::ConnectionError:
        case QSqlError::StatementError:
        case QSqlError::TransactionError:
        case QSqlError::UnknownError:
            qInfo() << "Server: @deleteClient --->" << err.text();
            return false;
        }
        return true;
    }
    return false;
}

bool DataBase::updatePassword(QString newPassword)
{
    if (users_ptr)
    {
        users_ptr->setPassword(newPassword);
        QSqlError err = qx::dao::update(users_ptr);
        switch (err.type())
        {
        case QSqlError::NoError:
            return true;
        case QSqlError::ConnectionError:
        case QSqlError::StatementError:
        case QSqlError::TransactionError:
        case QSqlError::UnknownError:
            qWarning() << "Server:@updatePassword ---> ERROR:" << err.text();
            return false;
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
const QJsonArray DataBase::getHistory(ClientNetwork *const user)
{
        if (!readUserFromBaseWithRelations(user->getName()))
        {
            return QJsonArray();
        }
    QJsonArray history_list;
    QJsonObject history_obj;
    for (const QSharedPointer<History> &history : users_ptr->getlist_of_History())
    {

            history_obj["game_start"] = history->getGame_start().toUTC().toString("yyyy-MM-ddThh:mm:ssZ");
            history_obj["game_end"] = history->getGame_end().toUTC().toString("yyyy-MM-ddThh:mm:ssZ");
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
    return history_list;
}

// Возвращает список друзей для указанного клиента.
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

const unsigned long DataBase::getAllGameCount() const
{
    return users_ptr->getAll_game_count();
}

const unsigned long DataBase::getWinGameCount() const
{
    return users_ptr->getWin_game_count();
}

const QString DataBase::getUserQuestion(const QStringList &questions) const
{
    return questions.at(users_ptr->getQuestion_type());
}

DataBase::~DataBase()
{

    qx::QxSqlDatabase::getSingleton()->closeAllDatabases();
    qInfo() << "Server: DataBase ---> deleted";
}
