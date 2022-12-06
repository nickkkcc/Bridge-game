#include "servernetwork.h"


ServerNetwork::ServerNetwork(QObject *parent) : QObject(parent)
{
    server = nullptr;
    lobbyManager = nullptr;
    clients.clear();
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
    // Подключение базы данных.
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

    // Подключение обработчика сообщений.
    msgHandler = new MsgHandler(this->base,
                                &this->bAllowNewClientConnection,
                                &this->maxPlayers,
                                &this->maxLoginLength,
                                &this->minLoginLength,
                                &this->clients,
                                this);

    // Подключение лобби - менеджера.
    lobbyManager = new LobbyManager(this,
                                    2,
                                    &this->clients);

    connect(this, &ServerNetwork::startTimer, lobbyManager, &LobbyManager::startTimer);
    connect(lobbyManager, &LobbyManager::disconnectClient, this, &ServerNetwork::disconnectClient);

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

    while (!clients.isEmpty()){
        QString tempPlayerName = clients.takeFirst()->getName();
        clients.takeFirst()->getClientSoc()->abort();
        qInfo() << "Остановка сервера: Удален клиент: " + tempPlayerName;
    }
}
void ServerNetwork::deleteAllClients()
{
    stopListening();
}

void ServerNetwork::connectClient()
{
    QWebSocket* clientConnection = server->nextPendingConnection();

    qInfo() << "Присоединение клиента: Клиент пытается подключиться: " << clientConnection;

    if (!clientConnection) {
        emit generalError("Плохое соединение. Игнорируем.");
        return;
    }

    connect(clientConnection, &QWebSocket::textMessageReceived,this, &ServerNetwork::validateClientText);
    connect(clientConnection, &QWebSocket::disconnected,this, &ServerNetwork::disconnectTempClient);

    clientSocTemp.append(clientConnection);

    qInfo() << "Присоединение клиента: Клиент добавлен.";
//    clientConnection->sendTextMessage("[Server]: You have joined the Bridge server (validation)");
}

void ServerNetwork::validateClientText(const QString &message)
{
    qInfo() << "Проверка клиента: Входящее соединение.";

    QWebSocket* tempSocket = qobject_cast<QWebSocket*>(sender());

    int numRemoved = clientSocTemp.removeAll(tempSocket);

    if (numRemoved > 1) {
        emit generalError("Найдены дубликаты соединения. Удалены.");
        tempSocket->abort();
        return;
    }

    qInfo() << "Проверка клиента: Отправитель успешно преобразован.";

    QJsonObject rxTxObj = QJsonDocument::fromJson(message.toUtf8()).object();
    qInfo() << rxTxObj;

    if (rxTxObj.contains("type") && rxTxObj["type"].isString() && rxTxObj.contains("id") &&
        rxTxObj["id"].isString()){
        qWarning() << "Проверка клиента: пользователь не авторизован.";
        if(rxTxObj["type"] == "registration" || rxTxObj["type"] == "registration_questions"){
            if(!msgHandler->checkTryReg(rxTxObj)){
                tempSocket->abort();
            }
        }else{
            qWarning() << "Проверка клиента: пользователь авторизован. Попытка авторизации";
            if(!msgHandler->tryLogin(rxTxObj)){
                qWarning() << "Проверка клиента: Ошибка авторизации.";
            }else{
                qWarning() << "Проверка клиента: авторизация прошла успешно.";

                clients.append(new ClientNetwork(nullptr,
                                                 base->getUserLogin(),
                                                 tempSocket,
                                                 new QUuid(rxTxObj["data"].toObject()["token"].toString())));
                clientSocTemp.removeAll(tempSocket);
                disconnect(tempSocket, &QWebSocket::textMessageReceived,this, &ServerNetwork::validateClientText);
                disconnect(tempSocket, &QWebSocket::disconnected,this, &ServerNetwork::disconnectTempClient);
//              connect(clients.last(), &ClientNetwork::clientDisconnected,this, &ServerNetwork::disconnectClient);
                connectClientToLobbyManager(clients.last());
                emit startTimer();
            }
        }
        QJsonDocument doc(rxTxObj);
        int tempVal = tempSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));

        qInfo() << "Проверка клиента: Кол-во байт, отправленные клиенту: " << tempVal;
        qInfo() << doc;

        if (tempVal == -1) {
            emit generalError("Произошла ошибка при отправке данных клиенту. Предлагается перезапустить игру.");
        }
    }
}

void ServerNetwork::disconnectTempClient()
{
    QWebSocket* tempSocket = qobject_cast<QWebSocket*>(sender());

    if (clientSocTemp.contains(tempSocket)) {
        clientSocTemp.removeAll(tempSocket);
        qInfo() << "Отключение клиента: Отключение сокета.";
    }

    for(ClientNetwork* client : clients){
        if(client->getClientSoc() == tempSocket){
            QString name = client->getName();
            qInfo() << "Отключение клиента: Отключение сокета. Игрок: " + name;
            clients.removeAll(client);
        }
    }
}

void ServerNetwork::disconnectClient(ClientNetwork* sender)
{
  clients.removeAll(sender);
  delete sender;
  sender = nullptr;
}

void ServerNetwork::connectClientToLobbyManager(ClientNetwork *client)
{
    connect(client, &ClientNetwork::rxCreateLobby, this->lobbyManager, &LobbyManager::createLobby);
    connect(client, &ClientNetwork::rxCloseLobby, this->lobbyManager, &LobbyManager::closeLobby);
    connect(client, &ClientNetwork::rxAcceptSelectTeam, this->lobbyManager, &LobbyManager::acceptSelectTeam);
    connect(client, &ClientNetwork::rxInvitePlayers, this->lobbyManager, &LobbyManager::invitePlayers);
    connect(client, &ClientNetwork::rxStartGamePlayers, this->lobbyManager, &LobbyManager::startGame);
    connect(client, &ClientNetwork::rxSelectTeamAdmin, this->lobbyManager, &LobbyManager::selectTeamAdmin);
    connect(client, &ClientNetwork::clientDisconnected, this->lobbyManager, &LobbyManager::clientDisconnected);
    connect(client, &ClientNetwork::rxJoinLobby, this->lobbyManager, &LobbyManager::joinLobby);
    connect(client, &ClientNetwork::rxAcceptInvitePlayers, this->lobbyManager, &LobbyManager::acceptInvitePlayers);
};



