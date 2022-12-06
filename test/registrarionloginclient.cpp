#include "registrarionloginclient.h"
#include <QJsonObject>
#include<QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QtTest/QTest>

QT_USE_NAMESPACE

RegistrationClient::RegistrationClient(const QUrl &url, bool debug, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_debug(debug)
{
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &RegistrationClient::onConnected);
    m_webSocket.open(QUrl(url));
    currentType = RegistrationClient::REGISTRATION_TRUE;
}

void RegistrationClient::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &RegistrationClient::onTextMessageReceived);
    connect(this, &RegistrationClient::send,
            this, &RegistrationClient::sendD);

    emit send((RegistrationClient::Type)currentType);
}


void RegistrationClient::onTextMessageReceived(const QString& message)
{
    qDebug() <<"Answer: " << currentType << " : " << QJsonDocument::fromJson(message.toUtf8())  << '\n';

    checkAnswerFromServer(QJsonDocument::fromJson(message.toUtf8()).object());
    if(currentType > RegistrationClient::LOGIN_FALSE_NOSUCH_PASSWORD){
        emit closed();
    }
    currentType++;
    emit send((RegistrationClient::Type)currentType);
}

void RegistrationClient::sendD(const Type &type)
{
    QJsonObject m = createJsonD(type);
    QJsonDocument n(m);
    m_webSocket.sendTextMessage(n.toJson(QJsonDocument::JsonFormat::Compact));

}

void RegistrationClient::checkAnswerFromServer(const QJsonObject &rx)
{
    QJsonObject answer;
    QJsonObject data;
    switch (currentType) {
    case RegistrationClient::Type::LOGIN_FALSE_NOSUCH_PASSWORD:{
        data["token"] = "0";
        data["successful"] = false;
        data["error"] = "Password is wrong";

        answer["id"] = "0";
        answer["type"] = "login";
        answer["data"] = data;
        break;
    }
    case RegistrationClient::Type::REGISTRATION_TRUE:{
        answer["id"] = "0";
        answer["type"] = "registration";

        data["successful"] = true;
        data["error"] = "";
        answer["data"] = data;
        break;
    }
    case RegistrationClient::Type::REGISTRATION_FALSE:{
        answer["id"] = "0";
        answer["type"] = "registration";

        data["successful"] = false;
        data["error"] = "An account with this username already exists. Choose another login";
        answer["data"] = data;
        break;
    }
    case RegistrationClient::Type::REGISTRATION_QUESTIONS:{
        QJsonArray arr;
        for(QString str:questions){
            arr.push_back(str);
        };
        data["questions"] = arr;

        answer["id"] = "0";
        answer["type"] = "registration_questions";
        answer["data"] = data;
        break;
    }
    case RegistrationClient::Type::LOGIN_TRUE:{
        data["token"] = rx["data"].toObject()["token"];
        data["successful"] = true;
        data["error"] = "";

        answer["id"] = "0";
        answer["type"] = "login";
        answer["data"] = data;
        break;
    }
    case RegistrationClient::Type::LOGIN_FALSE:{
        data["token"] = "0";
        data["successful"] = false;
        data["error"] = "There is no such login in the database. Choose another login";

        answer["id"] = "0";
        answer["type"] = "login";
        answer["data"] = data;
        break;
    }
    }
    qWarning() << "Test " << currentType << ": " << (rx == answer);
}

QJsonObject RegistrationClient::createJsonD(const RegistrationClient::Type& name){

    QJsonObject obj;
    QJsonObject data;

    switch (name) {
    case RegistrationClient::Type::REGISTRATION_TRUE:{
        data["login"] = "Kolya";
        data["password"] = "123456";
        data["question"] = "0";
        data["answer"] = "Norm";

        obj["data"] = data;
        obj["id"] = "0";
        obj["type"] = "registration";


        return obj;
    }
    case RegistrationClient::Type::REGISTRATION_FALSE:{
        data["login"] = "Kolya";
        data["password"] = "123456";
        data["question"] = "0";
        data["answer"] = "Norm";

        obj["data"] = data;
        obj["id"] = "0";
        obj["type"] = "registration";


        return obj;
    }

    case RegistrationClient::Type::REGISTRATION_QUESTIONS:{
        obj["id"] = "0";
        obj["type"] = "registration_questions";
        obj["data"] = data;
        return obj;
    }
    case RegistrationClient::Type::LOGIN_TRUE:{
        data["login"] = "Kolya";
        data["password"] = "123456";

        obj["id"] = "0";
        obj["type"] = "login";
        obj["data"] = data;
        return obj;
    }
    case RegistrationClient::Type::LOGIN_FALSE:{
        data["login"] = "klkll";
        data["password"] = "123456";

        obj["id"] = "0";
        obj["type"] = "login";
        obj["data"] = data;
        return obj;
    }
    case RegistrationClient::Type::LOGIN_FALSE_NOSUCH_PASSWORD:{
        data["login"] = "Kolya";
        data["password"] = "123kljkl;h;hk45";

        obj["id"] = "0";
        obj["type"] = "login";
        obj["data"] = data;
        return obj;
    }
    default:return obj;
    }
}

