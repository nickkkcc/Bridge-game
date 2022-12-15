#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QUuid>
#include "inc/enumeration/Enumiration.h"
#include "inc/game/aboutGameState/playergamestate.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    // Функция, вызываемая, когда сервер отправляет запрос клиенту для того, чтобы тот сделал заявку.
    virtual void bidTurn() = 0;

    // Функция, вызываемая, когда сервер отправляет запрос клиенту для того, чтобы тот сделал ход картой.
    virtual void moveTurn() = 0;

    // Функция, вызываемая, когда сервер отпрвляет клиенту отказ, что заявка, отправленная клиентом неправильная.
    virtual void bidRejected(QString reason) = 0;

    // Функция, вызываемая, когда сервер отправляет клиенту отказ, что ход картой сделан неправильно.
    virtual void moveRejected(QString reason) = 0;

    // Функция, вызываемая, когда сервер завершил игру по каким-либо причинам.
    //virtual void gameTerminated(QString reason) = 0;

    // Функция, вызываемая, когда сервер приостановил игру по каким-либо причинам.
    //virtual void gameStopped(QString reason) = 0;

    // Функция, вызываемая, когда сервер отправляет клиенту обновленные игровые данные.
    virtual void updateGameState(PlayerGameState gameState) = 0;

    // Получить имя игрока.
    QString getName() const;

    // Установить имя игрока.
    void setName(QString name);

    // Получить позицию игрока.
    PlayerPosition getPosition() const;

    // Установить позицию игрока.
    void setPosition(PlayerPosition position);

    // Получить команду игрока.
    Team getTeam() const;

    // Установить команду для игрока.
    void setTeam(Team newTeam);

    // Установить идентификатор игрока.
    void setUuid(const QUuid &newUuid);

    // Получить идентификатор для игрока.
    QUuid *getUuid();

protected:
    // Имя (логин) игрока.
    QString name;

    // Временный идентификатор игрока на время прибывания на сервере.
    QUuid *uuid;

    // Позиция игрока (север, восток, юг, запад)
    PlayerPosition position;

    // Команда игрока (NS , EW)
    Team team = Team::NONE_TEAM;
signals:
    void rxBidSelected(Bid bid);
    void rxMoveSelected(Card card);
};

#endif // PLAYER_H
