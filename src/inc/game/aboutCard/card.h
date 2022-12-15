#ifndef CARD_H
#define CARD_H

#include "inc/enumeration/Enumiration.h"
#include <QJsonObject>

// Представляет собой игральную карту, используемую во время игры в бридж.
// Уникально определяется мастью и рангом
class Card
{
public:
    Card();
    Card(CardSuit suit, CardRank rank);
    CardSuit getCardSuit() const;
    CardRank getCardRank() const;
    void readFromJson(const QJsonObject &card);
    void writeToJson(QJsonObject &card) const;
    bool operator <(const Card& card) const;
    bool operator ==(const Card& card) const;
private:
    CardSuit cardSuit;
    CardRank cardRank;
};
Q_DECLARE_METATYPE(Card);

#endif // CARD_H
