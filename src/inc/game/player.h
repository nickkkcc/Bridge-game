#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QUuid>
#include "inc/enumeration/Enumiration.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    QString getName() const;
    PlayerPosition getPosition() const;
    void setPosition(PlayerPosition position);
    void setName(QString name);
    const QUuid &getUuid() const;
    void setUuid(const QUuid &newUuid);
    QUuid *getUuid();

    void setTeam(Team newTeam);

    Team getTeam() const;

protected:
    QString name;
    QUuid *uuid;
    PlayerPosition position;
    Team team = Team::NONE_TEAM;
signals:
};

#endif // PLAYER_H
