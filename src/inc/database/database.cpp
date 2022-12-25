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

bool DataBase::writeUserToBase(QString login, QString password,
                          QString questionType, QString answer, double score)
{

    if (readUserFromBase(login))
    {

        return false;
    }

    userPtr.clear();
    userPtr.reset(new Users());
    userPtr->setlogin(login);
    userPtr->setpassword(password);
    userPtr->setquestion_type(questionType);
    userPtr->setanswer(answer);
    userPtr->setscore(score);

    qx::dao::insert(userPtr);

    return true;
}

bool DataBase::readUserFromBase(QString login)
{

    usersList.clear();
    qx_query query;
    query.where("t_users.login").isEqualTo(login);
    qx::dao::fetch_by_query(query, usersList);

    return !usersList.isEmpty();
}

const QString DataBase::getUserLogin() const
{

    if (!usersList.isEmpty())
    {

        return usersList[0].getlogin();
    }
    return "";
}

const QString DataBase::getuserPassword() const
{

    if (!usersList.isEmpty())
    {

        return usersList[0].getpassword();
    }
    return "";
}

const QString DataBase::getUserQuestionType() const
{

    if (!usersList.isEmpty())
    {

        return usersList[0].getquestion_type();
    }
    return "";
}

const QString DataBase::getUseranswer() const
{

    if (!usersList.isEmpty())
    {

        return usersList[0].getanswer();
    }
    return "";
}

int DataBase::getUserScore()
{

    if (!usersList.isEmpty())
    {

        return usersList[0].getscore();
    }
    return 0;
}

DataBase::~DataBase()
{

    qx::QxSqlDatabase::getSingleton()->closeAllDatabases();
    qInfo() << "Server: DataBase ---> deleted";
}
