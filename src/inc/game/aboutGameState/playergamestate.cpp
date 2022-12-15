#include "playergamestate.h"
#include <QJsonArray>
#include <QVariantMap>

PlayerGameState::PlayerGameState() {}

PlayerGameState::PlayerGameState(GamePhase phase, const Bid* currentBid, const Bid* contractBid,
                                 qint8 gameNumber, qint8 dealNumber, qint8 trickNumber,
                                 QVector<CardKit> tricks, qint8 tricksWon[4], PlayerPosition playerTurn,
                                 PlayerPosition handToPlay, PlayerPosition dealer,
                                 PlayerPosition declarer, Score score,
                                 GameEvent gameEvent, QMap<PlayerPosition, QString> playerPositions,
                                 QMap<PlayerPosition, qint8> playerCardCount, CardKit playerHand, CardKit dummyHand)
{
    this->gamePhase = phase;
    if(currentBid == nullptr)
        this->currentBid = nullptr;
    else
        this->currentBid = new Bid(*currentBid);
    if(contractBid == nullptr)
        this->contractBid = nullptr;
    else
        this->contractBid = new Bid(*contractBid);
    this->rubberNumber = 1;
    this->gameNumber = gameNumber;
    this->dealNumber = dealNumber;
    this->trickNumber = trickNumber;
    this->tricks = tricks;
    this->tricksWon[NORTH] = tricksWon[NORTH];
    this->tricksWon[EAST] = tricksWon[EAST];
    this->tricksWon[SOUTH] = tricksWon[SOUTH];
    this->tricksWon[WEST] = tricksWon[WEST];
    this->playerTurnPosition = playerTurn;
    this->handToPlayPosition = handToPlay;
    this->dealerPosition = dealer;
    this->declarerPosition = declarer;
    this->matchScore = score;
    this->gameEvent = gameEvent;
    this->playerPositions = playerPositions;
    this->playerCardCount = playerCardCount;
    this->playerHandCardKit = playerHand;
    this->dummyHandCardKit = dummyHand;
}

PlayerGameState::PlayerGameState(const GameState &gameState, GameEvent gameEvent,
                                 QMap<PlayerPosition, QString> playerPositions,
                                 QMap<PlayerPosition, qint8> playerCardCount,
                                 CardKit playerHandCardKit, CardKit dummyHandCardKit)
{
    gamePhase = gameState.getGamePhase();
    if(gameState.getCurrentBid() == nullptr)
        currentBid = nullptr;
    else
        currentBid = new Bid(*gameState.getCurrentBid());
    if(gameState.getContractBid() == nullptr)
        contractBid = nullptr;
    else
        contractBid = new Bid(*gameState.getContractBid());
    rubberNumber = gameState.getRubberNumber();
    gameNumber = gameState.getGameNumber();
    dealNumber = gameState.getDealNumber();
    trickNumber = gameState.getTrickNumber();
    tricks = gameState.getTricks();
    this->tricksWon[NORTH] = gameState.getTricksWonFromPosition(NORTH);
    this->tricksWon[EAST] = gameState.getTricksWonFromPosition(EAST);
    this->tricksWon[SOUTH] = gameState.getTricksWonFromPosition(SOUTH);
    this->tricksWon[WEST] = gameState.getTricksWonFromPosition(WEST);
    playerTurnPosition = gameState.getPlayerTurnPosition();
    handToPlayPosition = gameState.getHandToPlayPosition();
    dealerPosition = gameState.getDealerPosition();
    declarerPosition = gameState.getDeclarerPosition();
    matchScore = gameState.getScore();
    this->gameEvent = gameEvent;
    this->playerPositions = playerPositions;
    this->playerCardCount = playerCardCount;
    this->playerHandCardKit = playerHandCardKit;
    this->dummyHandCardKit = dummyHandCardKit;
}

// Получить последнее событие игры.
GameEvent PlayerGameState::getEvent()
{
    return this->gameEvent;
}

// Получить набор карт в руке игрока.
CardKit PlayerGameState::getPlayerHandCardKit()
{
    return this->playerHandCardKit;
}

// Поулчить набор карт в руке у дурочка.
CardKit PlayerGameState::getDummyHandCardKit()
{
    return this->dummyHandCardKit;
}

// Поулчить имя (логин игрока) по указанной позиции.
QString PlayerGameState::getPlayerNameFromPosition(PlayerPosition position)
{
    return this->playerPositions.value(position);
}

// Получить количество карт игрока по указанной позиции.
qint8 PlayerGameState::getPlayerCardCountFromPosition(PlayerPosition position)
{
    return this->playerCardCount.value(position);
}

// Десериализация игрового состояния для игрока из json.
void PlayerGameState::readFromJson(const QJsonObject &json)
{
    this->gamePhase = GamePhase(json["phase"].toInt());
    this->rubberNumber = json["rubber_number"].toInt();
    this->gameNumber = json["game_number"].toInt();
    this->dealNumber = json["deal_number"].toInt();
    this->trickNumber = json["trick_number"].toInt();
    this->playerTurnPosition = PlayerPosition(json["player_turn"].toInt());
    this->handToPlayPosition = PlayerPosition(json["hand_to_play"].toInt());
    this->dealerPosition = PlayerPosition(json["dealer"].toInt());
    this->declarerPosition = PlayerPosition(json["declarer"].toInt());

    if(json["current_bid"].isNull()){
        this->currentBid = nullptr;
    }
    else{
        if(this->currentBid == nullptr)
            this->currentBid = new Bid();
        this->currentBid->readFromJson(json["current_bid"].toObject());
    }
    if(json["contract_bid"].isNull()){
        this->contractBid = nullptr;
    }
    else{
        if(this->contractBid == nullptr)
            this->contractBid = new Bid();
        this->contractBid->readFromJson(json["contract_bid"].toObject());
    }
    this->matchScore.readFromJson(json["score"].toObject());

    this->gameEvent = GameEvent(json["game_event"].toInt());

    this->playerHandCardKit.readFromJson(json["player_hand"].toObject());
    this->dummyHandCardKit.readFromJson(json["dummy_hand"].toObject());

    QJsonArray jsonTricks = json["tricks"].toArray();
    this->tricks.clear();
    this->tricks.reserve(jsonTricks.size());
    for (qint8 index = 0; index < jsonTricks.size(); ++ index) {
        QJsonObject jsonTrick = jsonTricks[index].toObject();
        CardKit trick;
        trick.readFromJson(jsonTrick);
        this->tricks.append(trick);
    }

    this->playerPositions.clear();
    QJsonArray playerPositionKeys = json["player_position_keys"].toArray();
    QJsonArray playerPositionValues = json["player_position_values"].toArray();
    for(qint8 index = 0; index < playerPositionKeys.size(); ++index){
        PlayerPosition key = PlayerPosition(playerPositionKeys[index].toInt());
        QString value = playerPositionValues[index].toString();
        this->playerPositions.insert(key, value);
    }

    this->playerCardCount.clear();
    QJsonArray playerCardCountKeys = json["player_card_count_keys"].toArray();
    QJsonArray playerCardCountValues = json["player_card_count_values"].toArray();
    for(qint8 index = 0; index < playerCardCountKeys.size(); ++index){
        PlayerPosition key = PlayerPosition(playerCardCountKeys[index].toInt());
        qint8 value = playerCardCountValues[index].toInt();
        this->playerCardCount.insert(key, value);
    }

    QJsonArray jsonTricksWonArray = json["tricks_won"].toArray();
    for (qint8 index = 0; index < jsonTricksWonArray.size(); ++ index) {
        qint8 tricksWonElement = jsonTricksWonArray[index].toInt();
        this->tricksWon[index] = tricksWonElement;
    }
}

// Сериализовать состояние игры для игрока в json.
void PlayerGameState::writeToJson(QJsonObject &json) const
{
    json["phase"] = this->gamePhase;
    json["rubber_number"] = this->rubberNumber;
    json["game_number"] = this->gameNumber;
    json["deal_number"] = this->dealNumber;
    json["trick_number"] = this->trickNumber;
    json["player_turn"] = this->playerTurnPosition;
    json["hand_to_play"] = this->handToPlayPosition;
    json["dealer"] = this->dealerPosition;
    json["declarer"] = this->declarerPosition;

    if(this->currentBid == nullptr){
        //json["current_bid"] = QJsonValue(QJsonValue::Type::Null);
        json["current_bid"] = QJsonObject();
    }
    else{
        QJsonObject jsonCurrentBid;
        this->currentBid->writeToJson(jsonCurrentBid);
        json["current_bid"] = jsonCurrentBid;
    }
    if(this->contractBid == nullptr){
        //json["contract_bid"] = QJsonValue(QJsonValue::Type::Null);
        json["contract_bid"] = QJsonObject();
    }
    else{
        QJsonObject jsonContractBid;
        this->contractBid->writeToJson(jsonContractBid);
        json["contract_bid"] = jsonContractBid;
    }
    QJsonObject jsonScore;
    this->matchScore.writeToJson(jsonScore);
    json["score"] = jsonScore;

    json["game_event"] = this->gameEvent;

    QJsonObject jsonPlayerHand;
    this->playerHandCardKit.writeToJson(jsonPlayerHand);
    json["player_hand"] = jsonPlayerHand;
    QJsonObject jsonDummyHand;
    this->dummyHandCardKit.writeToJson(jsonDummyHand);
    json["dummy_hand"] = jsonDummyHand;


    QJsonArray jsonTricks;
    for (const CardKit &trick: this->tricks) {
        QJsonObject jsonTrick;;
        trick.writeToJson(jsonTrick);
        jsonTricks.append(jsonTrick);
    }
    json["tricks"] = jsonTricks;

    QMapIterator<PlayerPosition, QString> iter1(this->playerPositions);
    QJsonArray playerPositionKeys;
    QJsonArray playerPositionValues;
    while (iter1.hasNext()) {
        iter1.next();
        playerPositionKeys.append(iter1.key());
        playerPositionValues.append(iter1.value());
    }
    json["player_position_keys"] = playerPositionKeys;
    json["player_position_values"] = playerPositionValues;

    QMapIterator<PlayerPosition, qint8> iter2(this->playerCardCount);
    QJsonArray playerCardCountKeys;
    QJsonArray playerCardCountValues;
    while (iter2.hasNext()) {
        iter2.next();
        playerCardCountKeys.append(iter2.key());
        playerCardCountValues.append(iter2.value());
    }
    json["player_card_count_keys"] = playerCardCountKeys;
    json["player_card_count_values"] = playerCardCountValues;

    QJsonArray jsonTricksWonArray;
    for (const qint8 &tricksWonElement: this->tricksWon)
        jsonTricksWonArray.append(tricksWonElement);
    json["tricks_won"] = jsonTricksWonArray;
}


bool PlayerGameState::operator ==(const PlayerGameState& playerGameState) const
{
    if(this->currentBid != nullptr && playerGameState.currentBid != nullptr){
        if(!(*this->currentBid == *playerGameState.currentBid))
            return false;
    } else if(this->currentBid != playerGameState.currentBid)
        return false;

    if(this->contractBid != nullptr && playerGameState.contractBid != nullptr){
        if(!(*this->contractBid == *playerGameState.contractBid))
            return false;
    } else if(this->contractBid != playerGameState.contractBid)
        return false;

    return this->gamePhase == playerGameState.gamePhase &&
            this->rubberNumber == playerGameState.rubberNumber &&
            this->gameNumber == playerGameState.gameNumber &&
            this->dealNumber == playerGameState.dealNumber &&
            this->tricks == playerGameState.tricks &&
            this->tricksWon[NORTH] == tricksWon[NORTH] &&
            this->tricksWon[EAST] == tricksWon[EAST] &&
            this->tricksWon[SOUTH] == tricksWon[SOUTH] &&
            this->tricksWon[WEST] == tricksWon[WEST] &&
            this->playerTurnPosition == playerGameState.playerTurnPosition &&
            this->handToPlayPosition == playerGameState.getHandToPlayPosition() &&
            this->dealerPosition == playerGameState.dealerPosition &&
            this->declarerPosition == playerGameState.declarerPosition &&
            this->matchScore == playerGameState.matchScore &&
            this->gameEvent == playerGameState.gameEvent &&
            this->playerPositions == playerGameState.playerPositions &&
            this->playerCardCount == playerGameState.playerCardCount &&
            this->dummyHandCardKit == playerGameState.dummyHandCardKit;
}
