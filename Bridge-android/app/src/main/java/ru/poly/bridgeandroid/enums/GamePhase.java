package ru.poly.bridgeandroid.enums;

public enum GamePhase {

    BIDDING(0),
    CARD_PLAY(1);

    private final int phaseIndex;

    GamePhase(int phaseIndex) {
        this.phaseIndex = phaseIndex;
    }

    public int getPhaseIndex() {
        return phaseIndex;
    }

    public static GamePhase getGamePhase(int phaseIndex) {
        for (GamePhase gamePhase : values()) {
            if (gamePhase.phaseIndex == phaseIndex) {
                return gamePhase;
            }
        }
        return null;
    }
}
