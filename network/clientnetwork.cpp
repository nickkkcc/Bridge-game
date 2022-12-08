#include "network/clientnetwork.h"


#include <network/clientnetwork.h>


ClientNetwork::ClientNetwork(QObject *parent) :
    QObject(parent)
{
    url = QUrl(QStringLiteral("ws://31.134.130.194:8888"));
    connect(&m_webSocket, &QWebSocket::connected, this, &ClientNetwork::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &ClientNetwork::closed);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &ClientNetwork::serverResponse);
    m_webSocket.open(QUrl(url));
    qDebug() << "Класс clientnetwork";
}
ClientNetwork::~ClientNetwork()
{
    m_webSocket.close();
}

void ClientNetwork::onConnected() //когда есть коннет, отправляем на сервер логин и пароль
{

    qInfo() << "Коннект";


}
//перенести функции парсера json в отдельный класс
void ClientNetwork::sendLoginAndPassword(QString login, QString password)// отправка логина и пароля
{
    QJsonObject sendLoginAndPassword;
    QJsonObject data;
    sendLoginAndPassword.insert("id", 0);
    sendLoginAndPassword.insert("type", "login");

    data.insert("login", m_login);
    data.insert("password", m_password);

    sendLoginAndPassword.insert("data", data);
    sendData(sendLoginAndPassword);
}
void ClientNetwork::serverResponse(const QString &message){ //функция получение ответов от сервера и их обработки
    QJsonObject dataServer = QJsonDocument::fromJson(message.toUtf8()).object();
    id = QUuid(dataServer["id"].toString());
    if(dataServer["type"].toString() == "login"){
        token = dataServer["token"].toInt();
        successful = dataServer["successful"].toBool();
        error = dataServer["error"].toString();

    }
    if(dataServer["type"].toString() == "registration_questions"){
        list = dataServer["data"].toArray().toVariantList();       //посмотри правильность
        listq = list.toStringList(); // надо подумать как убрать это шнягу
    }
    if(dataServer["type"].toString()  == "registration"){
        successful = dataServer["successful"].toBool();
        error = dataServer["error"].toString();
    }
    else{
        abort();
    };
}
void ClientNetwork::sendRegistr(QString login, QString password, qint64 question, QString answer ){ //функцияя отправки данных регистрации
    QJsonObject sendRegistr;
    QJsonObject data;
    sendRegistr.insert("id", "0");
    sendRegistr.insert("type", "registration");

    data.insert("login", m_login);
    data.insert("password", m_password);
    data.insert("question", 0);
    data.insert("answer", m_answer);

    sendRegistr.insert("data", data);


    sendData(sendRegistr);
}
void ClientNetwork::sendQuestion(){ //запрашиваем список вопросов
    QJsonObject sendQuestion;
    QJsonObject data;
    sendQuestion.insert("id", "0");
    sendQuestion.insert("type", "registration_questions");
    sendQuestion.insert("data", data);
    sendData(sendQuestion);
}
void ClientNetwork::sendData(QJsonObject dataJson){ //упаковка всех данных
    QByteArray dataSend = QJsonDocument::fromVariant(dataJson).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(dataSend);
}
void ClientNetwork::abort(){
    m_webSocket.close();
    qDebug()<<"Ошибка";
}
