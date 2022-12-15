#include "card.h"

Card::Card() {}


Card::Card(CardSuit cardSuit, CardRank cardRank)
{
    this->cardSuit= cardSuit;
    this->cardRank = cardRank;
}


CardSuit Card::getCardSuit() const
{
    return this->cardSuit;
}


CardRank Card::getCardRank() const
{
    return this->cardRank;
}


void Card::readFromJson(const QJsonObject &card)
{
    this->cardSuit = CardSuit(card["suit"].toInt());
    this->cardRank = CardRank(card["rank"].toInt());
}


void Card::writeToJson(QJsonObject &card) const
{
    card["suit"] = this->cardSuit;
    card["rank"] = this->cardRank;
}

bool Card::operator <(const Card& card) const
{
    if (this->cardSuit == card.getCardSuit())
        return this->cardRank < card.getCardRank();
    else
        return this->cardSuit < card.getCardSuit();
}

bool Card::operator ==(const Card& card) const
{
    return this->cardSuit == card.getCardSuit() && this->cardRank == card.getCardRank();
}
