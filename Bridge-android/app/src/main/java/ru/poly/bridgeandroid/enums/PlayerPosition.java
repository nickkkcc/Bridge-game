package ru.poly.bridgeandroid.enums;

public enum PlayerPosition {

    NORTH(0),
    EAST(1),
    SOUTH(2),
    WEST(3);

    private final int positionIndex;

    PlayerPosition(int positionIndex) {
        this.positionIndex = positionIndex;
    }

    public int getPositionIndex() {
        return positionIndex;
    }

    public static PlayerPosition getPlayerPosition(int positionIndex) {
        for (PlayerPosition playerPosition : values()) {
            if (playerPosition.positionIndex == positionIndex) {
                return playerPosition;
            }
        }
        return null;
    }
}
