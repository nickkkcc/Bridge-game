#include "inc/game/aboutCard/cardkit.h"
#include <algorithm>
#include <QJsonArray>

CardKit::CardKit() {}

// Добавить карту в конец набора. Вверх набора находится в индексе 0.
void CardKit::addCard(Card card)
{
    cards.append(card);
}

// Вернуть и удалить карту по позиции в наборе. Вершина набора находится под индексом 0.
Card CardKit::removeCardFromPosition(qint8 cardPosition)
{
    return cards.takeAt(cardPosition);
}

// Вернуть и удалить из набора карту из вершины набора.
Card CardKit::removeTopCard()
{
    return cards.takeAt(0);
}

// Вернуть значение карты из набора без ее удаления. Вершина набора находится под индексом 0.
Card CardKit::getCardFromPosition(qint8 cardPosition) const
{
    return cards.value(cardPosition);
}

// Проверить, содержится ли указанная карта в наборе.
bool CardKit::containsCard(const Card &card) const
{
    return cards.contains(card);
}

// Проверить, содержится ли в наборе хотя бы одна карта с указанной мастью.
bool CardKit::containsSuit(CardSuit cardSuit) const
{
    for(const Card &card: cards){
        if(card.getCardSuit() == cardSuit)
            return true;
    }
    return false;
}

// Вернуть количество карт в текущем наборе карт.
qint8 CardKit::getCardCount() const
{
    return cards.count();
}


// Отсортировать набор карт по значимости карт в игре Бридж (от самых слабых до самых сильных) исходя из перечислений CardRank и CardSuit.
void CardKit::orderHandSort()
{
    std::sort(cards.begin(), cards.end());
}

// Удалить все карты в наборе.
void CardKit::clearCardkit()
{
    cards.clear();
    cards.contains(Card());
}

// Перемешать набор карт случайным образом.
void CardKit::shuffleCardKit()
{
    std::random_shuffle(cards.begin(), cards.end());
}

// Десериализовать набор карт из json.
void CardKit::readFromJson(const QJsonObject &cardKit)
{
    QJsonArray jsonCards = cardKit["cards"].toArray();
    cards.clear();
    cards.reserve(jsonCards.size());
    for (qint8 index = 0; index < jsonCards.size(); ++index) {
        QJsonObject jsonCard = jsonCards[index].toObject();
        Card card;
        card.readFromJson(jsonCard);
        cards.append(card);
    }
}

// Сериализовать набор карт в json.
void CardKit::writeToJson(QJsonObject &cardKit) const
{
    QJsonArray jsonCards;
    for (const Card &card : cards) {
        QJsonObject jsonCard;
        card.writeToJson(jsonCard);
        jsonCards.append(jsonCard);
    }
    cardKit["cards"] = jsonCards;
}

// Два набора равны, если равны карты при одних и тех же индексах.
bool CardKit::operator ==(const CardKit& cardSet) const
{
    return this->cards == cardSet.cards;
}
