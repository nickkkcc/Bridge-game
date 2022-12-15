#ifndef CARDSET_H
#define CARDSET_H
#include "inc/game/aboutCard/card.h"

// Представляет из себя любой набор карт в игре (колода, trick, карты на руказ у игроков)
class CardKit
{
public:
    CardKit();
    void addCard(Card card);
    Card removeCardFromPosition(qint8 cardPposition);
    Card removeTopCard();
    Card getCardFromPosition(qint8 cardPosition) const;
    bool containsCard(const Card &card) const;
    bool containsSuit(CardSuit cardSuit) const;
    qint8 getCardCount() const;
    void orderHandSort();
    void clearCardkit();
    void shuffleCardKit();
    void readFromJson(const QJsonObject &cardKit);
    void writeToJson(QJsonObject &cardKit) const;
    bool operator ==(const CardKit& cardKit) const;
private:
    QVector<Card> cards;
};
#endif // CARDSET_H
