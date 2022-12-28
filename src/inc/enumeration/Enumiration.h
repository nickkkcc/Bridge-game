#ifndef ENUMIRATION_H
#define ENUMIRATION_H

enum BidCall
{

    PASS,
    BID,
    DOUBLE_BID,
    REDOUBLE_BID
};

enum CardRank
{

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

enum CardSuit
{

    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES,
    NONE
};

enum GameEvent
{

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
    RUBBER_COMPLETED, // Один роббер завершен
    PLAY_STOP, // Остановка игры моменты, когда один и до 3 игроков неожиданно отключились
    PLAY_CONTINUES // Продолжение игры после остановки.
};

enum GamePhase
{

    BIDDING,
    CARDPLAY
};

enum PlayerPosition
{

    NORTH,
    EAST,
    SOUTH,
    WEST,
    NONE_POSITION
};

enum Team
{

    N_S,
    E_W,
    NONE_TEAM
};

#endif // ENUMIRATION_H
