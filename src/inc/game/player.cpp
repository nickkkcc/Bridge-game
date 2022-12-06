#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{

}

QString Player::getName() const
{
    return name;
}

PlayerPosition Player::getPosition() const
{
    return position;
}

void Player::setName(QString name)
{
    this->name = name;
}

QUuid *Player::getUuid()
{
    return uuid;
}

void Player::setTeam(Team newTeam)
{
    team = newTeam;
}

Team Player::getTeam() const
{
    return team;
}



void Player::setPosition(PlayerPosition position)
{
    this->position = position;
}
