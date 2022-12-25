#include "msghandler.h"

bool MsgHandler::checkTryReg(QJsonObject &obj)
{

    if (obj["type"] == "registration_questions")
    {

        obj = generateAnswer(MsgType::QUESTIONS);
        return true;
    }
    else if (obj["type"] == "registration")
    {

        bool playerIs = base->readUserFromBase(obj["data"].toObject()["login"].toString());
        qDebug() << base->getUserLogin();
        if (playerIs)
        {

            obj = generateAnswer(MsgType::REGISTRATION, false,
                                 "An account with this username already exists. Choose another login");
        }
        else
        {

            base->writeUserToBase(obj["data"].toObject()["login"].toString(),
                                  obj["data"].toObject()["password"].toString(),
                                  questions.at(obj["data"].toObject()["question"].toInt()),
                                  obj["data"].toObject()["answer"].toString(), 0);
            obj = generateAnswer(MsgType::REGISTRATION, true, "");
        }
        return true;
    }
    else
    {

        return false;
    }
}

bool MsgHandler::tryLogin(QJsonObject &obj)
{

    if (!bAllowNewClientConnection)
    {

        obj = generateAnswer(MsgType::LOGIN, false, "Game already started.");
        return false;
    }

    if (clients->count() >= *maxPlayers)
    {

        obj = generateAnswer(MsgType::LOGIN, false, "Server is full.");
        return false;
    }

    if (obj["data"].toObject()["login"].toString().length() > *maxLoginLength ||
        obj["data"].toObject()["login"].toString().length() < *minLoginLength)
    {

        obj = generateAnswer(MsgType::LOGIN, false,
                             "Length of login must be greater than " + QString::number(*maxLoginLength) +
                                 " and less than " + QString::number(*minLoginLength));
        return false;
    }

    if (!base->readUserFromBase(obj["data"].toObject()["login"].toString()))
    {

        obj = generateAnswer(MsgType::LOGIN, false, "Такого логина нет в базе данных. Choose another login");
        return false;
    }
    else
    {

        if (obj["data"].toObject()["password"] != base->getuserPassword())
        {

            obj = generateAnswer(MsgType::LOGIN, false, "Password is wrong");
            return false;
        }
        else
        {

            int num = 0;
            for (ClientNetwork *client : *clients)
            {

                if (obj["data"].toObject()["login"].toString() == client->getName())
                {

                    num++;
                }
            }
            if (num == 0)
            {

                QUuid uid = QUuid::createUuid();
                obj = generateAnswer(MsgType::LOGIN, true, "", uid.toString(QUuid::StringFormat::WithoutBraces));
                return true;
            }
            else
            {

                obj = generateAnswer(MsgType::LOGIN, false, "Such a client is already online");
                return false;
            }
        }
    }
}

QJsonObject MsgHandler::generateAnswer(const MsgType &type, bool successful, const QString &error, const QString &uid)
{

    QJsonObject answer;
    QJsonObject data;

    switch (type)
    {

    case MsgType::LOGIN: {

        data["token"] = uid;
        data["successful"] = successful;
        data["error"] = error;

        answer["id"] = "0";
        answer["type"] = "login";
        answer["data"] = data;
        break;
    }

    case MsgType::QUESTIONS: {

        QJsonArray arr;
        QJsonObject question;
        for (int i = 0; i < questions.count(); i++)
        {

            question["question_id"] = i;
            question["question"] = questions.at(i);
            arr.push_back(question);
        };
        data["questions"] = arr;

        answer["id"] = "0";
        answer["type"] = "registration_questions";
        answer["data"] = data;
        break;
    }

    case MsgType::REGISTRATION: {

        answer["id"] = "0";
        answer["type"] = "registration";

        data["successful"] = successful;
        data["error"] = error;
        answer["data"] = data;
        break;
    }
    }
    return answer;
}

MsgHandler::MsgHandler(DataBase *base, bool *bAllowNewClientConnection, int *maxPlayers, int *maxLoginLength,
                       int *minLoginLength, QVector<ClientNetwork *> *clients, QObject *parent = nullptr)
    : QObject(parent)
{

    this->bAllowNewClientConnection = bAllowNewClientConnection;
    this->maxPlayers = maxPlayers;
    this->maxLoginLength = maxLoginLength;
    this->minLoginLength = minLoginLength;
    this->clients = clients;
    this->base = base;
}

MsgHandler::~MsgHandler()
{
    qInfo() << "Server: MsgHandler ---> deleted.";
}
