#pragma once

namespace net
{

enum class ConnectionStatus
{
    CONNECTING,
    CONNECTED,
    DISCONNECTED,
    CONNECTION_ERROR
};

enum class RequestType
{
    QUESTIONS_REQUEST,
    REGISTER_REQUEST,
    LOGIN_REQUEST,
    CREATE_LOBBY_REQUEST
};

enum class AnswerType
{
    QUESTIONS_ANSWER,
    REGISTER_ANSWER,
    LOGIN_ANSWER,
    CREATE_LOBBY_ANSWER,
    SELECT_TEAM_ADMIN,
    EXIT_LOBBY,
    ADD_FRIEND,
    DELETE_FRIEND,
    INVITE_PLAYER,
    START_GAME,
    FIND_GAME,
    ACCEPT_INVITE,
    SELECT_TEAM
};
} // namespace net
