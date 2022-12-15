#include "servergamestate.h"

// Инициализирует все атрибуты значениями для начала совпадения.
ServerGameState::ServerGameState(QObject *parent) : QObject(parent)
{
    // Инициализация полей.
    gamePhase = BIDDING;
    rubberNumber = 1;
    gameNumber = 1;
    dealNumber = 0;
    trickNumber = 0;
    passCount = 0;
    dealerPosition = NORTH;
    playerTurnPosition = dealerPosition;
    handToPlayPosition = NORTH;
    declarerPosition = NORTH;

    // Инициилизация рук игроков.
    for(qint8 player = NORTH; player <= WEST; ++player)
        playerHands.insert(PlayerPosition(player), CardKit());

    // Заполнить колоду картами.
    for(qint8 suitVal = CLUBS; suitVal <= SPADES; suitVal++){
        CardSuit suit = CardSuit(suitVal);
        for(qint8 rankVal = TWO; rankVal <= ACE; rankVal++){
            CardRank rank = CardRank(rankVal);
            Card card(suit, rank);
            deck.addCard(card);
        }
    }

    // Отправить сигнал о инициилизации начального состояния игры.
    emit gameEvent(INITIALIZE);
}

// Начинаем игру, раздавая все карты игрокам и выбирая игрока для первого хода.
void ServerGameState::startMatch(qint32 maxRubbers)
{
    this->maxRubbers = maxRubbers;
    nextDeal();
    emit gameEvent(BID_START);
}

// Подготовить игру к следующему сделке.
// Сбросить игровые атрибуты, перетасовать колоду и раздать карты.
void ServerGameState::nextDeal()
{
    // Выберите игрока слева от текущего дилера в качестве нового дилера.
    if(dealNumber > 0 || gameNumber > 1)
        dealerPosition = PlayerPosition((dealerPosition + 1) % 4);

    // Обновить атрибуты.
    gamePhase = BIDDING;
    currentBid = nullptr;
    contractBid = nullptr;
    ++ dealNumber;
    trickNumber = 0;
    tricks.clear();
    passCount = 0;
    for(qint8 i = 0; i < 4; ++ i)
        tricksWon[i] = 0;

    // Очистить руки игроков.
    for(qint8 player = NORTH; player <= WEST; ++ player)
        playerHands[PlayerPosition(player)].clearCardkit();

    // Перемешать колоду.
    deck.shuffleCardKit();

    // Сдаем карты по часовой стрелке.
    // Выберите игрока слева от дилера как первого игрока, получившего карту.
    PlayerPosition targetPlayer = PlayerPosition((dealerPosition + 1) % 4);
    for(int index = 0; index < deck.getCardCount(); ++ index){
        playerHands[targetPlayer].addCard(deck.getCardFromPosition(index));
        targetPlayer = PlayerPosition((targetPlayer + 1) % 4);
    }

// Сделайте снимок рук игроков в начале раздачи, чтобы использовать его для проверки почестей при последующем подсчете очков.
    playerHandsSnapshot = playerHands;

// Выбор дилера для первого хода.
    playerTurnPosition = dealerPosition;
}

// Готовим игру к следующему трюку.
void ServerGameState::nextTrick()
{
    ++ trickNumber;
    tricks.append(CardKit());
}

// Готовим следующую игру.
void ServerGameState::nextGame()
{
    matchScore.nextGame();
    dealNumber = 0;
    gameNumber++;
}

// Готовим следующий руббер.
void ServerGameState::nextRubber()
{
    // Создаем новый экземпляр счета для следующей накладки с обратным счетом.
    quint32 backScore[2] = {0, 0};
    quint32 totalScoreNS = matchScore.getTotalScore(N_S);
    quint32 totalScoreEW = matchScore.getTotalScore(E_W);
    if(totalScoreNS > totalScoreEW)
        backScore[N_S] = totalScoreNS - totalScoreEW;
    else
        backScore[E_W] = totalScoreEW - totalScoreNS;
    matchScore = Score(backScore);

    // Обновить счетчики совпадений.
    dealNumber = 0;
    gameNumber = 1;
    rubberNumber++;
}

// Обновить состояние игры на основе последней сделанной ставки.
// Ничего не делает, если ставка недействительна. Действительность предложения должна быть проверена отдельно до
// вызов этой функции isBidValid().
void ServerGameState::updateBidState(const Bid &bid)
{
    // Подсчитываем количество пассов подряд.
    if(bid.getBidCall() == PASS){
        ++ passCount;
    }
    // Проверяем, действительна ли ставка
    else if(isBidValid(bid)){
        // Сброс счетчика проходов
        passCount = 0;

        // Обновить ставку
        if(bid.getBidCall() == DOUBLE_BID || bid.getBidCall() == REDOUBLE_BID){
            currentBid->setBidCall(bid.getBidCall());
        }
        else{
            delete currentBid;
            currentBid = new Bid(bid);
        }
    }
    else {
        return;
    }

    // Выберите игрока слева от самого последнего игрока, чтобы играть на следующий ход
    playerTurnPosition = PlayerPosition((playerTurnPosition + 1) % 4);

    // Сигнал о том, что игрок сделал ставку
    emit gameEvent(PLAYER_BID);

    // Проверяем, была ли сделана ставка.
    if(currentBid == nullptr){
        // Пересдача карт, если было сделано 4 прохода при условии, что ставки не было сделано.
        if(passCount == 4){
            nextDeal();
            // Сигнал о перезапуске торгов.
            emit gameEvent(BID_RESTART);
        }
    }
    else{
        // Проверяем, было ли сделано 3 прохода, если была сделана ставка.
        if(passCount == 3){
            // Переход к фазе воспроизведения.
            gamePhase = CARDPLAY;
            contractBid = currentBid;
            currentBid = nullptr;
            declarerPosition = contractBid->getBidderPosition();
            // Выбор игрока слева от разыгрывающего для первого хода.
            playerTurnPosition = PlayerPosition((declarerPosition + 1) % 4);
            handToPlayPosition = playerTurnPosition;
            nextTrick();
            // Сигнал о том, что фаза карточной игры началась.
            emit gameEvent(BID_END);
            emit gameEvent(PLAY_START);
            emit gameEvent(TRICK_START);
        }
    }
}

// Обновить состояние игры на основе последней карты, сыгранной игроком
// Предполагается, что карта сыграна игроком PlayerTurn из handToPlay
// Ничего не делает, если ставка недействительна. Действительность карты должна быть проверена отдельно перед
// вызов этой функции с помощью isCardValid()
void ServerGameState::updatePlayState(const Card &card)
{
    // Проверяем, действительна ли сыгранная карта
    if(!isCardValid(card))
        return;

    // Карты текущего trick-a.
    CardKit* currentTrick = &tricks[trickNumber - 1];
    currentTrick->addCard(card);

    // Удалить карту из руки игрока.
    qint8 removeIndex = 0;
    while(!(playerHands[handToPlayPosition].getCardFromPosition(removeIndex) == card))
        removeIndex++;
    playerHands[handToPlayPosition].removeCardFromPosition(removeIndex);

    // Проверяем, завершен ли трюк.
    if(currentTrick->getCardCount() == 4){
        // Signal that a player has played a card
        emit gameEvent(PLAYER_MOVED);

        // Определяем победителя
        PlayerPosition winner = determineTrickWinner();

        // Трюки с обновлением будут учитываться
        tricksWon[winner] += 1;

        // Проверяем завершен ли раунд сделки
        if(tricks.size() == 13){
            // Обновить счет
            matchScore.updateScore(*contractBid, playerHandsSnapshot, getTricksWonFromTeam(contractBid->getBiddingTeam()));

            // Проверяем, выиграла ли команда вторую игру и, следовательно, накладка
            if(matchScore.isRubberWinner()){
                // Завершение бонуса и очков резины для текущей резины
                matchScore.finaliseRubber();

                // Проверяем, готовы ли накладки для матча
                if(rubberNumber == maxRubbers){
                    // Конец совпадения
                    emit gameEvent(TRICK_END);
                    emit gameEvent(PLAY_END);
                    emit gameEvent(MATCH_END);
                    return;
                }else{
                    // Инициализировать следующую резину
                    emit gameEvent(RUBBER_COMPLETED);
                    nextRubber();
                }

            }
            // Проверяем, выиграла ли команда игру
            else if(matchScore.isGameWinner()){
                // Инициализировать следующую игру
                nextGame();
            }

            // Инициализировать следующую сделку
            nextDeal();

            // Сигнал о том, что трюк и игра завершены
            emit gameEvent(TRICK_END);
            emit gameEvent(PLAY_END);
            emit gameEvent(BID_START);
            return;
        }

        // Победитель играет первым, следующим
        handToPlayPosition = winner;
        if(handToPlayPosition == getDummyPosition())
            playerTurnPosition = declarerPosition;
        else
            playerTurnPosition = handToPlayPosition;

        // Сигнал о том, что трюк выполнен
        emit gameEvent(TRICK_END);

        nextTrick();

        // Сигнал о начале следующего трюка
        emit gameEvent(TRICK_START);
    }
    // Получить следующую раздачу и позицию игрока
    else{
        handToPlayPosition = PlayerPosition((handToPlayPosition + 1) % 4);
        if(handToPlayPosition == getDummyPosition())
            playerTurnPosition = declarerPosition;
        else
            playerTurnPosition = handToPlayPosition;

        // Сигнал о том, что игрок разыграл карту
        emit gameEvent(PLAYER_MOVED);
    }
}

// Получить карты, находящиеся в данный момент в колоде
const CardKit& ServerGameState::getDeck()
{
    return deck;
}

// Генерируем и возвращаем состояние игры, адаптированное для игрока
PlayerGameState ServerGameState::getPlayerGameState(PlayerPosition player, QVector<ClientNetwork*> players, GameEvent gameEvent)
{
    // Create player positions map and card count map
    QMap<PlayerPosition, QString> playerPositions;
    QMap<PlayerPosition, qint8> playerCardCount;
    for(const ClientNetwork* client: players){
        playerPositions.insert(client->getPosition(), client->getName());
        playerCardCount.insert(client->getPosition(), playerHands.value(client->getPosition()).getCardCount());
    }

    // Инициализировать пустую руку, отправленную игроку
    CardKit dummyHand;
    if(gamePhase == CARDPLAY)
        dummyHand = playerHands[getDummyPosition()];
    return PlayerGameState(*this, gameEvent, playerPositions, playerCardCount, playerHands[player], dummyHand);
}

// Получить словарь <позиция, карты игрока>.
const QMap<PlayerPosition, CardKit>& ServerGameState::getPlayerHands() const
{
    return playerHands;
}

// Установить словарь <позиция, карты игрока>.
void ServerGameState::setPlayerHands(const QMap<PlayerPosition, CardKit> &playerHands)
{
    this->playerHands = playerHands;
}

// Проверяем, действительна ли новая ставка с учетом текущей ставки. Передача nullptr в качестве текущей ставки
// аргумент подразумевает отсутствие текущей ставки
bool ServerGameState::isBidValid(const Bid &bid) const
{
    if(bid.getBidCall() == PASS){
        return true;
    }
    // Проверяем, действительна ли новая ставка, учитывая, что ставки еще не было сделано.
    else if(currentBid == nullptr){
        return bid.getBidCall() == BID;
    }
    else{
        Team currentBidderTeam = getPlayerTeam(currentBid->getBidderPosition());
        Team newBidderTeam = getPlayerTeam(bid.getBidderPosition());
        if(bid.getBidCall() == DOUBLE_BID){
            // Контра недействительна, если ставку сделал член той же команды
            return newBidderTeam != currentBidderTeam && currentBid->getBidCall() == BID;
        }
        else if(bid.getBidCall() == REDOUBLE_BID){
            // Руконтра недействителен, если ставка была сделана членом противоположной команды или
            // ставка была не контра
            return newBidderTeam == currentBidderTeam && currentBid->getBidCall() == DOUBLE_BID;
        }
        // Проверяем, является ли новая ставка более высокой, чем текущая ставка
        else if(bid > *currentBid){
            return true;
        }
    }
    return false;
}

// Проверяем, действительна ли карта для использования в текущем trick. Предполагается, что карта разыгрывается из
// руки указанной в handToPlay.
bool ServerGameState::isCardValid(const Card &card) const
{
    // Проверяем, есть ли указанная карта в руке у игрока.
    if(!playerHands[handToPlayPosition].containsCard(card))
        return false;

    // Проверяем, были ли сыграны какие-либо карты в текущем trick-e.
    CardKit trick = tricks[trickNumber - 1];
    if(trick.getCardCount() == 0)
        return true;

    // Проверяем, соответствует ли карта масти первой карты, сыгранной в trick-e.
    if(card.getCardSuit() == trick.getCardFromPosition(0).getCardSuit())
        return true;
    // Проверяем, была ли у игрока первая масть в руке
    else if(!playerHands[handToPlayPosition].containsSuit(trick.getCardFromPosition(0).getCardSuit()))
        return true;
    return false;
}

// Получаем команду, к которой принадлежит игрок, исходя из его позиции.
Team ServerGameState::getPlayerTeam(PlayerPosition player){
    switch (player) {
        case NORTH:
        case SOUTH:
            return N_S;
        default:
            return E_W;
    }
}

// Определяем игрока, выигравшего последнюю сыгранный trick.
PlayerPosition ServerGameState::determineTrickWinner() const{
    const CardKit &trick = tricks[trickNumber - 1];
    qint8 bestIndex = 0;
    for(qint8 index = 1; index < 4; ++ index){
        const Card &bestCard = trick.getCardFromPosition(bestIndex);
        const Card &card = trick.getCardFromPosition(index);
        CardSuit trumpSuit = contractBid->getTrumpCardSuit();
        bool updateBestCard = false;

        // Проверяем, является ли масть карты козырной
        if(card.getCardSuit() == trumpSuit && bestCard.getCardSuit() != trumpSuit)
            updateBestCard = true;
        // Проверяем, лучше ли карта, чем лучшая карта, если та же масть
        else if(card.getCardSuit() == bestCard.getCardSuit() && bestCard < card)
            updateBestCard = true;
        // Обновить лучшую карту
        if(updateBestCard)
            bestIndex = index;
    }

    // Определяем победившего игрока
    // playerTurn относится к игроку, сыгравшему последнюю карту во взятке
    // Добавьте 1, чтобы получить игрока, сыгравшего первую карту, затем добавьте лучший индекс, чтобы получить игрока-победителя
    return PlayerPosition((handToPlayPosition + bestIndex + 1) % 4);
}
