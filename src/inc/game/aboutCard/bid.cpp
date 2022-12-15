#include "bid.h"

Bid::Bid() {}

// Конструктор, используемый, когда игрок делает любой вызов, не являющийся BID
Bid::Bid(PlayerPosition bidderPosition, BidCall bidCall)
{
    this->bidderPosition = bidderPosition;
    this->bidCall = bidCall;
}

// Конструктор, используемый, когда игрок делает вызов, который является BID
Bid::Bid(PlayerPosition bidderPosition, CardSuit trumpCardSuit, qint8 tricksValue)
{
    this->bidCall = BID;
    this->bidderPosition = bidderPosition;
    this->trumpCardSuit = trumpCardSuit;
    this->tricksValue = tricksValue;
}

// Геттер для позиции игрока, сделавшего заявку.
PlayerPosition Bid::getBidderPosition() const
{
    return this->bidderPosition;
}

// Геттер для заявки, сделанной игроком.
BidCall Bid::getBidCall() const
{
    return this->bidCall;
}

// Геттер козырной масти, предложенной в заявке.
CardSuit Bid::getTrumpCardSuit() const
{
    return this->trumpCardSuit;
}

// Получить команду, к которой принадлежит игрок делающий заявку.
Team Bid::getBiddingTeam() const
{
    switch (this->bidderPosition) {
        case NORTH:
        case SOUTH:
            return N_S;
        default:
            return E_W;
    }
}

// Геттер для количества взяток выше 6, выбранного в ставке.
qint8 Bid::getTricksValue() const
{
    return this->tricksValue;
}

// Сеттер для установления заявки (Заявки, пассб контра - реконтра).
void Bid::setBidCall(BidCall bidCall){
    this->bidCall = bidCall;
}

// Инициализировать атрибуты завяки из объекта JSON.
void Bid::readFromJson(const QJsonObject &bid)
{
    this->bidderPosition = PlayerPosition(bid["bidder"].toInt());
    this->bidCall = BidCall(bid["call"].toInt());
    this->trumpCardSuit = CardSuit(bid["trump_suit"].toInt());
    this->tricksValue = bid["tricks_above"].toInt();
}

// Добавляем атрибуты экземпляра Bid в аргумент объекта JSON
void Bid::writeToJson(QJsonObject &bid) const
{
    bid["bidder"] = this->bidderPosition;
    bid["call"] = this->bidCall;
    bid["trump_suit"] = this->trumpCardSuit;
    bid["tricks_above"] = this->tricksValue;
}
// Перегруженный > оператор отношения. Возвращает true, если заявка в аргументе меньше по величине заявки (цифра).
// Если величина заявки одинаковые сравниваются масти. Если масти равны заявки сравниваются по значимости (реконтра > контра > ставка > пасс).
bool Bid::operator >(const Bid& bid) const
{
    if(this->tricksValue == bid.getTricksValue()){
        if(this->trumpCardSuit == bid.getTrumpCardSuit())
            return this->bidCall > bid.getBidCall();
        return this->trumpCardSuit > bid.getTrumpCardSuit();
    }
    return this->tricksValue > bid.getTricksValue();
}

// Перегружен == реляционный оператор. Заявки равны, если равны их значимости по величине заявки, действия или масти.
bool Bid::operator ==(const Bid& bid) const
{
    if(this->bidCall == PASS && bid.getBidCall() == PASS){
        return true;
    }
    else if(this->bidCall == bid.getBidCall() && this->trumpCardSuit == bid.getTrumpCardSuit()
            && this->tricksValue == bid.getTricksValue()){
        return true;
    }
    return false;
}
