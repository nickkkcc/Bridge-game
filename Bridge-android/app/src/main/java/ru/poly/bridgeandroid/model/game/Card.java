package ru.poly.bridgeandroid.model.game;

import com.google.gson.annotations.SerializedName;

import ru.poly.bridgeandroid.enums.CardRank;
import ru.poly.bridgeandroid.enums.CardSuit;

public class Card {

    @SerializedName("suit")
    private final int suit;

    @SerializedName("rank")
    private final int rank;

    public Card(int suit, int rank) {
        this.suit = suit;
        this.rank = rank;
    }

    public CardSuit getSuit() {
        return CardSuit.getCardSuit(suit);
    }

    public CardRank getRank() {
        return CardRank.getCardRank(rank);
    }
}
