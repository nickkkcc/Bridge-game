#include "inc/findlobbymanager.h"

FindLobbyManager::FindLobbyManager(QObject *parent)
    : QObject{parent}
{
}

void FindLobbyManager::updateStates(FindPopupState state, QString inviterId, QString inviterLobbyId, int nsCount,
                                    int ewCount)
{
    this->state = state;
    m_stateInt = state;
    if (inviterId != "")
    {
        m_inviterId = inviterId;
    }
    if (inviterLobbyId != "")
    {
        m_inviterLobbyId = inviterLobbyId;
    }
    if (nsCount != 0)
    {
        m_inviter_NS_count = nsCount;
    }
    if (ewCount != 0)
    {
        m_inviter_EW_count = ewCount;
    }
}

QString FindLobbyManager::headerText()
{
    switch (state)
    {

    case SET_JOIN: {
        return "Поиск лобби ...";
    }
    case UNSET_JOIN: {
        return "";
    }

    case RX_INVITING: {
        return "Приглашение в лобби: " + m_inviterLobbyId;
    }

    case TX_ACCEPT_INVITE: {
        return "Ожидание регистрации ответа...";
    }
    case RX_ACCEPT_INVITE: {
        return "Ваш ответ принят!";
    }
    case RX_SELECT_TEAM: {
        return "Выбор команды в лобби";
    }
    case RX_RESELECT_TEAM:
    case TX_SELECT_TEAM: {
        return "Отправка выбора команды";
    }
    case RX_IN_LOBBY: {
        return "Лобби " + m_inviterLobbyId;
    }
    }
}

QString FindLobbyManager::selectTeamText()
{
    switch (state)
    {
    case SET_JOIN:
        return "Ожидание приглашения!";
    case RX_INVITING:
        return "Примем приглашение?";
    case RX_RESELECT_TEAM:
    case RX_SELECT_TEAM: {
        return "Выберите команду!";
    }

    default: {
        if (m_selectedTeam == 0)
        {
            return "Ваша команда: Север-Юг";
        }
        if (m_selectedTeam == 1)
        {
            return "Ваша команда: Запад-Восток";
        }
    }
    }
    return "";
}

QString FindLobbyManager::statusText()
{
    switch (state)
    {

    case SET_JOIN: {
        return "Статус: Поиск лобби ...";
    }
    case UNSET_JOIN: {
        return "";
    }

    case RX_INVITING: {
        return "Статус: Принятие приглашения";
    }

    case TX_ACCEPT_INVITE: {
        return "Статус: Ответ отправлен!";
    }
    case RX_ACCEPT_INVITE: {
        return "Статус: Получен ответ от сервера";
    }
    case RX_SELECT_TEAM:
    case TX_SELECT_TEAM: {
        return "Статус: Выбор команды!";
    }

    case RX_RESELECT_TEAM:
        return "Статус: Нет места в команде! Выберите другую";
    case RX_IN_LOBBY:
        return "Статус: Вы в лобби!";
    }
}

QString FindLobbyManager::counPlayerText()
{
    if (state == FindPopupState::RX_IN_LOBBY)
    {
        return "Кол-во игроков в лобби: " + QString::number(m_lobbyPlayerCont);
    }
    return "Кол-во игроков в лобби: неизвестно";
}

bool FindLobbyManager::listEnabled()
{
    switch (state)
    {

    case SET_JOIN:
    case UNSET_JOIN:
    case RX_INVITING:
    case TX_ACCEPT_INVITE:
    case RX_IN_LOBBY:
    case TX_SELECT_TEAM:
        return false;
    case RX_SELECT_TEAM:
    case RX_ACCEPT_INVITE:

    case RX_RESELECT_TEAM:
        return true;
    }
}

bool FindLobbyManager::acceptBtnEnabled()
{
    switch (state)
    {

    case SET_JOIN:
    case UNSET_JOIN:

    case RX_SELECT_TEAM:
    case TX_SELECT_TEAM:
    case RX_RESELECT_TEAM:
    case RX_IN_LOBBY:
        return false;
    case RX_INVITING:
    case TX_ACCEPT_INVITE:
    case RX_ACCEPT_INVITE:
        return true;
    }
}

QString FindLobbyManager::inviterLobbyId() const
{
    return m_inviterLobbyId;
}

void FindLobbyManager::setInviter_NS_count(int newInviter_NS_count)
{
    m_inviter_NS_count = newInviter_NS_count;
}

int FindLobbyManager::selectedTeam() const
{
    return m_selectedTeam;
}

void FindLobbyManager::setSelectedTeam(int newSelectedTeam)
{
    if (m_selectedTeam == newSelectedTeam)
        return;
    m_selectedTeam = newSelectedTeam;
    emit selectedTeamChanged();
}

int FindLobbyManager::lobbyPlayerCont() const
{
    return m_lobbyPlayerCont;
}

void FindLobbyManager::setLobbyPlayerCont(int newLobbyPlayerCont)
{
    if (m_lobbyPlayerCont == newLobbyPlayerCont)
        return;
    m_lobbyPlayerCont = newLobbyPlayerCont;
    emit lobbyPlayerContChanged();
}

bool FindLobbyManager::reject() const
{
    return m_reject;
}

void FindLobbyManager::setReject(bool newReject)
{
    if (m_reject == newReject)
        return;
    m_reject = newReject;
    emit rejectChanged();
}

bool FindLobbyManager::isOpen() const
{
    return m_isOpen;
}

void FindLobbyManager::setIsOpen(bool newIsOpen)
{
    if (m_isOpen == newIsOpen)
        return;
    m_isOpen = newIsOpen;
    emit isOpenChanged();
}

int FindLobbyManager::stateInt() const
{
    return m_stateInt;
}

void FindLobbyManager::setStateInt(int newStateInt)
{
    if (m_stateInt == newStateInt)
        return;
    m_stateInt = newStateInt;
    emit stateIntChanged();
}

bool FindLobbyManager::getSelfExit() const
{
    return selfExit;
}

void FindLobbyManager::setSelfExit(bool newSelfExit)
{
    if (selfExit == newSelfExit)
        return;
    selfExit = newSelfExit;
    emit selfExitChanged();
}
