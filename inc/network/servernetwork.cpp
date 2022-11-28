#include "servernetwork.h"


ServerNetwork::ServerNetwork(QObject *parent) : QObject(parent)
{
    server = nullptr;
    playerNames.clear();
    clientSoc.clear();
    clientSocTemp.clear();
    bAllowNewClientConnection = true;
}

ServerNetwork::~ServerNetwork()
{
    qInfo() << "ServerNetwork остановлен";
    if (server != nullptr) {
        disconnect(server, &QWebSocketServer::newConnection, 0, 0);
        server->close();
        server->deleteLater();
    }
}

bool ServerNetwork::initServer(quint16 port, int maxPlayers,
                               int maxLoginLength,
                               int minLoginLength)
{
    base = new DataBase("BridgeDatabase",
                        "localhost",
                        4500,
                        "rise",
                        "7499",
                        "QPSQL",
                        this);
    if(base == nullptr){
        emit connectionResult(4, port, "");
        return false;
    }
    this->port = port;

    msgHandler = new MsgHandler(this->base,
                                &this->bAllowNewClientConnection,
                                &this->maxPlayers,
                                &this->maxLoginLength,
                                &this->minLoginLength,
                                &this->playerNames,
                                &this->uidClients);

    if (server != nullptr){
        emit connectionResult(3, port, "");
        return false;
    }

    bool secureMode = false;

    if(!secureMode){
        server = new QWebSocketServer("Bridge Server", QWebSocketServer::NonSecureMode, this);
    }else{
        server = new QWebSocketServer("Bridge Server", QWebSocketServer::NonSecureMode, this);
    }

    if (!server->listen(QHostAddress::AnyIPv4, port)) {
        emit connectionResult(2, port, server->errorString());
        server->deleteLater();
        server = nullptr;
        return false;
    }

    connect(server, &QWebSocketServer::newConnection, this, &ServerNetwork::connectClient);
    qInfo() << "Инициилизация сервера: Сервер прослушивает на порте: " << server->serverPort();
    emit connectionResult(0, port, "");
    return true;
}

void ServerNetwork::stopListening()
{
    bAllowNewClientConnection = false;

    while (!clientSoc.isEmpty()){
        QWebSocket* tempRemoveSoc = clientSoc.takeFirst();
        QString tempPlayerName = playerNames.takeFirst();

        tempRemoveSoc->abort();

        qInfo() << "Остановка сервера: Удален клиент: " + tempPlayerName;
    }
}

void ServerNetwork::deleteAllClients()
{
    stopListening();
    for (QWebSocket *obj : clientSoc) {
        obj->abort();
    }

    for (QWebSocket *obj : clientSocTemp) {
        obj->abort();
    }

    for (QWebSocket *obj : clientSoc) {
        obj->abort();
    }
    playerNames.clear();
}


void ServerNetwork::connectClient()
{
    QWebSocket* clientConnection = server->nextPendingConnection();

    qInfo() << "Присоединение клиента: Клиент пытается подключиться: " << clientConnection;

    if (!clientConnection) {
        emit generalError("Плохое соединение. Игнорируем.");
        return;
    }

//  connect(clientConnection, &QWebSocket::textMessageReceived,this, &ServerNetwork::validateClientText);
    connect(clientConnection, &QWebSocket::binaryMessageReceived,this, &ServerNetwork::validateClientBinary);
    connect(clientConnection, &QWebSocket::disconnected,this, &ServerNetwork::disconnectClient);
    connect(clientConnection, &QWebSocket::disconnected,clientConnection, &QObject::deleteLater);

    clientSocTemp.append(clientConnection);

    qInfo() << "Присоединение клиента: Клиент добавлен.";
}

void ServerNetwork::validateClientBinary(const QByteArray &message)
{
    qInfo() << "Проверка клиента: Входящее соединение.";

    QObject* obj = sender();
    QWebSocket* tempSocket = qobject_cast<QWebSocket*>(obj);

    int numRemoved = clientSocTemp.removeAll(tempSocket);

//    if (numRemoved == 0){
//        emit generalError("Неожиданное подключение. Удаляем клиента.");
//        tempSocket->abort();
//        return;
//    }
    if (numRemoved > 1) {
        emit generalError("Найдены дубликаты соединения. Удалены.");
        tempSocket->abort();
        return;
    }

    qInfo() << "Проверка клиента: Отправитель успешно преобразован.";

    QJsonObject rxTxObj = QJsonDocument::fromJson(message).object();

    if (rxTxObj.contains("type") && rxTxObj["type"].isString() && rxTxObj.contains("id") &&
        (rxTxObj["id"].isDouble() || rxTxObj["id"].isString())){
        bool valide = false;
            qWarning() << "Проверка клиента: пользователь не авторизован.";
            if(rxTxObj["type"] == "registration" || rxTxObj["type"] == "registration_questions"){
                if(!msgHandler->checkTryReg(rxTxObj)){
                tempSocket->abort();
                }
            }else{
            qWarning() << "Проверка клиента: пользователь авторизован. Попытка авторизации";
            if(!msgHandler->tryLogin(rxTxObj)){
                qWarning() << "Проверка клиента: Ошибка авторизации.";
                valide = false;
            }else{
                valide = true;
            }
        }

    if (valide){
        clientSoc.append(tempSocket);
        playerNames.append(rxTxObj["data"].toObject()["login"].toString());
        emit playerJoined(playerNames.last());
    }
    QJsonDocument doc(rxTxObj);
    int tempVal = tempSocket->sendBinaryMessage(doc.toJson(QJsonDocument::Compact));

    qInfo() << "Проверка клиента: Кол-во байт, отправленные клиенту: " << tempVal;
    qInfo() << doc;

    if (tempVal == -1) {
        emit generalError("Произошла ошибка при отправке данных клиенту. Предлагается перезапустить игру.");
    }
//    if (!validateRes.isEmpty()){
//        disconnect(tempSocket, &QWebSocket::textMessageReceived,this, &ServerNetwork::validateClientText);
//    }
    }
}
void ServerNetwork::disconnectClient()
{
    qInfo() << "Отключение клиента: Сокет отключен (Сервер).";
    qInfo() << "Отключение клиента: Перед отключением:" << clientSocTemp << clientSoc << playerNames;

    QObject* obj = sender();
    QWebSocket* tempSocket = qobject_cast<QWebSocket*>(obj);

    if (clientSocTemp.contains(tempSocket)) {
        clientSocTemp.removeAll(tempSocket);
        qInfo() << "Отключение клиента: Отключение сокета.";
    }

    if (clientSoc.contains(tempSocket)){
        QString tempPlayerName = playerNames.at(clientSoc.indexOf(tempSocket));
        playerNames.removeAll(tempPlayerName);
        clientSoc.removeAll(tempSocket);

        qInfo() << "Отключение клиента: Отключение сокета. Игрок: " + tempPlayerName;
        if (bAllowNewClientConnection) emit playerDisconnected(tempPlayerName);
    }
}

