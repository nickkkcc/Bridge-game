#include "inc/mainclient.h"

MainClient::MainClient(QObject *parent) : QObject{parent}
{    
    m_socket = new Socket(QUrl("ws://31.134.129.26:8888"), this);
    connect(m_socket, &Socket::sendConnected, this, &MainClient::sendConnected);
    connect(m_socket, &Socket::sendDisconnected, this, &::MainClient::sendDisconnected);
    connect(m_socket, &Socket::pong, this, &MainClient::sendPong);
    connect(m_socket, &Socket::sendJson, this, &MainClient::parseJson);
    connect(this, &MainClient::sendJson, m_socket, &Socket::txJson);
    statusTimer.setInterval(3000);
    statusTimer.setSingleShot(true);
    connect(&statusTimer, &QTimer::timeout, this, &MainClient::statusReset);
    setTeamSelect(QStringList{"Север-Юг", "Запад-Восток"});
}

void MainClient::sendRegisterQuestions()
{
    QJsonObject tx = getTx(net::AnswerType::QUESTIONS_ANSWER);
    tx["data"] = QJsonObject();
    emit sendJson(tx);
}

void MainClient::sendRegister(QString login, QString password, int index, QString answer)
{
    QJsonObject tx = getTx(net::AnswerType::REGISTER_ANSWER);
    QJsonObject data;
    data["login"] = login;
    data["password"] = password;
    data["question"] = index;
    data["answer"] = answer;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::sendLogin(QString login, QString password)
{
    QJsonObject tx = getTx(net::AnswerType::LOGIN_ANSWER);
    QJsonObject data;
    data["login"] = login;
    data["password"] = password;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::closeFromServer()
{
    m_socket->onCloseConnection();
}

void MainClient::connectToServer()
{
    m_socket->onConnectToServer();
}

void MainClient::createLobby(int team)
{
    m_team = team;
    QJsonObject tx = getTx(net::AnswerType::CREATE_LOBBY_ANSWER);
    tx["data"] = QJsonObject{};
    emit sendJson(tx);
}

void MainClient::selectTeamAdmin(int team)
{
    QJsonObject tx = getTx(net::AnswerType::SELECT_TEAM_ADMIN);
    QJsonObject data{};
    data["lobby_id"] = m_lobbyUud;
    data["team"] = team;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::exitLobby()
{
    setLobbyCreated(false);
    QJsonObject tx = getTx(net::AnswerType::EXIT_LOBBY);
    QJsonObject data{};
    data["lobby_id"] = m_lobbyUud;
    data["successful"] = true;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::exitInviterLobby()
{
    QJsonObject tx = getTx(net::AnswerType::EXIT_LOBBY);
    QJsonObject data{};
    data["lobby_id"] = m_findLobbyManager->inviterLobbyId();
    data["successful"] = true;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::addFriend(int index)
{
    QJsonObject tx = getTx(net::AnswerType::ADD_FRIEND);
    QJsonObject data{};
    data["login"] = m_playersList->removeLoginPlayer(index);
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::deleteFriend(int index)
{
    QJsonObject tx = getTx(net::AnswerType::DELETE_FRIEND);
    QJsonObject data{};
    data["login"] = m_playersList->removeLoginFriend(index);
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::clearPlayersList()
{
    m_playersList->clearAll();
}

void MainClient::invitePlayer(int index)
{
    QJsonObject tx = getTx(net::AnswerType::INVITE_PLAYER);
    QJsonObject data{};
    data["lobby_id"] = m_lobbyUud;
    data["login"] = m_playersList->getLoginPlayer(index);
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::inviteFriend(int index)
{
    QJsonObject tx = getTx(net::AnswerType::INVITE_PLAYER);
    QJsonObject data{};
    data["lobby_id"] = m_lobbyUud;
    data["login"] = m_playersList->getLoginFriend(index);
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::startGame()
{
    QJsonObject tx = getTx(net::AnswerType::START_GAME);
    QJsonObject data{};
    data["lobby_id"] = m_lobbyUud;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::findGame()
{
    m_findLobbyManager->updateStates(FindPopupState::SET_JOIN, "", "", 0, 0);
    emit findLobbyManagerChanged();
    QJsonObject tx = getTx(net::AnswerType::FIND_GAME);
    QJsonObject data{};
    data["join"] = true;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::stopFindGame()
{
    m_findLobbyManager->updateStates(FindPopupState::UNSET_JOIN, "", "", 0, 0);
    emit findLobbyManagerChanged();
    QJsonObject tx = getTx(net::AnswerType::FIND_GAME);
    QJsonObject data{};
    data["join"] = false;
    tx["data"] = data;

    emit sendJson(tx);
}

void MainClient::acceptedInvite(bool anwer)
{
    if (anwer)
    {
        m_findLobbyManager->updateStates(FindPopupState::TX_ACCEPT_INVITE);
    }
    else
    {
        m_findLobbyManager->setReject(true);
    }
    emit findLobbyManagerChanged();
    QJsonObject tx = getTx(net::AnswerType::ACCEPT_INVITE);
    QJsonObject data{};
    data["lobby_id"] = m_findLobbyManager->inviterLobbyId();
    data["successful"] = anwer;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::selectTeam(int index)
{
    m_findLobbyManager->updateStates(FindPopupState::TX_SELECT_TEAM);
    emit findLobbyManagerChanged();
    QJsonObject tx = getTx(net::AnswerType::SELECT_TEAM);
    QJsonObject data{};
    data["lobby_id"] = m_findLobbyManager->inviterLobbyId();
    data["team"] = index;
    tx["data"] = data;
    emit sendJson(tx);
}

void MainClient::loadImages()
{
    cardPreView << "qrc:/resources/cards/8H.svg"
                << "qrc:/resources/cards/QC.svg"
                << "qrc:/resources/cards/AS.svg"
                << "qrc:/resources/cards/10D.svg"
                << "qrc:/resources/cards/6C.svg";
    emit cardPreViewChanged();
}

void MainClient::parseJson(const QJsonObject &rxObj)
{

    if (rxObj["type"] == "registration_questions")
    {

        QStringList temp;
        QJsonArray rx = rxObj["data"].toObject()["questions"].toArray();
        for (const auto &t : qAsConst(rx))
        {

            temp.append(t["question"].toString());
        }
        setQuestions(temp);
    }
    if (rxObj["type"] == "registration")
    {

        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setStatus(true);
            setError("Вы успешно зарегистрировались");
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
        return;
    }
    if (rxObj["type"] == "login")
    {

        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setUuid(rxObj["data"].toObject()["token"].toString());
            setStatus(true);
            setError("Вы успешно вошли! Ваш временный идентификатор:" + m_uuid);
            emit sendLoginAnswer();
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
        return;
    }
    if (rxObj["type"] == "create_lobby")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setLobbyUud(rxObj["data"].toObject()["lobby_id"].toString());
            selectTeamAdmin(m_team);
            setLobbyCreated(true);
            setStatus(true);
            setError("Вы успешно создали лобби с идентификатором " + m_lobbyUud);
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
    }
    if (rxObj["type"] == "players_online")
    {
        m_playersList->parsePlayersList(rxObj["data"].toObject()["friends"].toArray(),
                                        rxObj["data"].toObject()["players"].toArray());
    }
    if (rxObj["type"] == "exit_lobby")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            if (m_findLobbyManager->isOpen())
            {
                m_findLobbyManager->updateStates(FindPopupState::SET_JOIN);
                m_findLobbyManager->setIsOpen(false);
                if (!m_findLobbyManager->getSelfExit())
                {
                    findGame();
                }
                emit findLobbyManagerChanged();
            }
            setStatus(true);
            setError("Лобби " + rxObj["lobby_id"].toString() + "успешно закрыто!");
            m_lobbyUud = QString{};
            setLobbyCreated(false);
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
    }
    if (rxObj["type"] == "add_friend")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setStatus(true);
            setError("Выбранный игрок успешно добавлен в друзья!");
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
    }
    if (rxObj["type"] == "delete_friend")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setStatus(true);
            setError("Выбранный игрок успешно удален из друзей!");
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
    }
    if (rxObj["type"] == "players_count_lobby")
    {
        int count{rxObj["data"].toObject()["count"].toInt()};
        if (count > m_players_lobby_count)
        {
            setStatus(true);
            setError("В лобби зашел новый участник! Игроков в лобби: " + QString::number(count));
            setPlayers_lobby_count(count);
        }
        else if (count < m_players_lobby_count)
        {
            setStatus(false);
            setError("Один из игроков вышел из лобби! Игроков в лобби: " + QString::number(count));
            setPlayers_lobby_count(count);
        }
        m_findLobbyManager->setLobbyPlayerCont(count);
        emit findLobbyManagerChanged();
    }
    if (rxObj["type"] == "start_game_players")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setStatus(true);
            setError("Игра успешно начата!!!");
            emit gameStarted();
        }
        else
        {
            setStatus(false);
            setError(rxObj["data"].toObject()["error"].toString());
        }
    }
    if (rxObj["type"] == "invite_players")
    {
        QString inviter = rxObj["data"].toObject()["alias"].toString().mid(0, 18);
        QString lobbyId = rxObj["data"].toObject()["lobby_id"].toString();

        setError("Игрок: " + inviter + " приглашает вас в свое лобби: " + lobbyId);
        setStatus(true);
        m_findLobbyManager->updateStates(FindPopupState::RX_INVITING, inviter, lobbyId, 0, 0);
        emit findLobbyManagerChanged();
    }

    if (rxObj["type"] == "accept_invite_players")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            setError("Ваш ответ зарегистрирован сервером!!");
            setStatus(true);
            m_findLobbyManager->setIsOpen(true);
        }
        else
        {
            setError(rxObj["data"].toObject()["error"].toString());
            setStatus(true);
        }
        m_findLobbyManager->updateStates(FindPopupState::RX_ACCEPT_INVITE);
        emit findLobbyManagerChanged();

        if (m_findLobbyManager->reject())
        {
            m_findLobbyManager->updateStates(FindPopupState::SET_JOIN);
            emit findLobbyManagerChanged();
            m_findLobbyManager->setReject(false);
        }
    }

    if (rxObj["type"] == "select_team")
    {
        m_findLobbyManager->updateStates(FindPopupState::RX_SELECT_TEAM, "", "",
                                         rxObj["data"].toObject()["team_1"].toInt(),
                                         rxObj["data"].toObject()["team_2"].toInt());
        emit findLobbyManagerChanged();
    }
    if (rxObj["type"] == "accept_select_team")
    {
        if (rxObj["data"].toObject()["error"].toString() == "")
        {
            if (!lobbyCreated)
            {
                setStatus(true);
                setError("Вас успешно добавили в команду!");
                m_findLobbyManager->updateStates(FindPopupState::RX_IN_LOBBY, "", "", 0, 0);
                emit findLobbyManagerChanged();
            }
        }
        else
        {
            setStatus(false);
            setLobbyCreated(false);
            setError(rxObj["data"].toObject()["error"].toString());
            m_findLobbyManager->updateStates(FindPopupState::RX_RESELECT_TEAM, "", "", 0, 0);
            emit findLobbyManagerChanged();
        }
    }
}
void MainClient::sendPong(quint64 time)
{
    setPing(time);
}

QJsonObject MainClient::getTx(net::AnswerType type)
{
    switch (type)
    {

    case net::AnswerType::QUESTIONS_ANSWER: {
        QJsonObject tx;
        tx["id"] = "0";
        tx["type"] = "registration_questions";
        return tx;
    }
    case net::AnswerType::REGISTER_ANSWER: {
        QJsonObject tx;
        tx["id"] = "0";
        tx["type"] = "registration";
        return tx;
    }
    case net::AnswerType::LOGIN_ANSWER: {
        QJsonObject tx;
        tx["id"] = "0";
        tx["type"] = "login";
        return tx;
    }
    case net::AnswerType::CREATE_LOBBY_ANSWER: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "create_lobby";
        return tx;
    }
    case net::AnswerType::SELECT_TEAM_ADMIN: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "select_team_admin";
        return tx;
    }
    case net::AnswerType::EXIT_LOBBY: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "exit_lobby";
        return tx;
    }
    case net::AnswerType::ADD_FRIEND: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "add_friend";
        return tx;
    }
    case net::AnswerType::DELETE_FRIEND: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "delete_friend";
        return tx;
    }
    case net::AnswerType::INVITE_PLAYER: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "invite_players";
        return tx;
    }
    case net::AnswerType::START_GAME: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "start_game_players";
        return tx;
    }
    case net::AnswerType::FIND_GAME: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "join";
        return tx;
    }
    case net::AnswerType::ACCEPT_INVITE: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "accept_invite_players";
        return tx;
    }
    case net::AnswerType::SELECT_TEAM: {
        QJsonObject tx;
        tx["id"] = m_uuid;
        tx["type"] = "select_team";
        return tx;
    }
    }
}

void MainClient::statusReset()
{

    setStatus(true);
    setError("");
    setStatusVisible(false);
}

QStringList MainClient::getCardPreView() const
{
    return cardPreView;
}

void MainClient::setCardPreView(const QStringList &newCardPreView)
{
    if (cardPreView == newCardPreView)
        return;
    cardPreView = newCardPreView;
    emit cardPreViewChanged();
}

FindLobbyManager *MainClient::findLobbyManager() const
{
    return m_findLobbyManager;
}

void MainClient::setFindLobbyManager(FindLobbyManager *newFindLobbyManager)
{
    if (m_findLobbyManager == newFindLobbyManager)
        return;
    m_findLobbyManager = newFindLobbyManager;
    emit findLobbyManagerChanged();
}

int MainClient::players_lobby_count() const
{
    return m_players_lobby_count;
}

void MainClient::setPlayers_lobby_count(int newPlayers_lobby_count)
{
    if (m_players_lobby_count == newPlayers_lobby_count)
        return;
    m_players_lobby_count = newPlayers_lobby_count;
    emit players_lobby_countChanged();
}

QString MainClient::uuid() const
{
    return m_uuid;
}

void MainClient::setUuid(const QString &newUuid)
{
    if (m_uuid == newUuid)
        return;
    m_uuid = newUuid;
    emit uuidChanged();
}

QString MainClient::lobbyUud() const
{
    return m_lobbyUud;
}

void MainClient::setLobbyUud(const QString &newLobbyUud)
{
    if (m_lobbyUud == newLobbyUud)
        return;
    m_lobbyUud = newLobbyUud;
    emit lobbyUudChanged();
}

QStringList MainClient::TeamSelect() const
{
    return m_TeamSelect;
}

void MainClient::setTeamSelect(const QStringList &newTeamSelect)
{
    if (m_TeamSelect == newTeamSelect)
        return;
    m_TeamSelect = newTeamSelect;
    emit TeamSelectChanged();
}

bool MainClient::getLobbyCreated() const
{
    return lobbyCreated;
}

void MainClient::setLobbyCreated(bool newLobbyCreated)
{
    if (lobbyCreated == newLobbyCreated)
        return;
    lobbyCreated = newLobbyCreated;
    emit lobbyCreatedChanged();
}

PlayersList *MainClient::playersList() const
{
    return m_playersList;
}

void MainClient::setPlayersList(PlayersList *newPlayersList)
{
    if (m_playersList == newPlayersList)
        return;
    m_playersList = newPlayersList;
    emit playersListChanged();
}

quint64 MainClient::ping() const
{
    return m_ping;
}

void MainClient::setPing(quint64 newPing)
{
    if (m_ping == newPing)
        return;
    m_ping = newPing;
    emit pingChanged();
}

bool MainClient::questionsIsEmpty()
{
    return questions().isEmpty();
}

bool MainClient::getStatusVisible() const
{
    return statusVisible;
}

void MainClient::setStatusVisible(bool newStatusVisible)
{
    if (statusVisible == newStatusVisible)
        return;
    statusVisible = newStatusVisible;
    emit statusVisibleChanged();
}

bool MainClient::status() const
{
    return m_status;
}

void MainClient::setStatus(bool newStatus)
{
    if (m_status == newStatus)
        return;
    m_status = newStatus;
    emit statusChanged();
    setStatusVisible(true);
}

QString MainClient::error() const
{
    return m_error;
}

void MainClient::setError(const QString &newError)
{
    if (m_error == newError)
        return;
    m_error = newError;
    emit errorChanged();
    setStatusVisible(true);
    statusTimer.start();
}

QStringList MainClient::questions() const
{
    return m_questions;
}

void MainClient::setQuestions(const QStringList &newQuestions)
{
    if (m_questions == newQuestions)
        return;
    m_questions = newQuestions;
    emit questionsChanged();
}
