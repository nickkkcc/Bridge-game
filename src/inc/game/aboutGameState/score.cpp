#include "score.h"
#include <QJsonArray>

Score::Score() {
    this->contractPoints[N_S].append(0);
    this->contractPoints[E_W].append(0);
}

// Конструктор с учетом очков, заработанных командами за предыдущий руббер.
Score::Score(quint32 backScore[2])
{
    this->contractPoints[N_S].append(0);
    this->contractPoints[E_W].append(0);
    this->backScore[N_S] = backScore[N_S];
    this->backScore[E_W] = backScore[E_W];
}

// Обновить состояние счета на основе контракта, рук игроков, и результатов последнего trick-а.
void Score::updateScore(const Bid &contractBid, QMap<PlayerPosition, CardKit> playerHands, quint8 declarerTricksWon)
{
    // Получение нападающей и защищающейся команды.
    Team biddingTeam = contractBid.getBiddingTeam();
    Team defendingTeam;
    if(biddingTeam == N_S)
        defendingTeam = E_W;
    else
        defendingTeam = N_S;

    // Установление статуса неконтра-контра-реконтра заявки.
    uint multiplier = 1;
    bool doubled = false;
    bool redoubled = false;
    if(contractBid.getBidCall() == DOUBLE_BID){
        doubled = true;
        multiplier = 2;
    }
    else if(contractBid.getBidCall() == REDOUBLE_BID){
        redoubled = true;
        multiplier = 4;
    }

    // Определение уязвимости команды нападающих.
    bool bidderVulnerable = getTeamVulnerable(biddingTeam);

    // Если контракт завершен
    if(declarerTricksWon >= contractBid.getTricksValue() + 6){
        // Выдать очки за каждый trick.
        uint trickPoints;
        uint overtrickPoints;
        switch(contractBid.getTrumpCardSuit()){
            // Некозырная и старшая масть.
            case NONE:
            case SPADES:
            case HEARTS:
                trickPoints = 30;
                overtrickPoints = 30;
                break;

            // Младшие масти.
            default:
                trickPoints = 20;
                overtrickPoints = 20;
                break;
        }

        // Начисление очков за перевыполнение контракта при контре и реконтре.
        if(doubled || redoubled){
            overtrickPoints = 100 * multiplier * 0.5;
            if(bidderVulnerable)
                overtrickPoints *= 2;
        }

        // Применение умножителя при контре и реконтре.
        trickPoints *= multiplier;


        // Корректировка начисления для случая некозырной масти.
        uint noTrumpAdjust = 0;
        if(contractBid.getTrumpCardSuit() == NONE)
            noTrumpAdjust = 10 * multiplier;

        // Обновить очки за контракт.
        this->contractPoints[biddingTeam].last() += (contractBid.getTricksValue() * trickPoints) + noTrumpAdjust;

        // Обновить очки за перевыполнение контракта.
        uint overTricks = declarerTricksWon - (contractBid.getTricksValue() + 6);
        this->overtricks[biddingTeam] += overTricks * overtrickPoints;

        // Применить слэм бонусы.
        if(declarerTricksWon == 12){
            if(bidderVulnerable)
                this->slamBonuses[biddingTeam] += 750;
            else
                this->slamBonuses[biddingTeam] += 500;
        }
        else if(declarerTricksWon == 13){
            if(bidderVulnerable)
                this->slamBonuses[biddingTeam] += 1500;
            else
                this->slamBonuses[biddingTeam] += 1000;
        }

        // Начислить бонусы за контру и реконтру.
        if(doubled)
            this->doubleBonuses[biddingTeam] += 50;
        else if (redoubled)
            this->redoubleBonuses[biddingTeam] += 100;

    }
    // Контракт не был выполнен.
    else{
        // Количество невыполненных контрактов.
        uint numUndertricks = contractBid.getTricksValue() + 6 - declarerTricksWon;

        // Начисление штрафных очков за первый trick.
        if(bidderVulnerable){
            if(doubled)
                this->undertricks[defendingTeam] += 200;
            else if(redoubled)
                this->undertricks[defendingTeam] += 400;
            else
                this->undertricks[defendingTeam] += 100;
        }
        else{
            if(doubled)
                this->undertricks[defendingTeam] += 100;
            else if(redoubled)
                this->undertricks[defendingTeam] += 200;
            else
               this->undertricks[defendingTeam] += 50;
        }

        // Начисление штрафных очков за 2 и 3 trick.
        if(numUndertricks >= 2){
            uint numTricks;
            if(numUndertricks == 2)
                numTricks = 1;
            else if(numUndertricks > 2)
                numTricks = 2;

            // Начисление общих штрафных очков.
            if(bidderVulnerable){
                if(doubled)
                   this->undertricks[defendingTeam] += 300 * numTricks;
                else if(redoubled)
                   this->undertricks[defendingTeam] += 600 * numTricks;
                else
                   this->undertricks[defendingTeam] += 100 * numTricks;
            }
            else{
                if(doubled)
                    this->undertricks[defendingTeam] += 200 * numTricks;
                else if(redoubled)
                    this->undertricks[defendingTeam] += 400 * numTricks;
                else
                    this->undertricks[defendingTeam] += 50 * numTricks;
            }
        }

        // Применяем штрафные очки за 4-ю и последующие trick-и
        // Применить штрафные очки за 2-ю и 3-ю trick-и
        if(numUndertricks >= 4){
            uint numTricks = numUndertricks - 3;

            // Начисление штрафных очков.
            if(bidderVulnerable){
                if(doubled)
                    this->undertricks[defendingTeam] += 300 * numTricks;
                else if(redoubled)
                    this->undertricks[defendingTeam] += 600 * numTricks;
                else
                    this->undertricks[defendingTeam] += 100 * numTricks;
            }
            else{
                if(doubled)
                    this->undertricks[defendingTeam] += 300 * numTricks;
                else if(redoubled)
                    this->undertricks[defendingTeam] += 600 * numTricks;
                else
                    this->undertricks[defendingTeam] += 50 * numTricks;
            }
        }
    }

    // Проверка карт на руках у игроков для начисления honor бонусов.
    if(contractBid.getTrumpCardSuit() == NONE){
        for(qint8 position = NORTH; position <= WEST; ++ position){
            PlayerPosition playerPosition = PlayerPosition(position);
            Team team = getTeam(playerPosition);
            const CardKit &hand = playerHands.value(playerPosition);

            // Проверить имеет ли игрок на руках четыре туза.
            if(hand.containsCard(Card(DIAMONDS, ACE))
                    && hand.containsCard(Card(HEARTS, ACE))
                    && hand.containsCard(Card(DIAMONDS, ACE))
                    && hand.containsCard(Card(HEARTS, ACE))){
                this->honors[team] += 150;
            }
        }
    }
    else{
        for(qint8 position = NORTH; position <= WEST; ++ position){
            PlayerPosition playerPosition = PlayerPosition(position);
            Team team = getTeam(playerPosition);
            const CardKit &hand = playerHands.value(playerPosition);
            CardSuit trumpSuit = contractBid.getTrumpCardSuit();

            // Проверить как много honor масте игроки держат в руках.
            qint8 honorsCount = 0;
            if(hand.containsCard(Card(trumpSuit, TEN)))
                ++ honorsCount;
            if(hand.containsCard(Card(trumpSuit, JACK)))
                ++ honorsCount;
            if(hand.containsCard(Card(trumpSuit, QUEEN)))
                ++ honorsCount;
            if(hand.containsCard(Card(trumpSuit, KING)))
                ++ honorsCount;
            if(hand.containsCard(Card(trumpSuit, ACE)))
                ++ honorsCount;

            // Начисления honor бонусов.
            if(honorsCount == 4)
                this->honors[team] += 100;
            else if(honorsCount == 5)
                this->honors[team] += 150;
        }
    }

}

// Проверяем, есть ли команда, которая выиграла гейм, исходя из контрактных очков.
bool Score::isGameWinner() const
{
    return this->contractPoints[N_S].last() >= 100 ||
           this->contractPoints[E_W].last() >= 100;
}

// Проверяем, есть ли команда, которая выиграла руббер, исходя из контрактных очков.
bool Score::isRubberWinner() const
{
    return getGamesWon(N_S) == 2 || getGamesWon(E_W) == 2;
}

// Получаем победителя игры по текущим очкам контракта.
// Вызовите эту функцию только после того, как isWinner() вернет true.
Team Score::getGameWinner() const
{
    if(this->contractPoints[N_S].last() >= 100)
        return N_S;
    else
        return E_W;
}

// Получаем победителя резины исходя из количества выигранных игр
// Вызовите эту функцию только после того, как isRubberWinner() вернет true
Team Score::getRubberWinner() const
{
    if(getGamesWon(N_S) == 2)
        return N_S;
    else
        return E_W;
}

// Определяем победителя матча на основе контрактных очков и премиальных очков
// Вызовите это, если isMatchDraw() возвращает false, так как это возвращает E_W как победителя, если ничья
Team Score::getMatchWinner() const
{
    if(getTotalScore(N_S) > getTotalScore(E_W))
        return N_S;
    else
        return E_W;
}

// Получает, является ли матч ничьей на основе общего количества очков.
bool Score::isMatchDraw() const
{
    return getTotalScore(N_S) == getTotalScore(E_W);
}

// Если isRubberWinner() возвращает true, возвращает общее количество баллов за законченный руббер.
// Если isRubberWinner() возвращает false, возвращает общее количество очков за незаконченный руббер.
// Вызывать эту функцию только после вызова finaliseRubber()
quint32 Score::getTotalScore(Team team) const
{
    quint32 totalScore = 0;
    for(quint32 points: this->contractPoints[team])
        totalScore += points;
    totalScore += this->backScore[team];
    totalScore += this->overtricks[team];
    totalScore += this->undertricks[team];
    totalScore += this->honors[team];
    totalScore += this->slamBonuses[team];
    totalScore += this->doubleBonuses[team];
    totalScore += this->redoubleBonuses[team];
    totalScore += this->rubberBonuses[team];

    return totalScore;
}

// Если isRubberWinner() возвращает true, вычисляет Rubber бонус за законченный руббер и сохраняет результат в RubberBonus
// Если isRubberWinner() возвращает false, вычисляет Rubber бонус за незаконченный руббер и сохраняет результат в RubberBonus
void Score::finaliseRubber()
{
    if(isRubberWinner()){
        Team rubberWinner = getRubberWinner();
        if(getGamesWon(getOppositeTeam(rubberWinner)) == 0)
            this->rubberBonuses[rubberWinner] += 700;
        else
            this->rubberBonuses[rubberWinner] += 500;
    }
    else{
        // Проверить, если только одна стороная выиграла игру.
        if(getGamesWon(N_S) == 1 && getGamesWon(E_W) == 0)
            this->rubberBonuses[N_S] += 300;
        else if(getGamesWon(N_S) == 0 && getGamesWon(E_W) == 1)
            this->rubberBonuses[E_W] += 300;
        else if(contractPoints[N_S].last() > 0 && this->contractPoints[E_W].last() == 0)
            this->rubberBonuses[N_S] += 100;
        else if(contractPoints[N_S].last() == 0 && this->contractPoints[E_W].last() > 0)
            this->rubberBonuses[E_W] += 100;
    }
}

// Закрыть точки контракта для текущей игры и обнулить для новой игры
// Вызывать эту функцию только после того, как isWinner() вернет true
// Не вызывайте эту функцию при первом создании объекта счета.
void Score::nextGame()
{
    this->contractPoints[N_S].append(0);
    this->contractPoints[E_W].append(0);
}

// Получить команду соперника указанной команды
Team Score::getOppositeTeam(Team team) const
{
    if(team == N_S)
        return E_W;
    else
        return N_S;
}

// Получаем команду, к которой принадлежит игрок, исходя из его позиции
Team Score::getTeam(PlayerPosition position) const
{
    switch (position) {
        case NORTH:
        case SOUTH:
            return N_S;
        default:
            return E_W;
    }
}

// Получить очки контракта, накопленные указанной командой для текущей резины
// Измерение 0 представляет каждую из игр в порядке 1, 2, ...
// Измерение 1 представляет каждый отдельный вход очков контракта для данной игры
const QVector<quint32> Score::getContractPoints(Team team) const
{
    return this->contractPoints[team];
}

// Получить число игр, выигранных в текущем руббере указанной командой
quint8 Score::getGamesWon(Team team) const
{
    quint8 gamesWon = 0;
    for(qint8 i = 0; i < this->contractPoints[team].size(); i++){
        if(this->contractPoints[team].value(i) >= 100)
            ++ gamesWon;
    }
    return gamesWon;
}

// Получить счет за предыдущие рубберы.
quint32 Score::getBackScore(Team team) const
{
    return this->backScore[team];
}

// Получить количество очков овертриков для указанной команды.
quint32 Score::getOvertricks(Team team) const
{
    return this->overtricks[team];
}

// Получить количество очков андертриков для указанной команды.
quint32 Score::getUndertricks(Team team) const
{
    return this->undertricks[team];
}

// Получить honor бонусы для указанной команды.
quint32 Score::getHonors(Team team) const
{
    return this->honors[team];
}

// Получить слэм бонусы для указанной команды.
quint32 Score::getSlamBonuses(Team team) const
{
    return this->slamBonuses[team];
}

// Получить бонусы за контру для указанной команды.
quint32 Score::getDoubleBonuses(Team team) const
{
   return this->doubleBonuses[team];
}

// Получить бонусы за реконтру для указанной команды.
quint32 Score::getReDoubleBonuses(Team team) const
{
   return this->redoubleBonuses[team];
}

// Получить руббер бонусы для указанной команды.
quint32 Score::getRubberBonuses(Team team) const
{
    return this->rubberBonuses[team];
}

// Получить уязвима ли команда для указанной команды.
bool Score::getTeamVulnerable(Team team) const
{
    return getGamesWon(team) > 0;
}

// Десериализация объекта счета из json.
void Score::readFromJson(const QJsonObject &json)
{
    QJsonArray jsonConPointsTeams = json["contract_points"].toArray();
    for (quint8 team = N_S; team <= E_W; ++team)
    {
        QJsonArray jsonConPointsGames = jsonConPointsTeams[team].toArray();
        QVector<quint32> conPointsGames;
        conPointsGames.reserve(jsonConPointsGames.size());
        for (quint8 game = 0; game < jsonConPointsGames.size(); ++game)
        {
            quint32 points = jsonConPointsGames[game].toInt();
            conPointsGames.append(points);
        }
        this->contractPoints[team] = conPointsGames;
    }

    QJsonArray jsonBackScoreArray = json["back_score"].toArray();
    for (quint8 index = 0; index < jsonBackScoreArray.size(); ++index)
    {
        quint32 backScoreElement = jsonBackScoreArray[index].toInt();
        this->backScore[index] = backScoreElement;
    }

    QJsonArray jsonOvertricksArray = json["overtricks"].toArray();
    for (quint8 index = 0; index < jsonOvertricksArray.size(); ++index)
    {
        quint32 overtricksElement = jsonOvertricksArray[index].toInt();
        this->overtricks[index] = overtricksElement;
    }

    QJsonArray jsonUndertricksArray = json["undertricks"].toArray();
    for (quint8 index = 0; index < jsonUndertricksArray.size(); ++index)
    {
        quint32 undertricksElement = jsonUndertricksArray[index].toInt();
        this->undertricks[index] = undertricksElement;
    }

    QJsonArray jsonHonorsArray = json["honors"].toArray();
    for (quint8 index = 0; index < jsonHonorsArray.size(); ++index)
    {
        quint32 honorsElement = jsonHonorsArray[index].toInt();
        this->honors[index] = honorsElement;
    }

    QJsonArray jsonslamBonusesArray = json["slam_bonuses"].toArray();
    for (quint8 index = 0; index < jsonslamBonusesArray.size(); ++index)
    {
        quint32 slamBonusesElement = jsonslamBonusesArray[index].toInt();
        this->slamBonuses[index] = slamBonusesElement;
    }

    QJsonArray jsonDoubleBonusesArray = json["double_bonuses"].toArray();
    for (quint8 index = 0; index < jsonDoubleBonusesArray.size(); ++index)
    {
        quint32 doubleBonusesElement = jsonDoubleBonusesArray[index].toInt();
        this->doubleBonuses[index] = doubleBonusesElement;
    }

    QJsonArray jsonRubberBonusesArray = json["rubber_bonuses"].toArray();
    for (quint8 index = 0; index < jsonRubberBonusesArray.size(); ++index)
    {
        quint32 rubberBonusesElement = jsonRubberBonusesArray[index].toInt();
        this->rubberBonuses[index] = rubberBonusesElement;
    }
}

// Сериализация объекта счета в json.
void Score::writeToJson(QJsonObject &json) const
{
    QJsonArray jsonConPointsTeams;
    for (quint8 team = N_S; team <= E_W; ++team)
    {
        QVector<quint32> conPointsGames = contractPoints[team];
        QJsonArray jsonConPointsGames;
        for (quint8 game = 0; game < conPointsGames.size(); ++game)
        {
            qint64 points = conPointsGames.value(game);
            jsonConPointsGames.push_back(points);
        }
        jsonConPointsTeams.append(jsonConPointsGames);
    }
    json["contract_points"] = jsonConPointsTeams;

    QJsonArray jsonBackScoreArray;
    for (const qint32 &backScoreElement : this->backScore)
    {
        jsonBackScoreArray.append(backScoreElement);
    }
    json["back_score"] = jsonBackScoreArray;

    QJsonArray jsonOvertricksArray;
    for (const qint32 &overTricksElement : this->overtricks)
    {

        jsonOvertricksArray.append(overTricksElement);
    }
    json["overtricks"] = jsonOvertricksArray;

    QJsonArray jsonUndertricksArray;
    for (const qint32 &undertricksElement : this->undertricks)
    {

        jsonUndertricksArray.append(undertricksElement);
    }
    json["undertricks"] = jsonUndertricksArray;

    QJsonArray jsonHonorsArray;
    for (const qint32 &honorsElement : this->honors)
    {
        jsonHonorsArray.append(honorsElement);
    }
    json["honors"] = jsonHonorsArray;

    QJsonArray jsonslamBonusesArray;
    for (const qint32 &slamBonusesElement : this->slamBonuses)
        jsonslamBonusesArray.append(slamBonusesElement);
    json["slam_bonuses"] = jsonslamBonusesArray;

    QJsonArray jsonDoubleBonusesArray;
    for (const qint32 &doubleBonusesElement : this->doubleBonuses)
        jsonDoubleBonusesArray.append(doubleBonusesElement);
    json["double_bonuses"] = jsonDoubleBonusesArray;

    QJsonArray jsonRedoubleBonusesArray;
    for (const qint32 &redoubleBonusesElement : this->redoubleBonuses)
        jsonRedoubleBonusesArray.append(redoubleBonusesElement);
    json["redouble_bonuses"] = jsonRedoubleBonusesArray;

    QJsonArray jsonRubberBonusesArray;
    for (const qint32 &rubberBonusesElement : this->rubberBonuses)
        jsonRubberBonusesArray.append(rubberBonusesElement);
    json["rubber_bonuses"] = jsonRubberBonusesArray;
}

bool Score::operator ==(const Score& score) const
{
    return this->contractPoints[N_S] == score.contractPoints[N_S] &&
            this->contractPoints[E_W] == score.contractPoints[E_W] &&
            this->backScore[N_S] == score.backScore[N_S] &&
            this->backScore[E_W] == score.backScore[E_W] &&
            this->overtricks[N_S] == score.overtricks[N_S] &&
            this->overtricks[E_W] == score.overtricks[E_W] &&
            this->undertricks[N_S] == score.undertricks[N_S] &&
            undertricks[E_W] == score.undertricks[E_W] &&
            this->honors[N_S] == score.honors[N_S] &&
            this->honors[E_W] == score.honors[E_W] &&
            this->slamBonuses[N_S] == score.slamBonuses[N_S] &&
            this->slamBonuses[E_W] == score.slamBonuses[E_W] &&
            this->doubleBonuses[N_S] == score.doubleBonuses[N_S] &&
            this->doubleBonuses[E_W] == score.doubleBonuses[E_W] &&
            this->redoubleBonuses[N_S] == score.redoubleBonuses[N_S] &&
            this->redoubleBonuses[E_W] == score.redoubleBonuses[E_W] &&
            this->rubberBonuses[N_S] == score.rubberBonuses[N_S] &&
            this->rubberBonuses[E_W] == score.rubberBonuses[E_W];
}
