package ru.poly.bridgeandroid.model.game;

import com.google.gson.annotations.SerializedName;

import java.util.Objects;

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

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Bid bid = (Bid) o;
        return playerPosition == bid.playerPosition &&
                bidCall == bid.bidCall &&
                cardSuit == bid.cardSuit &&
                tricksAbove == bid.tricksAbove;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 31 * hash + playerPosition;
        hash = 31 * hash + bidCall;
        hash = 31 * hash + cardSuit;
        hash = 31 * hash + tricksAbove;
        return hash;
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
