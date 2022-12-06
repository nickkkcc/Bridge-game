#ifndef ENUMIRATION_H
#define ENUMIRATION_H

enum BidCall {
    PASS = 0,
    BID = 1,
    DOUBLE_BID = 2,
    REDOUBLE_BID = 3
};

enum CardRank {
    TWO = 1,
    THREE = 2,
    FOUR = 3,
    FIVE = 4,
    SIX = 5,
    SEVEN = 6,
    EIGHT = 7,
    NINE = 8,
    TEN = 9,
    JACK = 10,
    QUEEN = 11,
    KING = 12,
    ACE = 13,
};

enum CardSuit {
    CLUBS = 0,
    DIAMONDS = 1,
    HEARTS = 2,
    SPADES = 3,
    NONE = 4
};

enum GameEvent{
    INITIALIZE, // Состояние игры иницилизировано и игра готова к запуску.
    BID_START, // Начало торгов
    BID_RESTART, // Сделано четыре ПАСС. Торги начинаются заново.
    PLAYER_BID, // Игрок завершил свой ход во время торгов
    BID_END, // Фаза торгов закончена
    PLAY_START, // Начало игры.
    TRICK_START, // Начало взятки во время фазы игры
    PLAYER_MOVED, // Игрок сделал свой ход во время фазы игры
    TRICK_END, // Взятка закончена в фазе игры.
    PLAY_END, // Фаза игры закончена
    MATCH_END, // Матч завершен
    RUBBER_COMPLETED // Один роббер завершен
};

enum GamePhase{
    BIDDING,
    CARDPLAY
};

enum PlayerPosition{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

enum Team{
    N_S = 0,
    E_W = 1,
    NONE_TEAM = 2
};

enum LobbyManAns{
    LOBBY_CREATED_TRUE,
    LOBBY_CREATED_FALSE,
    CLOSE_LOBBY_TRUE,
    CLOSE_LOBBY_FALSE
};

#endif // ENUMIRATION_H
