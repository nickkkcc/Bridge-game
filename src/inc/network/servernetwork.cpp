#include "servernetwork.h"

ServerNetwork::ServerNetwork(QObject *parent) : QObject(parent)
{
    bAllowNewClientConnection = true;
}

ServerNetwork::~ServerNetwork()
{
    for (int var = 0; var < clientSocTemp.count(); ++var)
    {
        QWebSocket *client = clientSocTemp.takeAt(var);
        if (client)
        {
            client->abort();
            delete client;
        }
    }
    server->close();
    server->deleteLater();

    delete lobbyManager;;

    delete dataBase;;

    delete msgHandler;;

    qInfo() << "Server: ServerNetwork ---> deleted.";
}

bool ServerNetwork::initServer(quint16 port, int maxPlayers, int maxLoginLength, int minLoginLength)
{
    // Подключение базы данных.
    this->dataBase = new DataBase("BridgeDatabase", "localhost", 4500, "rise", "7499", "QPSQL", this);

    if (!dataBase)
    {
        emit connectionResult(4, port, "");
        return false;
    }
    this->port = port;

    // Подключение обработчика сообщений.
    msgHandler = new MsgHandler(dataBase, &this->bAllowNewClientConnection, &this->maxPlayers, &this->maxLoginLength,
                                &this->minLoginLength, &this->clients, this);

    // Подключение лобби - менеджера.
    lobbyManager = new LobbyManager(this, 2, &this->clients, dataBase);

    connect(this, &ServerNetwork::startTimer, lobbyManager, &LobbyManager::startTimer);
    connect(lobbyManager, &LobbyManager::disconnectClient, this, &ServerNetwork::disconnectClient);

    if (server)
    {
        emit connectionResult(3, port, "");
        return false;
    }

    bool secureMode = false;

    if (!secureMode)
    {
        server = new QWebSocketServer("Bridge Server", QWebSocketServer::NonSecureMode, nullptr);
    }
    else
    {
        // Secure mode
    }

    if (!server->listen(QHostAddress::AnyIPv4, port)) {
        emit connectionResult(2, port, server->errorString());
        delete server;
        return false;
    }

    connect(server, &QWebSocketServer::newConnection, this, &ServerNetwork::connectClient);
    qInfo() << "Server: initialization --->"
            << server->serverAddress().toString() + ":" + QString::number(server->serverPort());
    emit connectionResult(0, port, "");
    return true;
}

void ServerNetwork::stopListening()
{
    bAllowNewClientConnection = false;

    while (!clients.isEmpty())
    {
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
    QWebSocket *const clientConnection = server->nextPendingConnection();

    if (!clientConnection) {
        emit generalError("Плохое соединение. Игнорируем.");
        return;
    }

    connect(clientConnection, &QWebSocket::textMessageReceived, this, &ServerNetwork::validateClientText);
    connect(clientConnection, &QWebSocket::disconnected, this, &ServerNetwork::disconnectTempClient);

    clientSocTemp.append(clientConnection);
    qInfo() << "Server: socket connected --->"
            << clientConnection->peerAddress().toString() + ":" + QString::number(clientConnection->peerPort());
}

void ServerNetwork::validateClientText(const QString &message)
{

    QWebSocket *const tempSocket = qobject_cast<QWebSocket *>(sender());

    int numRemoved = clientSocTemp.removeAll(tempSocket);

    if (numRemoved > 1)
    {

        emit generalError("Server: finded dublicate ---> deleted.");
        tempSocket->abort();
        return;
    }

    QJsonObject rxTxObj = QJsonDocument::fromJson(message.toUtf8()).object();
    qInfo() << rxTxObj;

    if (rxTxObj.contains("type") && rxTxObj["type"].isString() && rxTxObj.contains("id") && rxTxObj["id"].isString())
    {

        if (rxTxObj["type"] == "registration" || rxTxObj["type"] == "registration_questions")
        {

            qWarning() << "Server: client --->"
                       << tempSocket->peerAddress().toString() + ":" + QString::number(tempSocket->peerPort())
                       << "try register or request questions.";
            if (!msgHandler->checkTryReg(rxTxObj))
            {

                tempSocket->abort();
            }
        }
        else
        {

            if (!msgHandler->tryLogin(rxTxObj))
            {

                qWarning() << "Server: client --->"
                           << tempSocket->peerAddress().toString() + ":" + QString::number(tempSocket->peerPort())
                           << "couldn't login";
            }
            else
            {

                qWarning() << "Server: client --->"
                           << tempSocket->peerAddress().toString() + ":" + QString::number(tempSocket->peerPort())
                           << "joined to server as --->" << dataBase->getUserLogin();

                Lobby *findedLobby = lobbyManager->findLobbyFromTempClient(dataBase->getUserLogin());
                if (findedLobby)
                {

                    QJsonDocument doc(rxTxObj);
                    int tempVal = tempSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));

                    qInfo() << "Server: send to client --->"
                            << tempSocket->peerAddress().toString() + ":" + QString::number(tempSocket->peerPort())
                            << tempVal << "byte(s)";

                    if (tempVal == -1)
                    {

                        emit generalError(
                            "Произошла ошибка при отправке данных клиенту. Предлагается перезапустить игру.");
                    }
                    returnClientToLobby(findedLobby, lobbyManager, tempSocket, dataBase->getUserLogin(),
                                        QUuid(rxTxObj["data"].toObject()["token"].toString()),
                                        dataBase->getUserAlias());
                    return;
                }
                else
                {
                    ClientNetwork *const client = new ClientNetwork(
                        this, dataBase->getUserLogin(), tempSocket,
                        QUuid(rxTxObj["data"].toObject()["token"].toString()), dataBase->getUserAlias());
                    clients.append(client);
                    client->setClientFriendLogins(dataBase->getFriendsList(client));
                    client->setScore(dataBase->getUserScore());
                    client->setAllGameCount(dataBase->getAllGameCount());
                    client->setWinGameCount(dataBase->getWinGameCount());
                    clientSocTemp.removeAll(tempSocket);

                    disconnect(tempSocket, &QWebSocket::textMessageReceived, this, &ServerNetwork::validateClientText);
                    disconnect(tempSocket, &QWebSocket::disconnected, this, &ServerNetwork::disconnectTempClient);
                    connectClientToLobbyManager(client);
                    emit startTimer();
                }
            }
        }

        QJsonDocument doc(rxTxObj);
        int tempVal = tempSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));

        qInfo() << "Server: send to client --->"
                << tempSocket->peerAddress().toString() + ":" + QString::number(tempSocket->peerPort()) << tempVal
                << "byte(s)";

        if (tempVal == -1)
        {

            emit generalError("Произошла ошибка при отправке данных клиенту. Предлагается перезапустить игру.");
        }
    }
}

void ServerNetwork::disconnectTempClient()
{

    QWebSocket *const tempSocket = qobject_cast<QWebSocket *>(sender());

    if (clientSocTemp.contains(tempSocket))
    {

        clientSocTemp.removeAll(tempSocket);
        qInfo() << "Server: socket disconnected --->"
                << tempSocket->peerAddress().toString() + ":" + QString::number(tempSocket->peerPort());
    }
}

void ServerNetwork::disconnectClient(ClientNetwork *const sender)
{

    int num = clients.indexOf(sender);
    if (num != -1)
    {

        ClientNetwork *client = clients.takeAt(num);
        delete client;
    }
}

void ServerNetwork::connectClientToLobbyManager(ClientNetwork *const client)
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
    connect(client, &ClientNetwork::rxAddFriend, this->lobbyManager, &LobbyManager::addFriend);
    connect(client, &ClientNetwork::rxDeleteFriend, this->lobbyManager, &LobbyManager::deleteFriend);
    connect(client, &ClientNetwork::rxRequestHistoryList, this->lobbyManager, &LobbyManager::requestHistoryList);
    connect(client, &ClientNetwork::rxDeleteAccount, this->lobbyManager, &LobbyManager::deleteAccount);
    connect(client, &ClientNetwork::rxRequestScore, this->lobbyManager, &LobbyManager::requestScore);
}

void ServerNetwork::returnClientToLobby(Lobby *const lobby, LobbyManager *const lobbyManager, QWebSocket *clientSoc,
                                        QString clientName, const QUuid &clientUuid, const QUuid &clientAlias)
{
    ClientNetwork *const client = new ClientNetwork(this, clientName, clientSoc, clientUuid, clientAlias);

    clients.append(client);
    clientSocTemp.removeAll(clientSoc);
    disconnect(clientSoc, &QWebSocket::textMessageReceived, this, &ServerNetwork::validateClientText);
    disconnect(clientSoc, &QWebSocket::disconnected, this, &ServerNetwork::disconnectTempClient);
    connectClientToLobbyManager(client);
    client->setScore(dataBase->getUserScore());
    client->setAllGameCount(dataBase->getAllGameCount());
    client->setWinGameCount(dataBase->getWinGameCount());

    lobbyManager->setReturnedClient(lobby, lobby->getDisconnectedPlayers().value(clientName), client);
    qInfo() << "Server: client --->" << client->getUuid().toString() << "--->" << clientName << "returned to lobby --->"
            << lobby->getUuid().toString();

    emit lobbyManager->sendUpdateGameState(PLAY_CONTINUES);
    emit lobbyManager->sendUpdateGameState(lobby->getLastGameEvent());
    emit lobbyManager->sendNextPlayerTurn();
};
