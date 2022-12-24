package ru.poly.bridgeandroid.model.game;

import com.google.gson.annotations.SerializedName;

import ru.poly.bridgeandroid.enums.BidCall;
import ru.poly.bridgeandroid.enums.CardSuit;
import ru.poly.bridgeandroid.enums.PlayerPosition;

public class Bid {

    @SerializedName("bidder")
    private final int playerPosition;

    @SerializedName("call")
    private final int bidCall;

    @SerializedName("trump_suit")
    private final int cardSuit;

    @SerializedName("tricks_above")
    private final int tricksAbove;

    public Bid(int playerPosition, int bidCall, int cardSuit, int tricksAbove) {
        this.playerPosition = playerPosition;
        this.bidCall = bidCall;
        this.cardSuit = cardSuit;
        this.tricksAbove = tricksAbove;
    }

    public PlayerPosition getPlayerPosition() {
        return PlayerPosition.getPlayerPosition(playerPosition);
    }

    public BidCall getBidCall() {
        return BidCall.getBidCall(bidCall);
    }

    public CardSuit getCardSuit() {
        return CardSuit.getCardSuit(cardSuit);
    }

    public int getTricksAbove() {
        return tricksAbove;
    }
}
