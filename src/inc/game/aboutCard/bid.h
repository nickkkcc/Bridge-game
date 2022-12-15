#ifndef BID_H
#define BID_H

#include "inc/enumeration/Enumiration.h"
#include <QJsonObject>
#include <QtGlobal>
#include <optional>

// Представляет ставку, сделанную игроком на этапе торгов.
class Bid
{
public:
    Bid();
    Bid(PlayerPosition bidderPosition, BidCall bidCall);
    Bid(PlayerPosition bidderPosition, CardSuit trumpCardSuit, qint8 tricksValue);
    PlayerPosition getBidderPosition() const;
    BidCall getBidCall() const;
    CardSuit getTrumpCardSuit() const;
    qint8 getTricksValue() const;
    Team getBiddingTeam() const;
    void setBidCall(BidCall bidCall);
    void readFromJson(const QJsonObject &bid);
    void writeToJson(QJsonObject &bid) const;
    bool operator >(const Bid& bid) const;
    bool operator ==(const Bid& bid) const;
    PlayerPosition bidderPosition; // Позиция игрока, сделавшего ставку.
private:
    BidCall bidCall; // Заявка, сделанный игроком во время его хода торгов.
    CardSuit trumpCardSuit; // Ставка масти как козырная. Актуально, только если вызов BID.
    qint8 tricksValue; // Количество взяток. Актуально, только если вызов BID
};
Q_DECLARE_METATYPE(Bid);

#endif // BID_H
