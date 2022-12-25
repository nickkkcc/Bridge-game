#include "player.h"

Player::Player(QObject *parent)
{
    this->setParent(parent);
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

const QUuid &Player::getUuid() const
{
    return uuid;
}

void Player::setTeam(Team newTeam)
{
    team = newTeam;
}

const Team Player::getTeam() const
{
    return team;
}

void Player::setPosition(PlayerPosition position)
{
    this->position = position;
}
