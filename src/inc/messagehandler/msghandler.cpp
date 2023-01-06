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
        if (playerIs)
        {

            obj = generateAnswer(MsgType::REGISTRATION, false,
                                 "Клиент с таким логином уже определен. Выберите себе другой логин!");
        }
        else
        {

            base->writeUserToBase(obj["data"].toObject()["login"].toString(), QUuid::createUuid(),
                                  obj["data"].toObject()["password"].toString(),
                                  QuestionsType::enum_QuestionsType(obj["data"].toObject()["question"].toInt()),
                                  obj["data"].toObject()["answer"].toString(), 0, 0, 0);
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

        obj = generateAnswer(MsgType::LOGIN, false, "Игра уже началась.");
        return false;
    }

    if (clients->count() >= *maxPlayers)
    {

        obj = generateAnswer(MsgType::LOGIN, false, "Сервер переполнен.");
        return false;
    }

    if (obj["data"].toObject()["login"].toString().length() > *maxLoginLength ||
        obj["data"].toObject()["login"].toString().length() < *minLoginLength)
    {

        obj = generateAnswer(MsgType::LOGIN, false,
                             "Длина логина должна быть больше, чем " + QString::number(*maxLoginLength) +
                                 " и меньше, чем " + QString::number(*minLoginLength));
        return false;
    }

    if (!base->readUserFromBase(obj["data"].toObject()["login"].toString()))
    {

        obj = generateAnswer(MsgType::LOGIN, false, "Такого логина нет в базе данных. Выберите другой логин!");
        return false;
    }
    else
    {

        if (obj["data"].toObject()["password"] != base->getUserPassword())
        {

            obj = generateAnswer(MsgType::LOGIN, false, "Пароль неверен!");
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

                obj = generateAnswer(MsgType::LOGIN, false, "Такой клиент уже авторизован!");
                return false;
            }
        }
    }
}

bool MsgHandler::tryChangePassword(QJsonObject &obj)
{
    if (obj["type"] == "forgot_password")
    {
        if (base->readUserFromBaseWithRelations(obj["data"].toObject()["login"].toString()))
        {
            obj = generateAnswer(MsgType::FORGOTTEN_PASSWORD, true, "", "0", base->getUserQuestion(questions),
                                 base->getUserLogin());
        }
        else
            obj = generateAnswer(MsgType::FORGOTTEN_PASSWORD, false, "Такого логина нет в базе данных!", "0", "", "");
        {
        }
    }
    if (obj["type"] == "secret_question")
    {
        if (base->readUserFromBaseWithRelations(obj["data"].toObject()["login"].toString()))
        {
            if (base->getUseranswer() == obj["data"].toObject()["answer"].toString())
            {
                obj = generateAnswer(MsgType::SECRET_QUESTION, true, "", "0", "", base->getUserLogin());
            }
            else
            {
                obj = generateAnswer(MsgType::SECRET_QUESTION, false, "Ответ на вопрос неверен!", "0", "",
                                     base->getUserLogin());
            }
        }
        else
        {
            obj = generateAnswer(MsgType::SECRET_QUESTION, false, "Такого логина нет в базе данных!", "0", "", "");
        }
    }
    if (obj["type"] == "change_password")
    {
        if (base->readUserFromBaseWithRelations(obj["data"].toObject()["login"].toString()))
        {
            if (base->updatePassword(obj["data"].toObject()["new_password"].toString()))
            {
                obj = generateAnswer(MsgType::CHANGE_PASSWORD, true, "", "0", "", base->getUserLogin());
            }
            else
            {
                obj = generateAnswer(MsgType::CHANGE_PASSWORD, false, "Не получилось обновить пароль!", "0", "",
                                     base->getUserLogin());
            }
        }
        else
        {
            obj = generateAnswer(MsgType::CHANGE_PASSWORD, false, "Такого логина нет в базе данных!", "0", "",
                                 base->getUserLogin());
        }
    }
    return false;
}

QJsonObject MsgHandler::generateAnswer(const MsgType &type, bool successful, const QString &error, const QString &uid,
                                       QString question, QString login)
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
        for (int i = QuestionsType::enum_QuestionsType::FIRST_PHONE_NUMBER;
             i <= QuestionsType::enum_QuestionsType::MOTHERS_NAME; i++)
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
    case FORGOTTEN_PASSWORD: {
        answer["id"] = "0";
        answer["type"] = "forgot_password";
        data["login"] = login;
        data["successful"] = successful;
        data["error"] = error;
        data["question"] = question;
        answer["data"] = data;
        break;
    }
    case SECRET_QUESTION: {
        answer["id"] = "0";
        answer["type"] = "secret_question";
        data["login"] = login;
        data["successful"] = successful;
        data["error"] = error;
        answer["data"] = data;
        break;
    }
    case CHANGE_PASSWORD: {
        answer["id"] = "0";
        answer["type"] = "change_password";
        data["login"] = login;
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
