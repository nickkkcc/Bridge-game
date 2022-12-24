package ru.poly.bridgeandroid.enums;

public enum CardSuit {

    CLUBS(0),
    DIAMONDS(1),
    HEARTS(2),
    SPADES(3),
    NONE(4);

    private final int suitIndex;

    CardSuit(int suitIndex) {
        this.suitIndex = suitIndex;
    }

    public int getSuitIndex() {
        return suitIndex;
    }

    public static CardSuit getCardSuit(int suitIndex) {
        for (CardSuit cardSuit : values()) {
            if (cardSuit.suitIndex == suitIndex) {
                return cardSuit;
            }
        }
        return null;
    }
}
