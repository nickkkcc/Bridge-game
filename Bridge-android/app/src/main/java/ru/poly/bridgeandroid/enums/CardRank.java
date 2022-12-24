package ru.poly.bridgeandroid.enums;

public enum CardRank {

    TWO(1),
    THREE(2),
    FOUR(3),
    FIVE(4),
    SIX(5),
    SEVEN(6),
    EIGHT(7),
    NINE(8),
    TEN(9),
    JACK(10),
    QUEEN(11),
    KING(12),
    ACE(13);

    private final int rankIndex;

    CardRank(int rankIndex) {
        this.rankIndex = rankIndex;
    }

    public int getRankIndex() {
        return rankIndex;
    }

    public static CardRank getCardRank(int rankIndex) {
        for (CardRank cardRank : values()) {
            if (cardRank.rankIndex == rankIndex) {
                return cardRank;
            }
        }
        return null;
    }
}
